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
#include "variable_handler.h"

#include <iostream>
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

  math::VariableHandler handler(&ws, expr_ctx.GetVariableList());

  expr_ctx.SetVariableHandler(&handler);

  math::ProcessVariableMap output = expr_ctx.EvaluateExpression();

  if (output.count("FAILURE") > 0)
  {
    output.at("FAILURE");
    std::string error_message = InstructionErrorProlog(*this)
                                + "An assignment variable needs to be defined in the expression.";
    ui.LogError(error_message);
    return ExecutionStatus::FAILURE;
  }

  for (auto assignment : output)
  {
    if (!ws.HasVariable(assignment.first) || !ws.SetValue(assignment.first, assignment.second))
    {
      return ExecutionStatus::FAILURE;
    }
  }
  return ExecutionStatus::SUCCESS;
}

}  // namespace sequencer

}  // namespace sup
