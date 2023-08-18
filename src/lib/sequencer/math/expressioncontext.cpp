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
#include "sequencer/exprtk/exprtk.hpp"
#include <sup/dto/anytype.h>
#include <sup/dto/anyvalue.h>
#include <sup/dto/anyvalue_helper.h>
#include <sup/dto/basic_scalar_types.h>
#include <cmath>
#include <iostream>
#include <string>
#include <utility>

namespace sup
{
namespace math
{

ExpressionContext::ExpressionContext(const std::string& expression)
    : m_in_expression(expression)
{
  exprtk::collect_variables(m_in_expression, m_list_vars);

  for (const auto &name : m_list_vars)
  {
    m_proc_vars.emplace(name, std::nan(""));
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
  m_proc_vars.at(name) = val.As<double>();
}

void ExpressionContext::ConfigureExpression()
{
  for (auto& entry : m_proc_vars)
  {
    m_symbol_table.add_variable(entry.first, entry.second);
  }

  m_expression.register_symbol_table(m_symbol_table);

  m_parser.compile(m_in_expression, m_expression);
}

sup::dto::AnyValue ExpressionContext::EvaluateExpression()
{

  ConfigureExpression();

  sup::dto::AnyValue output(m_expression.value());
  return output;
}

}  // namespace math

}  // namespace sup
