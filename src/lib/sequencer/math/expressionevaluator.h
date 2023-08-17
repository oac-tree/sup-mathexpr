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

#ifndef SUP_SEQUENCER_PLUGIN_MATH_EXPRESSION_EVALUATOR_H_
#define SUP_SEQUENCER_PLUGIN_MATH_EXPRESSION_EVALUATOR_H_

#include "../exprtk/exprtk.hpp"


#include <string>
#include <vector>
#include <map>

namespace sup
{
namespace math
{

using ProcessVariableMap = std::map<std::string, float>;

class ExpressionEvaluator
{
public:
  ExpressionEvaluator(const std::string& expression, ProcessVariableMap vars);
  void Setup();
  float Evaluate();

private:
  std::string m_s_expression;
  ProcessVariableMap m_variablemap;

  using expression_t = exprtk::expression<float>;
  using symbol_table_t = exprtk::symbol_table<float>;
  using parser_t = exprtk::parser<float>;

  symbol_table_t m_symbol_table;
  expression_t m_expression;
  parser_t m_parser;

};

}  // namespace math

}  // namespace sup

#endif  // SUP_SEQUENCER_PLUGIN_MATH_EXPRESSION_EVALUATOR_H_
