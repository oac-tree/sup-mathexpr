/******************************************************************************
 * $HeadURL: $
 * $Id: $
 *
 * Project       : SUP - MathExpr
 *
 * Description   : Mathematical expression evaluator for SUP
 *
 * Author        : Walter Van Herck (IO)
 *
 * Copyright (c) : 2010-2025 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 ******************************************************************************/

#ifndef SUP_MATHEXPR_EXCEPTIONS_H_
#define SUP_MATHEXPR_EXCEPTIONS_H_

#include <exception>
#include <string>

namespace sup
{
namespace mathexpr
{

  /**
 * @brief Base Exception class with message.
 */
class MessageException : public std::exception
{
public:
  explicit MessageException(std::string message);
  ~MessageException() override = default;
  const char* what() const noexcept override;
protected:
  MessageException(const MessageException& other) = default;
  MessageException(MessageException&& other) = default;
  MessageException& operator=(const MessageException& other) & = default;
  MessageException& operator=(MessageException&& other) & = default;
private:
  std::string m_message;
};

/**
 * @brief Exception thrown when evaluating an expression fails.
 */
class ExpressionEvaluateException : public MessageException
{
public:
  explicit ExpressionEvaluateException(const std::string& message);
  ~ExpressionEvaluateException() override = default;
  ExpressionEvaluateException(const ExpressionEvaluateException& other) = default;
  ExpressionEvaluateException(ExpressionEvaluateException&& other) = default;
  ExpressionEvaluateException& operator=(const ExpressionEvaluateException& other) & = default;
  ExpressionEvaluateException& operator=(ExpressionEvaluateException&& other) & = default;
};

}  // namespace mathexpr

}  // namespace sup

#endif  // SUP_MATHEXPR_EXCEPTIONS_H_
