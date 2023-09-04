/******************************************************************************
* $HeadURL: $
* $Id: $
*
* Project       : Supervision and Automation
*
* Description   : SUP MATHEXPR
*
* Author        : Ricardo Torres (Ext)
*
* Copyright (c) : 2010-2023 ITER Organization,
*                 CS 90 046
*                 13067 St. Paul-lez-Durance Cedex
*                 France
*
* This file is part of ITER CODAC software.
* For the terms and conditions of redistribution or use of this software
* refer to the file ITER-LICENSE.TXT located in the top level directory
* of the distribution package.
******************************************************************************/

#include <sup/mathexpr/expressioncontext.h>
#include <sup/mathexpr/ivariablestore.h>

#include <gtest/gtest.h>
#include <cstddef>
#include <vector>

using ProcessVariableMap = std::map<std::string, std::vector<double>>;

class vectorhandler : public sup::mathexpr::IVariableStore
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
  sup::mathexpr::ExpressionContext expr_ctx("z:=x+y",&handler);

  try {
    expr_ctx.EvaluateExpression();
  } catch (const std::exception& ex) {
    std::cout << ex.what() << std::endl;
  }

  EXPECT_TRUE(vars.at("z")==a);
}

