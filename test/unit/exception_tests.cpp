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
 * Copyright (c) : 2010-2026 ITER Organization,
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

#include <sup/mathexpr/exceptions.h>

#include <gtest/gtest.h>

using namespace sup::mathexpr;

const std::string MESSAGE_1 = "message 1";

class ExceptionTest : public ::testing::Test
{
protected:
  ExceptionTest() = default;
  virtual ~ExceptionTest() = default;
};

TEST_F(ExceptionTest, ExpressionEvaluateException)
{
  ExpressionEvaluateException exception{MESSAGE_1};
  EXPECT_EQ(exception.what(), MESSAGE_1);
}
