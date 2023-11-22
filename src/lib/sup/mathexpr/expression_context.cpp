/******************************************************************************
 * $HeadURL: $
 * $Id: $
 *
 * Project       : SUP - MathExpr
 *
 * Description   : Mathematical expression evaluator for SUP
 *
 * Author        : Ricardo Torres (EXT)
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

// Allow for uppercase variable names:
#define exprtk_disable_caseinsensitivity

#include "expression_context.h"

#include <sup/mathexpr/exceptions.h>

#include <sup/exprtk/exprtk.hpp>

#include <deque>
#include <stdexcept>

namespace
{
constexpr double TRUE_AS_DOUBLE = 1.0;
}

namespace sup
{
namespace mathexpr
{

ExpressionContext::ExpressionContext(IVariableStore& varhandler)
  : m_variable_handler(varhandler)
{}

double ExpressionContext::EvaluateExpression(const std::string& expression)
{
  ProcessVariableMap data_map;
  CollectVariables(data_map, expression);

  exprtk::symbol_table<double> symbol_table;
  exprtk::expression<double> exprtk_expression;
  exprtk::parser<double> parser;
  std::deque<exprtk::parser<double>::dependent_entity_collector::symbol_t> assignment_symbol_list;

  for (auto& var : data_map)
  {
    auto varname = var.first;
    switch (m_variable_handler.GetVariableType(varname))
    {
    case IVariableStore::kScalar:
      symbol_table.add_variable(varname, var.second[0]);
      break;
    case IVariableStore::kVector:
      symbol_table.add_vector(varname, var.second);
      break;
    case IVariableStore::kUnknown:
      break;
    }
  }

  exprtk_expression.register_symbol_table(symbol_table);

  parser.dec().collect_assignments() = true;
  if (!parser.compile(expression, exprtk_expression))
  {
    const std::string error = "ExpressionContext::EvaluateExpression(): expression could "
                              "not be properly parsed.";
    throw ExpressionEvaluateException(error);
  }
  parser.dec().assignment_symbols(assignment_symbol_list);

  auto expr_val = exprtk_expression.value();

  if (assignment_symbol_list.size() == 0)
  {
    return expr_val;
  }
  for (auto assignment : assignment_symbol_list)
  {
    SetVariable(data_map, assignment.first);
  }
  return TRUE_AS_DOUBLE;
}

void ExpressionContext::CollectVariables(ProcessVariableMap& data_map,
                                         const std::string& expression)
{
  std::vector<std::string> list_vars;
  exprtk::collect_variables(expression, list_vars);

  std::vector<double> readvector;
  for (const auto& varname : list_vars)
  {
    switch (m_variable_handler.GetVariableType(varname))
    {
    case IVariableStore::kScalar:
      readvector.resize(1);
      if (!m_variable_handler.GetScalar(varname, readvector[0]))
      {
        const std::string error =
          "ExpressionContext::CollectVariables(): could not retrieve scalar variable with name: [" +
          varname + "] from the variable store.";
        throw ExpressionEvaluateException(error);
      }
      break;
    case IVariableStore::kVector:
      if (!m_variable_handler.GetVector(varname, readvector))
      {
        const std::string error =
          "ExpressionContext::CollectVariables(): could not retrieve array variable with name: [" +
          varname + "] from the variable store.";
        throw ExpressionEvaluateException(error);
      }
      break;
    case IVariableStore::kUnknown:
      const std::string error =
        "ExpressionContext::CollectVariables(): could not determine type of variable with name: [" +
        varname + "] from the variable store.";
      throw ExpressionEvaluateException(error);
    }
    data_map.emplace(varname, readvector);
  }
}

void ExpressionContext::SetVariable(const ProcessVariableMap& data_map, const std::string& varname)
{
  switch (m_variable_handler.GetVariableType(varname))
  {
  case IVariableStore::kScalar:
    if (!m_variable_handler.SetScalar(varname, data_map.at(varname)[0]))
    {
      const std::string error =
        "ExpressionContext::SetVariable(): could not write scalar variable with name: [" +
        varname + "] to the variable store.";
      throw ExpressionEvaluateException(error);
    }
    break;
  case IVariableStore::kVector:
    if (!m_variable_handler.SetVector(varname, data_map.at(varname)))
    {
      const std::string error =
        "ExpressionContext::SetVariable(): could not write array variable with name: [" +
        varname + "] to the variable store.";
      throw ExpressionEvaluateException(error);
    }
    break;
  case IVariableStore::kUnknown:
    const std::string error =
      "ExpressionContext::SetVariable(): could not determine type of variable with name: [" +
      varname + "] to the variable store.";
    throw ExpressionEvaluateException(error);
  }
}

}  // namespace mathexpr

}  // namespace sup
