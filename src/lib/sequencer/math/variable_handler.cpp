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

#include "variable_handler.h"

#include "sequencer/math/ivariablestore.h"

#include <sup/dto/anytype.h>
#include <sup/dto/anyvalue.h>
#include <sup/dto/anyvalue_helper.h>
#include <sup/sequencer/workspace.h>

namespace sup
{
namespace math
{

VariableHandler::VariableHandler(sequencer::Workspace* ws) : m_ws(ws)
{}

IVariableStore::VarType VariableHandler::GetVariableType(const std::string& varname) const
{
  dto::AnyValue readvalue;
  if (!m_ws->HasVariable(varname) || !m_ws->GetValue(varname, readvalue))
  {
    return kUnknown;
  }
  if (dto::IsScalarValue(readvalue))
  {
    return kScalar;
  }
  if (sup::dto::IsArrayValue(readvalue))
  {
    return kVector;
  }
  return kUnknown;
}

bool VariableHandler::GetScalar(const std::string& varname, double& val) const
{
  dto::AnyValue readvalue;
  m_ws->GetValue(varname, readvalue);
  val = readvalue.As<double>();
  return true;
}

bool VariableHandler::SetScalar(const std::string& varname, const double& val)
{
  dto::AnyValue writevalue;
  m_ws->GetValue(varname, writevalue);
  dto::AnyValue temp = val;
  if (!sup::dto::TryConvert(writevalue, temp))
  {
    return false;
  }
  if (!m_ws->SetValue(varname, writevalue))
  {
    return false;
  }
  return true;
}

bool VariableHandler::GetVector(const std::string& varname, std::vector<double>& val) const
{
  dto::AnyValue readvalue;
  m_ws->GetValue(varname, readvalue);

  val.resize(readvalue.NumberOfElements());
  for (size_t i = 0; i < readvalue.NumberOfElements(); ++i)
  {
    val[i] = readvalue[i].As<double>();
  }
  return true;
}

bool VariableHandler::SetVector(const std::string& varname, const std::vector<double>& val)
{
  dto::AnyValue writevalue;
  dto::AnyValue temp;
  m_ws->GetValue(varname, writevalue);
  for (size_t i = 0; i < writevalue.NumberOfElements(); ++i)
  {
    temp = val[i];
    if (!sup::dto::TryConvert(writevalue[i], temp))
    {
      return false;
    }
  }
  if (!m_ws->SetValue(varname, writevalue))
  {
    return false;
  }
  return true;
}

}  // namespace math

}  // namespace sup
