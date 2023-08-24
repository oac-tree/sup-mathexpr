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

namespace sup
{
namespace math
{

ExpressionContext::ExpressionContext(const std::string& expression) : m_in_expression(expression)
{
  exprtk::collect_variables(m_in_expression, m_list_vars);

  for (const auto& name : m_list_vars)
  {
    m_proc_vars.emplace(name, std::vector<double>{std::nan("")});
  }
}

ProcessVariableMap::iterator ExpressionContext::begin()
{
  return m_proc_vars.begin();
}

ProcessVariableMap::iterator ExpressionContext::end()
{
  return m_proc_vars.end();
}

void ExpressionContext::ConvertVariable(const std::string& name, dto::AnyValue& val)
{
  // Check if the key exists in the map

  if (m_proc_vars.count(name))
  {
    // If the key exists, get a reference to the vector associated with the key
    auto& vec = m_proc_vars.at(name);
    // Check if the vector is not empty
    if (sup::dto::IsArrayValue(val))
    {
      vec.resize(val.NumberOfElements());
      for (size_t i = 0; i < val.NumberOfElements(); i++)
      {
        vec[i] = val[i].As<double>();
      }
    }
    else
    {
      vec[0] = val.As<double>();
    }
  }
}

void ExpressionContext::ConfigureExpression()
{
  for (auto& entry : m_proc_vars)
  {
    m_symbol_table.add_vector(entry.first, entry.second);
  }

  m_expression.register_symbol_table(m_symbol_table);

  m_parser.dec().collect_assignments() = true;
  m_parser.compile(m_in_expression, m_expression);
  m_parser.dec().assignment_symbols(m_symbol_list);
}

ProcessVariableMap ExpressionContext::EvaluateExpression()
{
  ConfigureExpression();

  m_expression.value();

  ProcessVariableMap output;
  if (m_symbol_list.size() == 0)
  {
    output.emplace("FAILURE", std::vector<double>{std::nan("")});
  }
  else
  {
    for (auto assignment : m_symbol_list)
    {
      output.insert(std::make_pair(assignment.first, m_proc_vars.at(assignment.first)));
    }
  }
  return output;
}

}  // namespace math

}  // namespace sup
