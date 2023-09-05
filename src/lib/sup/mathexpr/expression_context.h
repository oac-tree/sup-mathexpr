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

#ifndef SUP_MATHEXPR_EXPRESSION_CONTEXT_H_
#define SUP_MATHEXPR_EXPRESSION_CONTEXT_H_

#include <sup/mathexpr/i_variable_store.h>

#include <map>
#include <string>
#include <vector>

namespace sup
{
namespace mathexpr
{

/**
 * @brief Processes Anyvalue variables for mathematical expression evaluation
 */
class ExpressionContext
{
public:
  ExpressionContext(const std::string& expression, IVariableStore& varhandler);

  bool EvaluateExpression();

private:
  using ProcessVariableMap = std::map<std::string, std::vector<double>>;

  bool GetVariables(const std::vector<std::string>& list_vars);
  bool SetVariable(const std::string& varname);

  std::string m_raw_expression;
  IVariableStore& m_variable_handler;
  ProcessVariableMap m_data;
};

}  // namespace mathexpr

}  // namespace sup

#endif  // SUP_MATHEXPR_EXPRESSION_CONTEXT_H_
