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

#ifndef SUP_SEQUENCER_PLUGIN_MATH_I_VARIABLE_STORE_H_
#define SUP_SEQUENCER_PLUGIN_MATH_I_VARIABLE_STORE_H_

#include <string>
#include <vector>
namespace sup
{
namespace math
{

class IVariableStore
{
public:
  enum VarType
  {
    kScalar = 0,
    kVector,
    kUnknown
  };
  virtual ~IVariableStore() = default;

  virtual VarType GetVariableType(const std::string& varname) const = 0;

  virtual bool GetScalar(const std::string& varname, double& val) const = 0;
  virtual bool SetScalar(const std::string& varname, const double& val) = 0;

  virtual bool GetVector(const std::string& varname, std::vector<double>& val) const = 0;
  virtual bool SetVector(const std::string& varname, const std::vector<double>& val) = 0;
};
}  // namespace math
}  // namespace sup
#endif  // SUP_SEQUENCER_PLUGIN_MATH_I_VARIABLE_STORE_H_
