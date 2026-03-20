
#include "TestPlayerMessage.hh"
#include "SerializationUtils.hh"

namespace test {

TestPlayerMessage::TestPlayerMessage(const bsgalone::core::Uuid playerDbId)
  // The type of message does not matter, it is just defined because it is
  // required to pick one to instantiate an `AbstractPlayerMessage`.
  : AbstractPlayerMessage(bsgalone::core::MessageType::EQUIP, playerDbId)
{}

auto TestPlayerMessage::clone() const -> bsgalone::core::IMessagePtr
{
  return std::make_unique<TestPlayerMessage>(m_playerDbId);
}

} // namespace test
