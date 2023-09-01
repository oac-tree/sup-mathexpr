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

  sup::math::VariableHandler handler(&ws);

  try
  {
    sup::math::ExpressionContext expr_ctx(expression, &handler);
    if (!expr_ctx.EvaluateExpression())
    {
      std::string error_message =
        InstructionErrorProlog(*this) + "Failure evaluating the expression.";
      ui.LogError(error_message);
      return ExecutionStatus::FAILURE;
    }
  }
  catch (const std::exception& ex)
  {
    std::string error_message =
        InstructionErrorProlog(*this) + ex.what();
    ui.LogError(error_message);
    return ExecutionStatus::FAILURE;
  }

  return ExecutionStatus::SUCCESS;
}

}  // namespace sequencer

}  // namespace sup
