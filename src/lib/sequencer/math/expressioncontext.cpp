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

#include <cmath>
#include <deque>
#include <iostream>
#include <map>
#include <string>
#include <vector>

namespace sup
{
namespace math
{

ExpressionContext::ExpressionContext(const std::string& expression) : m_raw_expression(expression)
{
  exprtk::collect_variables(m_raw_expression, m_list_vars);

  for (const auto& name : m_list_vars)
  {
    m_data.emplace(name, dto::AnyValue(dto::EmptyType));
  }
}

void ExpressionContext::SetVariableHandler(IVariableStore* var_handler)
{
  m_variable_handler = var_handler;
}

ProcessVariableMap* ExpressionContext::GetVariableList()
{
  return &m_data;
}

bool ExpressionContext::ReadVariables()
{
  for (auto& entry : m_list_vars)
  {
    m_process_data.emplace(entry,std::vector<double>{std::nan("")});
    switch (m_variable_handler->GetVariableType(entry))
    {
    case IVariableStore::VarType::kScalar:
      if (!m_variable_handler->GetScalar(entry, m_process_data.at(entry)[0]))
      {
        return false;
      }
      break;
    case IVariableStore::kVector:
      if (!m_variable_handler->GetVector(entry, m_process_data.at(entry)))
      {
        return false;
      }
      break;
    case IVariableStore::kUnknown:
      return false;
      break;
    }
  }
  return true;
}

bool ExpressionContext::WriteVariable(std::string varname)
{
  switch (m_variable_handler->GetVariableType(varname))
  {
  case IVariableStore::VarType::kScalar:
    if (!m_variable_handler->SetScalar(varname, m_process_data.at(varname)[0]))
    {
      return false;
    }
    break;
  case IVariableStore::kVector:
    if (!m_variable_handler->SetVector(varname, m_process_data.at(varname)))
    {
      return false;
    }
    break;
  case IVariableStore::kUnknown:
    return false;
    break;
  }
  return true;
}

ProcessVariableMap ExpressionContext::EvaluateExpression()
{
  exprtk::symbol_table<double> m_symbol_table;
  exprtk::expression<double> m_expression;
  exprtk::parser<double> m_parser;
  std::deque<exprtk::parser<double>::dependent_entity_collector::symbol_t> m_symbol_list;

  this->ReadVariables();

  for (auto& varname : m_list_vars)
  {
    switch (m_variable_handler->GetVariableType(varname))
    {
    case IVariableStore::VarType::kScalar:
      m_symbol_table.add_variable(varname, m_process_data.at(varname)[0]);
      break;
    case IVariableStore::kVector:
      m_symbol_table.add_vector(varname, m_process_data.at(varname));
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

  ProcessVariableMap output;
  if (m_symbol_list.size() == 0)
  {
    output.emplace("FAILURE", sup::dto::AnyValue(sup::dto::EmptyType));
  }
  else
  {
    for (auto assignment : m_symbol_list)
    {
      auto varname = assignment.first;
      if (this->WriteVariable(varname))
      {
        output.emplace(varname, m_data.at(varname));
      }
      else
      {
        output.emplace("FAILURE", sup::dto::AnyValue(sup::dto::EmptyType));
      }
    }
  }
  return output;
}

}  // namespace math

}  // namespace sup
