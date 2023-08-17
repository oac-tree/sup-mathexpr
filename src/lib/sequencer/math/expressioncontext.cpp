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
#include "sequencer/math/expressionevaluator.h"
#include <sup/dto/anytype.h>
#include <sup/dto/anyvalue.h>
#include <sup/dto/anyvalue_helper.h>
#include <sup/dto/basic_scalar_types.h>
#include <iostream>
#include <utility>

namespace sup
{
namespace math
{

ExpressionContext::ExpressionContext(const std::string& expression)
    : m_expression(expression)
{
}

std::vector<std::string> ExpressionContext::CollectVariables()
{
  std::vector<std::string> variable_list;

  exprtk::collect_variables(m_expression, variable_list);

  return variable_list;
}

bool ExpressionContext::ConvertVariables(InVariableMap in_variables)
{
  for (const auto& entry : in_variables)
  {
    m_proc_vars.emplace(entry.first, entry.second.As<float>());
  }
  if (m_proc_vars.empty())
  {
    return false;
  }
  return true;
}

sup::dto::AnyValue ExpressionContext::EvaluateExpression()
{
  ExpressionEvaluator evaluator(m_expression, m_proc_vars);

  evaluator.Setup();

  float result = evaluator.Evaluate();
  sup::dto::AnyValue output(result);

  return output;
}

}  // namespace math

}  // namespace sup
