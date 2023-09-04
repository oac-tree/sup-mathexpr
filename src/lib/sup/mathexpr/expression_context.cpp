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

#include "expression_context.h"

#include <sup/exprtk/exprtk.hpp>

#include <deque>
#include <stdexcept>

namespace sup
{
namespace mathexpr
{

ExpressionContext::ExpressionContext(const std::string& expression, IVariableStore& varhandler)
    : m_raw_expression(expression), m_variable_handler(varhandler)
{
  std::vector<std::string> list_vars;
  exprtk::collect_variables(m_raw_expression, list_vars);

  if (!GetVariables(list_vars))
  {
    throw std::invalid_argument("ExpressionContext(): expression variables could not be properly "
                                "read from the variable store.");
  }
}

bool ExpressionContext::EvaluateExpression()
{
  exprtk::symbol_table<double> symbol_table;
  exprtk::expression<double> expression;
  exprtk::parser<double> parser;
  std::deque<exprtk::parser<double>::dependent_entity_collector::symbol_t> assignment_symbol_list;

  for (auto& var : m_data)
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

  expression.register_symbol_table(symbol_table);

  parser.dec().collect_assignments() = true;
  parser.compile(m_raw_expression, expression);
  parser.dec().assignment_symbols(assignment_symbol_list);

  expression.value();

  if (assignment_symbol_list.size() == 0)
  {
    return false;
  }
  for (auto assignment : assignment_symbol_list)
  {
    if (!this->SetVariable(assignment.first))
    {
      return false;
    }
  }
  return true;
}

bool ExpressionContext::GetVariables(std::vector<std::string> list_vars)
{
  std::vector<double> readvector;
  for (const auto& varname : list_vars)
  {
    switch (m_variable_handler.GetVariableType(varname))
    {
    case IVariableStore::kScalar:
      readvector.resize(1);
      if (!m_variable_handler.GetScalar(varname, readvector[0]))
      {
        return false;
      }
      break;
    case IVariableStore::kVector:
      if (!m_variable_handler.GetVector(varname, readvector))
      {
        return false;
      }
      break;
    case IVariableStore::kUnknown:
      return false;
    }
    m_data.emplace(varname, readvector);
  }
  return true;
}

bool ExpressionContext::SetVariable(std::string varname)
{
  switch (m_variable_handler.GetVariableType(varname))
  {
  case IVariableStore::kScalar:
    if (!m_variable_handler.SetScalar(varname, m_data.at(varname)[0]))
    {
      return false;
    }
    break;
  case IVariableStore::kVector:
    if (!m_variable_handler.SetVector(varname, m_data.at(varname)))
    {
      return false;
    }
    break;
  case IVariableStore::kUnknown:
    return false;
  }
  return true;
}

}  // namespace mathexpr

}  // namespace sup
