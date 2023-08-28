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

#include <vector>
#include <map>
#include <string>

namespace sup
{
namespace math
{

/**
 * @brief Processes Anyvalue variables for mathmatical expression evaluation
 *
 */
using ProcessVariableMap = std::map<const std::string, std::vector<double>>;

class ExpressionContext
{
public:
  ExpressionContext(const std::string& expression);

  ProcessVariableMap::iterator begin();

  ProcessVariableMap::iterator end();

  void ConvertVariable(const std::string& var, dto::AnyValue& val);

  ProcessVariableMap EvaluateExpression();

private:
  void ConfigureExpression();

  std::string m_in_expression;
  std::vector<std::string> m_list_vars;
  ProcessVariableMap m_proc_vars;
};

}  // namespace math

}  // namespace sup

#endif  // SUP_SEQUENCER_PLUGIN_MATH_EXPRESSION_CONTEXT_H_
