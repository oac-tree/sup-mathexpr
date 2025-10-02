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
 * Copyright (c) : 2010-2025 ITER Organization,
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

#ifndef SUP_MATHEXPR_I_VARIABLE_STORE_H_
#define SUP_MATHEXPR_I_VARIABLE_STORE_H_

#include <cstdint>
#include <string>
#include <vector>

namespace sup
{
namespace mathexpr
{

/**
 * @brief Interface to implement to be able to inject variable access into ExpressionContext.
 */
class IVariableStore
{
public:
  enum class VarType : uint32_t
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

}  // namespace mathexpr

}  // namespace sup

#endif  // SUP_MATHEXPR_I_VARIABLE_STORE_H_
