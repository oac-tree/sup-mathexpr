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

#include <string>
#include <vector>
#include <map>

#include <sup/dto/anyvalue.h>
#include "expressionevaluator.h"

namespace sup
{
namespace math
{

/**
 * @brief Processes Anyvalue variables for mathmatical expression evaluation
 *
 */
using InVariableMap = std::map<std::string, sup::dto::AnyValue>;
using ProcessVariableMap = std::map<std::string, float>;

class ExpressionContext
{
public:
  ExpressionContext(const std::string& expression);

  std::vector<std::string> CollectVariables();

  bool ConvertVariables(InVariableMap in_variables);

  sup::dto::AnyValue EvaluateExpression();

private:
  void ConfigureExpression();

  std::string m_expression;
  ProcessVariableMap m_proc_vars;

  /* std::map<std::string, float> variableMap_; */
  /* expression_t expression_; */
  /* exprtk::parser<float> parser_; */
};

}  // namespace math

}  // namespace sup

#endif // SUP_SEQUENCER_PLUGIN_MATH_EXPRESSION_CONTEXT_H_
