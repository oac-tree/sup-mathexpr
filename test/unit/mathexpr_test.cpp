/******************************************************************************
 * $HeadURL: $
 * $Id: $
 *
 * Project       : SUP - MathExpr
 *
 * Description   : Mathematical expression evaluator for SUP
 *
 * Author        : Ricardo Torres (Ext)
 *
 * Copyright (c) : 2010-2025 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 ******************************************************************************/

#include <sup/mathexpr/exceptions.h>
#include <sup/mathexpr/expression_context.h>
#include <sup/mathexpr/i_variable_store.h>

#include <gtest/gtest.h>
#include <cstddef>
#include <vector>

using ProcessVariableMap = std::map<std::string, std::vector<double>>;

using namespace sup::mathexpr;

class TestDoubleHandler : public IVariableStore
{
public:
  TestDoubleHandler() = default;
  ~TestDoubleHandler() = default;

  void AddScalar(const std::string &varname, double val)
  {
    m_var_map[varname] = val;
  }

  VarType GetVariableType(const std::string &varname) const override { return kScalar; }
  bool SetScalar(const std::string &varname, const double &val) override
  {
    if (varname == "readonly")
    {
      return false;
    }
    const auto it = m_var_map.find(varname);
    if (it == m_var_map.end())
    {
      return false;
    }
    it->second = val;
    return true;
  }
  bool GetScalar(const std::string &varname, double &val) const override
  {
    const auto it = m_var_map.find(varname);
    if (it == m_var_map.end())
    {
      return false;
    }
    val = it->second;
    return true;
  }
  bool SetVector(const std::string &varname, const std::vector<double> &val) override
  {
    return false;
  }
  bool GetVector(const std::string &varname, std::vector<double> &val) const override
  {
    return false;
  }
private:
  std::map<std::string, double> m_var_map;
};

class vectorhandler : public IVariableStore
{
public:
  vectorhandler(ProcessVariableMap* vars) : m_vars(vars){};
  VarType GetVariableType(const std::string &varname) const override { return kVector; }

  bool SetScalar(const std::string &varname, const double &val) override { return false; }
  bool GetScalar(const std::string &varname, double &val) const override { return false; }
  bool SetVector(const std::string &varname, const std::vector<double> &val) override
  {
    for (size_t i = 0; i < m_vars->at(varname).size(); ++i)
    {
       m_vars->at(varname)[i] = val[i];
    }
    return true;
  }
  bool GetVector(const std::string &varname, std::vector<double> &val) const override
  {
    val.resize(m_vars->at(varname).size());
    for (size_t i = 0; i < m_vars->at(varname).size(); ++i)
    {
       val[i] = m_vars->at(varname)[i];
    }
    return true;
  }

private:
  ProcessVariableMap* m_vars;
};

class MathexprTest : public ::testing::Test
{
protected:
  MathexprTest() = default;
  virtual ~MathexprTest() = default;
};

TEST_F(MathexprTest, Success)
{
  std::vector<double> x{1,1,1};
  std::vector<double> y{2,3,4};
  std::vector<double> z{0,0,0};
  std::vector<double> a{3,4,5};
  ProcessVariableMap vars;
  vars.emplace("x", x);
  vars.emplace("y", y);
  vars.emplace("z", z);

  vectorhandler handler(&vars);
  ExpressionContext expr_ctx(handler);

  try {
    expr_ctx.EvaluateExpression("z:=x+y");
  } catch (const std::exception& ex) {
    std::cout << ex.what() << std::endl;
  }

  EXPECT_TRUE(vars.at("z")==a);
}

TEST_F(MathexprTest, Exceptions)
{
  TestDoubleHandler handler;
  handler.AddScalar("a", 1.0);
  handler.AddScalar("b", 2.0);
  handler.AddScalar("readonly", 0.0);
  ExpressionContext expr_ctx(handler);
  {
    // Expression needs value from non-exisiting variable
    EXPECT_THROW(expr_ctx.EvaluateExpression("a+c"), ExpressionEvaluateException);
  }
  {
    // Expression cannot be correctly parsed
    EXPECT_THROW(expr_ctx.EvaluateExpression("a +-& b"), ExpressionEvaluateException);
  }
  {
    // Expression needs to write to readonly variable
    EXPECT_THROW(expr_ctx.EvaluateExpression("readonly:=a+b"), ExpressionEvaluateException);
  }
}

TEST_F(MathexprTest, Conditions)
{
  TestDoubleHandler handler;
  handler.AddScalar("a", 1.0);
  handler.AddScalar("b", 2.0);
  handler.AddScalar("c", 0.0);
  ExpressionContext expr_ctx(handler);
  // Expression without assigment returns the expression's value (true)
  EXPECT_TRUE(expr_ctx.EvaluateExpression("a<b"));
  // Expression without assigment returns the expression's value (false)
  EXPECT_FALSE(expr_ctx.EvaluateExpression("a>b"));
  // Expression with assigment returns true if no errors were encountered
  EXPECT_TRUE(expr_ctx.EvaluateExpression("c:=a+b"));
  // Expression with assigment returns true if no errors were encountered, even when the
  // expression itself evaluates to zero (false)
  EXPECT_TRUE(expr_ctx.EvaluateExpression("c:=0"));
}

TEST_F(MathexprTest, UppercaseVars)
{
  TestDoubleHandler handler;
  handler.AddScalar("A", 1.0);
  handler.AddScalar("B", 2.0);
  ExpressionContext expr_ctx(handler);
  EXPECT_TRUE(expr_ctx.EvaluateExpression("A < B"));
}
