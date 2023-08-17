/******************************************************************************
 * $HeadURL: $
 * $Id: $
 *
 * Project       : SUP - Sequencer
 *
 * Description   : Sequencer for operational procedures
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

#include "math_instruction.h"

#include "sequencer/math/expressioncontext.h"
#include "sup/sequencer/execution_status.h"
#include "sup/sequencer/workspace.h"

#include <sup/dto/anytype.h>
#include <sup/dto/anyvalue.h>
#include <sup/dto/basic_scalar_types.h>
#include <sup/sequencer/constants.h>
#include <sup/sequencer/exceptions.h>
#include <sup/sequencer/instruction_registry.h>
#include <sup/sequencer/instruction_utils.h>
#include <sup/sequencer/generic_utils.h>

#include <sup/dto/anyvalue_helper.h>
#include <iostream>
#include <string>
#include <vector>

const std::string EXPR_STRING_ATTR_NAME = "expression";
const std::string OUT_STRING_ATTR_NAME = "outVar";

namespace sup
{
namespace sequencer
{

const std::string Math::Type = "Math";
static bool _wait_for_condition_initialised_flag =
  RegisterGlobalInstruction<Math>();


Math::Math()
  : Instruction(Math::Type)
{
  AddAttributeDefinition(EXPR_STRING_ATTR_NAME, sup::dto::StringType).SetMandatory();
  AddAttributeDefinition(OUT_STRING_ATTR_NAME, sup::dto::StringType).SetMandatory();
}

Math::~Math() = default;

ExecutionStatus Math::ExecuteSingleImpl(UserInterface& ui, Workspace& ws)
{
  auto expression = GetAttributeValue<std::string>(EXPR_STRING_ATTR_NAME);
  sup::math::ExpressionContext expr_ctx(expression);

  std::vector<std::string> var_names = expr_ctx.CollectVariables();
  if (var_names.empty())
  {
    return ExecutionStatus::FAILURE;
  }

  sup::dto::AnyValue value;
  std::map<std::string, sup::dto::AnyValue> ws_vars;
  for (const auto& var_name : var_names)
  {
    // read the variable from the workspace
    if (!ws.HasVariable(var_name) || !ws.GetValue(var_name, value))
    {
      return ExecutionStatus::FAILURE;
    }
    // Only numeric and array types are accepted
    if (value.GetTypeCode() == dto::TypeCode::Char8 || value.GetTypeCode() == dto::TypeCode::String
        || value.GetTypeCode() == dto::TypeCode::Struct)
    {
      return ExecutionStatus::FAILURE;
    }
    ws_vars.emplace(var_name, value);
  }

  // convert input variables to be processed by the math library
  if (!expr_ctx.ConvertVariables(ws_vars))
  {
    return ExecutionStatus::FAILURE;
  }

  sup::dto::AnyType in_type(value.GetType());
  sup::dto::AnyValue return_value(in_type);
  sup::dto::AnyValue output;
  output = expr_ctx.EvaluateExpression();

  // convert the output value to the same type as the input
  if (!sup::dto::TryConvert(return_value, output))
  {
    return ExecutionStatus::FAILURE;
  }

  auto result = GetAttributeValue<std::string>(OUT_STRING_ATTR_NAME);

  if (!ws.HasVariable(result) || !ws.SetValue(result, return_value))
  {
    return ExecutionStatus::FAILURE;
  }
  return ExecutionStatus::SUCCESS;
}

}  // namespace sequencer

}  // namespace sup
