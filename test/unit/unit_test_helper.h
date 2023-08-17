/******************************************************************************
* $HeadURL: $
* $Id: $
*
* Project       : Supervision and Automation - Sequencer
*
* Description   : SUP sequencer control plugin
*
* Author        : Walter Van Herck (IO)
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

#ifndef SUP_SEQUENCER_PLUGIN_CONTROL_UNIT_TEST_HELPER_H_
#define SUP_SEQUENCER_PLUGIN_CONTROL_UNIT_TEST_HELPER_H_

#include <sup/sequencer/instruction.h>
#include <sup/sequencer/procedure.h>
#include <sup/sequencer/user_interface.h>

#include <thread>
#include <utility>
#include <vector>

namespace sup {

namespace sequencer {

namespace test {

static inline bool TryAndExecute(std::unique_ptr<Procedure>& proc, UserInterface& ui,
                                 const ExecutionStatus& expect = ExecutionStatus::SUCCESS);

static inline bool TryAndExecuteNoReset(std::unique_ptr<Procedure>& proc, UserInterface& ui,
                                        const ExecutionStatus& expect)
{
  if (!proc)
  {
    return false;
  }
  proc->Setup();
  ExecutionStatus exec = ExecutionStatus::FAILURE;
  do
  {
    if (exec == ExecutionStatus::RUNNING)
    {
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    proc->ExecuteSingle(ui);
    exec = proc->GetStatus();
  } while ((ExecutionStatus::SUCCESS != exec)
           && (ExecutionStatus::FAILURE != exec));
  return exec == expect;
}

static inline bool TryAndExecute(std::unique_ptr<Procedure>& proc,
                                 UserInterface& ui,
                                 const ExecutionStatus& expect)
{
  bool status = TryAndExecuteNoReset(proc, ui, expect);
  if (proc)
  {
    proc->Reset();
  }
  return status;
}

/**
 * Creates a string representing a valid XML of sequencer procedure by enclosing user provided body
 * between appropriate header and footer.
 */
std::string CreateProcedureString(const std::string& body);

} // namespace test

} // namespace sequencer

} // namespace sup

#endif // SUP_SEQUENCER_PLUGIN_CONTROL_UNIT_TEST_HELPER_H_
