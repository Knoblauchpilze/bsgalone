
#include "TestPlayerMessage.hh"
#include "SerializationUtils.hh"

namespace test {

TestPlayerMessage::TestPlayerMessage(const bsgalone::core::Uuid playerDbId,
                                     const bsgalone::core::Uuid systemDbId)
  // The type of message does not matter, it is just defined because it is
  // required to pick one to instantiate an `AbstractPlayerMessage`.
  : AbstractPlayerMessage(bsgalone::core::MessageType::DOCK, playerDbId, systemDbId)
{}

auto TestPlayerMessage::serialize(std::ostream &out) const -> std::ostream &
{
  ::core::serialize(out, m_messageType);
  ::core::serialize(out, m_playerDbId);
  ::core::serialize(out, m_systemDbId);

  return out;
}

bool TestPlayerMessage::deserialize(std::istream &in)
{
  bool ok{true};
  ok &= ::core::deserialize(in, m_messageType);
  ok &= ::core::deserialize(in, m_playerDbId);
  ok &= ::core::deserialize(in, m_systemDbId);

  return ok;
}

auto TestPlayerMessage::clone() const -> std::unique_ptr<IMessage>
{
  return std::make_unique<TestPlayerMessage>(m_playerDbId, m_systemDbId);
}

} // namespace test
