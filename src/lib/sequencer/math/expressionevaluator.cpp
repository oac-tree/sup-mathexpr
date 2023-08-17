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

#include "expressionevaluator.h"

namespace sup
{
namespace math
{

  ExpressionEvaluator::ExpressionEvaluator(const std::string& expression, ProcessVariableMap vars) : m_s_expression(expression), m_variablemap(vars) {}


void ExpressionEvaluator::Setup()
{
  for (const auto& entry : m_variablemap)
  {
    m_symbol_table.add_constant(entry.first, entry.second);
  }

  m_expression.register_symbol_table(m_symbol_table);

  m_parser.compile(m_s_expression, m_expression);
}

float ExpressionEvaluator::Evaluate()
{
  return m_expression.value();
}

}  // namespace math

}  // namespace sup
