/******************************************************************************
 * $HeadURL: $
 * $Id: $
 *
 * Project       : SUP - Sequencer
 *
 * Description   : SUP sequencer math plugin
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

#include "expressioncontext.h"

#include "sequencer/math/ivariablestore.h"
#include "sequencer/math/variable_handler.h"

#include <sequencer/exprtk/exprtk.hpp>
#include <sup/dto/anytype.h>
#include <sup/dto/anyvalue.h>

#include <deque>
#include <map>
#include <stdexcept>
#include <string>
#include <vector>
#include <exception>

namespace sup
{
namespace math
{

ExpressionContext::ExpressionContext(const std::string& expression, IVariableStore* varhandler)
    : m_raw_expression(expression), m_variable_handler(varhandler)
{
  std::vector<std::string> list_vars;
  exprtk::collect_variables(m_raw_expression, list_vars);

  if (!this->GetVariables(list_vars))
  {
    throw std::invalid_argument("Invalid variable list.");
  }
}

bool ExpressionContext::GetVariables(std::vector<std::string> list_vars)
{
  std::vector<double> readvector;
  for (const auto& varname : list_vars)
  {
    switch (m_variable_handler->GetVariableType(varname))
    {
    case IVariableStore::kScalar:
      readvector.resize(1);
      if (!m_variable_handler->GetScalar(varname, readvector[0]))
      {
        return false;
      }
      break;
    case IVariableStore::kVector:
      if (!m_variable_handler->GetVector(varname, readvector))
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
  switch (m_variable_handler->GetVariableType(varname))
  {
  case IVariableStore::kScalar:
    if (!m_variable_handler->SetScalar(varname, m_data.at(varname)[0]))
    {
      return false;
    }
    break;
  case IVariableStore::kVector:
    if (!m_variable_handler->SetVector(varname, m_data.at(varname)))
    {
      return false;
    }
    break;
  case IVariableStore::kUnknown:
    return false;
  }
  return true;
}

bool ExpressionContext::EvaluateExpression()
{
  exprtk::symbol_table<double> m_symbol_table;
  exprtk::expression<double> m_expression;
  exprtk::parser<double> m_parser;
  std::deque<exprtk::parser<double>::dependent_entity_collector::symbol_t> m_symbol_list;

  for (auto& var : m_data)
  {
    auto varname = var.first;
    switch (m_variable_handler->GetVariableType(varname))
    {
    case IVariableStore::kScalar:
      m_symbol_table.add_variable(varname, var.second[0]);
      break;
    case IVariableStore::kVector:
      m_symbol_table.add_vector(varname, var.second);
      break;
    case IVariableStore::kUnknown:
      break;
    }
  }

  m_expression.register_symbol_table(m_symbol_table);

  m_parser.dec().collect_assignments() = true;
  m_parser.compile(m_raw_expression, m_expression);
  m_parser.dec().assignment_symbols(m_symbol_list);

  m_expression.value();

  if (m_symbol_list.size() == 0)
  {
    return false;
  }
  for (auto assignment : m_symbol_list)
  {
    if (!this->SetVariable(assignment.first))
    {
      return false;
    }
  }
  return true;
}

}  // namespace math

}  // namespace sup
