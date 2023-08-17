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

#include "test_user_interface.h"
#include "unit_test_helper.h"

#include <sup/sequencer/instruction_registry.h>
#include <sup/sequencer/sequence_parser.h>

#include <gtest/gtest.h>

using namespace sup::sequencer;

class MathTest : public ::testing::Test
{
protected:
  MathTest() = default;
  virtual ~MathTest() = default;
};

// TEST_F(MathTest, Success)
// {
//   const std::string body{R"(
//     <ParallelSequence>
//         <WaitForCondition varNames="live" timeout="1.0">
//             <Equals lhs="live" rhs="one"/>
//         </WaitForCondition>
//         <Sequence>
//             <Wait timeout="0.1"/>
//             <Copy input="one" output="live"/>
//         </Sequence>
//     </ParallelSequence>
//     <Workspace>
//         <Local name="live" type='{"type":"uint64"}' value='0' />
//         <Local name="one" type='{"type":"uint64"}' value='1' />
//     </Workspace>
// )"};

//   test::NullUserInterface ui;
//   auto proc = ParseProcedureString(test::CreateProcedureString(body));
//   EXPECT_TRUE(test::TryAndExecute(proc, ui));
// }

// TEST_F(MathTest, Setup)
// {
//   {
//     // No child
//     const std::string body{R"(
//       <WaitForCondition varNames="live" timeout="0.1"/>
//       <Workspace>
//           <Local name="live" type='{"type":"uint64"}' value='0' />
//       </Workspace>)"};

//     auto proc = ParseProcedureString(test::CreateProcedureString(body));
//     EXPECT_THROW(proc->Setup(), InstructionSetupException);
//   }
//   {
//     // Missing attributes
//     auto instr = GlobalInstructionRegistry().Create("WaitForCondition");
//     auto wait = GlobalInstructionRegistry().Create("Wait");
//     ASSERT_TRUE(instr);
//     ASSERT_TRUE(wait);
//     ASSERT_TRUE(instr->InsertInstruction(std::move(wait), 0));
//     Procedure proc;
//     EXPECT_THROW(instr->Setup(proc), InstructionSetupException);
//     EXPECT_TRUE(instr->AddAttribute("timeout", "1.0"));
//     EXPECT_THROW(instr->Setup(proc), InstructionSetupException);
//     EXPECT_TRUE(instr->AddAttribute("varNames", "does_not_matter"));
//     EXPECT_NO_THROW(instr->Setup(proc));
//   }
// }

TEST_F(MathTest, Success)
{
  const std::string body{
    R"(
    <Sequence>
        <Math expression="x+y*y-2" outVar="z" />
        <Equals lhs="z" rhs="a"/>
    </Sequence>
    <Workspace>
        <Local name="x" type='{"type":"uint64"}' value='5' />
        <Local name="y" type='{"type":"uint64"}' value='3' />
        <Local name="z" type='{"type":"uint64"}' />
        <Local name="a" type='{"type":"uint64"}' value='12' />
    </Workspace>
)"};

  test::NullUserInterface ui;
  auto proc = ParseProcedureString(test::CreateProcedureString(body));
  EXPECT_TRUE(test::TryAndExecute(proc, ui, ExecutionStatus::SUCCESS));
}
