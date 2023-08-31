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

#include <iostream>
#include <utility>

namespace sup
{
namespace math
{

VariableHandler::VariableHandler(sequencer::Workspace* ws, ProcessVariableMap* vars) : m_data(vars)
{
  for (auto var : *vars)
  {
    this->GetWorkspaceValue(ws, var.first);
  }
}

IVariableStore::VarType VariableHandler::GetVariableType(const std::string& varname) const
{
  if (m_data->count(varname) > 0)
  {
    if (dto::IsScalarValue(m_data->at(varname)))
    {
      return kScalar;
    }
    if (sup::dto::IsArrayValue(m_data->at(varname)))
    {
      return kVector;
    }
  }
  return kUnknown;
}

bool VariableHandler::GetScalar(const std::string& varname, double& val) const
{
  val = m_data->at(varname).As<double>();
  return true;
}

bool VariableHandler::SetScalar(const std::string& varname, const double& val)
{
  dto::AnyValue temp(m_data->at(varname).GetType());
  temp = val;

  if (!sup::dto::TryConvert(m_data->at(varname), temp))
  {
    return false;
  }
  return true;
}

bool VariableHandler::GetVector(const std::string& varname, std::vector<double>& val) const
{
  dto::AnyValue temp = m_data->at(varname);
  val.resize(temp.NumberOfElements());
  for (size_t i = 0; i < temp.NumberOfElements(); ++i)
  {
    val[i] = temp[i].As<double>();
  }
  return true;
}

bool VariableHandler::SetVector(const std::string& varname, const std::vector<double>& val)
{
  dto::AnyValue temp;
  for (size_t i = 0; i < m_data->at(varname).NumberOfElements(); ++i)
  {
    temp = val[i];
    if (!sup::dto::TryConvert(m_data->at(varname)[i], temp))
    {
      return false;
    }
  }
  return true;
}

bool VariableHandler::GetWorkspaceValue(sequencer::Workspace* ws, std::string varname)
{
  dto::AnyValue readvalue;
  if (!ws->HasVariable(varname) || !ws->GetValue(varname, readvalue))
  {
    return false;
  }
  if (readvalue.GetTypeCode() == sup::dto::TypeCode::Empty
      || readvalue.GetTypeCode() == sup::dto::TypeCode::String
      || readvalue.GetTypeCode() == sup::dto::TypeCode::Struct)
  {
    return false;
  }

  if (m_data->at(varname) == 0)
  {
    return false;
  }

  m_data->at(varname) = readvalue;

  return true;
}

}  // namespace math

}  // namespace sup
