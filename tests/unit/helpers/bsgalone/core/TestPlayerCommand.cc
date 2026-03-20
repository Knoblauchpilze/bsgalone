
#include "TestPlayerCommand.hh"
#include "SerializationUtils.hh"

namespace test {

TestPlayerCommand::TestPlayerCommand(const bsgalone::core::Uuid playerDbId,
                                     const bsgalone::core::Uuid systemDbId)
  // The type of message does not matter, it is just defined because it is
  // required to pick one to instantiate an `AbstractPlayerCommand`.
  : AbstractPlayerCommand(bsgalone::core::MessageType::DOCK, playerDbId, systemDbId)
{}

auto TestPlayerCommand::clone() const -> bsgalone::core::IMessagePtr
{
  return std::make_unique<TestPlayerCommand>(m_playerDbId, m_systemDbId);
}

} // namespace test
