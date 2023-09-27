/******************************************************************************
 * $HeadURL: $
 * $Id: $
 *
 * Project       : SUP - MathExpr
 *
 * Description   : Mathematical expression evaluator for SUP
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

#include "exceptions.h"

namespace sup
{
namespace mathexpr
{

MessageException::MessageException(std::string message)
  : m_message{std::move(message)}
{}

const char* MessageException::what() const noexcept
{
  return m_message.c_str();
}

ExpressionEvaluateException::ExpressionEvaluateException(const std::string& message)
  : MessageException{message}
{}

}  // namespace mathexpr

}  // namespace sup
