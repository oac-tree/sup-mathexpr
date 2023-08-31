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

#ifndef SUP_SEQUENCER_PLUGIN_MATH_VARIABLE_HANDLER_H_
#define SUP_SEQUENCER_PLUGIN_MATH_VARIABLE_HANDLER_H_

#include "ivariablestore.h"

#include <sup/dto/anytype.h>
#include <sup/dto/anyvalue.h>
#include <sup/sequencer/workspace.h>

#include <map>
#include <string>
#include <vector>

namespace sup
{
namespace math
{

using ProcessVariableMap = std::map<const std::string, sup::dto::AnyValue>;
  
class VariableHandler : public IVariableStore
{
public:
  VariableHandler(sequencer::Workspace* ws, ProcessVariableMap* vars);
  ~VariableHandler() override = default;
  VarType GetVariableType(const std::string& varname) const override;
  bool GetScalar(const std::string& varname, double& val) const override;
  bool SetScalar(const std::string& varname, const double& val) override;
  bool GetVector(const std::string& varname, std::vector<double>& val) const override;
  bool SetVector(const std::string& varname, const std::vector<double>& val) override;

private:
  bool GetWorkspaceValue(sequencer::Workspace* ws, std::string varname);

  ProcessVariableMap* m_data;
};

}  // namespace math

}  // namespace sup

#endif  // SUP_SEQUENCER_PLUGIN_MATH_VARIABLE_HANDLER_H_
