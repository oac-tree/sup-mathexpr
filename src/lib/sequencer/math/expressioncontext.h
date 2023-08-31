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

using ProcessVariableMap = std::map<const std::string, sup::dto::AnyValue>;

class ExpressionContext
{
public:
  ExpressionContext(const std::string& expression);
  // ExpressionContext(const std::string& expression);

  ProcessVariableMap* GetVariableList();

  void SetVariableHandler(IVariableStore* var_handler);

  ProcessVariableMap EvaluateExpression();

private:
  bool ReadVariables();
  bool WriteVariable(std::string varname);

  std::string m_raw_expression;
  ProcessVariableMap m_data;
  IVariableStore* m_variable_handler;
  std::vector<std::string> m_list_vars;
  std::map<std::string, std::vector<double>> m_process_data;
};

}  // namespace math

}  // namespace sup

#endif  // SUP_SEQUENCER_PLUGIN_MATH_EXPRESSION_CONTEXT_H_
