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

#include <sequencer/math/expressioncontext.h>
#include <sup/dto/anytype.h>
#include <sup/dto/anyvalue.h>
#include <sup/dto/anyvalue_helper.h>
#include <sup/dto/basic_scalar_types.h>
#include <sup/sequencer/generic_utils.h>
#include <sup/sequencer/instruction_registry.h>
#include <sup/sequencer/user_interface.h>
#include <sup/sequencer/workspace.h>

#include <string>

const std::string EXPR_STRING_ATTR_NAME = "expression";

namespace sup
{
namespace sequencer
{

const std::string Math::Type = "Math";
static bool _math_initialised_flag = RegisterGlobalInstruction<Math>();

Math::Math() : Instruction(Math::Type)
{
  AddAttributeDefinition(EXPR_STRING_ATTR_NAME, sup::dto::StringType).SetMandatory();
}

Math::~Math() = default;

ExecutionStatus Math::ExecuteSingleImpl(UserInterface& ui, Workspace& ws)
{
  auto expression = GetAttributeValue<std::string>(EXPR_STRING_ATTR_NAME);
  sup::math::ExpressionContext expr_ctx(expression);

  sup::dto::AnyValue in_value;
  bool read_type = true;
  sup::dto::AnyType in_value_type;
  for (auto variable : expr_ctx)
  {
    std::string var_name = variable.first;

    // read the variable from the workspace
    if (!ws.HasVariable(var_name) || !ws.GetValue(var_name, in_value))
    {
      return ExecutionStatus::FAILURE;
    }

    if (read_type)
    {
      in_value_type = in_value.GetType();
      read_type = false;
    }
    else
    {
      if (in_value_type != in_value.GetType())
      {
        std::string error_message =
            InstructionErrorProlog(*this) + "The expression variables cannot have different types.";
        ui.LogError(error_message);
        return ExecutionStatus::FAILURE;
      }
    }

    // Only numeric and array types are accepted
    if (in_value_type.GetTypeCode() == sup::dto::TypeCode::Empty
        || in_value_type.GetTypeCode() == sup::dto::TypeCode::Bool
        || in_value_type.GetTypeCode() == sup::dto::TypeCode::Char8
        || in_value_type.GetTypeCode() == sup::dto::TypeCode::String
        || in_value_type.GetTypeCode() == sup::dto::TypeCode::Struct)
    {
      std::string error_message =
          InstructionErrorProlog(*this) + "Only arrays and numeric types are accepted.";
      ui.LogError(error_message);
      return ExecutionStatus::FAILURE;
    }
    expr_ctx.ConvertVariable(var_name, in_value);
  }

  sup::dto::AnyValue return_value(in_value_type);

  math::ProcessVariableMap output = expr_ctx.EvaluateExpression();

  if (output.count("FAILURE") > 0)
  {
    output.at("FAILURE");
    std::string error_message = InstructionErrorProlog(*this)
                                + "An assignment variable needs to be defined in the expression.";
    ui.LogError(error_message);
    return ExecutionStatus::FAILURE;
  }

  for (auto vec : output)
  {
    sup::dto::AnyValue temp;
    if (sup::dto::IsArrayValue(return_value))
    {
      for (size_t i = 0; i < return_value.NumberOfElements(); ++i)
      {
        temp = vec.second[i];
        if (!sup::dto::TryConvert(return_value[i], temp))
        {
          return ExecutionStatus::FAILURE;
        }
      }
    }
    else
    {
      temp = vec.second[0];
      if (!sup::dto::TryConvert(return_value, temp))
      {
        return ExecutionStatus::FAILURE;
      }
    }

    if (!ws.HasVariable(vec.first) || !ws.SetValue(vec.first, return_value))
    {
      return ExecutionStatus::FAILURE;
    }
  }

  return ExecutionStatus::SUCCESS;
}

}  // namespace sequencer

}  // namespace sup
