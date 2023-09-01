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

#ifndef SUP_SEQUENCER_PLUGIN_MATH_EXPRESSION_CONTEXT_H_
#define SUP_SEQUENCER_PLUGIN_MATH_EXPRESSION_CONTEXT_H_

#include <sup/dto/anyvalue.h>
#include "ivariablestore.h"

#include <sequencer/exprtk/exprtk.hpp>

#include <vector>
#include <map>
#include <string>

namespace sup
{
namespace math
{

/**
 * @brief Processes Anyvalue variables for mathematical expression evaluation
 *
 */

using ProcessVariableMap = std::map<std::string, std::vector<double>>;

class ExpressionContext
{
public:
  ExpressionContext(const std::string& expression, IVariableStore* varhandler);

  bool EvaluateExpression();

private:
  bool GetVariables(std::vector<std::string> list_vars);
  bool SetVariable(std::string varname);

  std::string m_raw_expression;
  IVariableStore* m_variable_handler;
  ProcessVariableMap m_data;
};

}  // namespace math

}  // namespace sup

#endif  // SUP_SEQUENCER_PLUGIN_MATH_EXPRESSION_CONTEXT_H_
