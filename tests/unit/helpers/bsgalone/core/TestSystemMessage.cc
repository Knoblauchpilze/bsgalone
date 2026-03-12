
#include "TestSystemMessage.hh"
#include "SerializationUtils.hh"

namespace test {

TestSystemMessage::TestSystemMessage(const bsgalone::core::Uuid systemDbId)
  // The type of message does not matter, it is just defined because it is
  // required to pick one to instantiate an `AbstractSystemMessage`.
  : AbstractSystemMessage(bsgalone::core::MessageType::DOCK, systemDbId)
{}

auto TestSystemMessage::clone() const -> bsgalone::core::IMessagePtr
{
  return std::make_unique<TestSystemMessage>(m_systemDbId);
}

} // namespace test
