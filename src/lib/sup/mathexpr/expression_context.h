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
 * @brief Class that handles mathematical expression evaluation.
 */
class ExpressionContext
{
public:
  explicit ExpressionContext(IVariableStore& varhandler);

/**
 * @brief Evaluate the given expression.
 *
 * @param expression Expression to evaluate
 *
 * @return If the expression contains no assigments, the double representation of the expression
 * is returned. In the presence of assignments, the return value is 1.0.
 *
 * @throws ExpressionEvaluateException when variables are missing or the expression could not be
 * parsed correctly.
 */
  double EvaluateExpression(const std::string& expression);

private:
  using ProcessVariableMap = std::map<std::string, std::vector<double>>;
  bool CollectVariables(ProcessVariableMap& data_map, const std::string& expression);
  bool SetVariable(const ProcessVariableMap& data_map, const std::string& varname);

  IVariableStore& m_variable_handler;
};

}  // namespace mathexpr

}  // namespace sup

#endif  // SUP_MATHEXPR_EXPRESSION_CONTEXT_H_
