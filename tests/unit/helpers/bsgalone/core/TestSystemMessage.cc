
#include "TestSystemMessage.hh"
#include "SerializationUtils.hh"

namespace test {

TestSystemMessage::TestSystemMessage(const bsgo::Uuid systemDbId)
  // The type of message does not matter, it is just defined because it is
  // required to pick one to instantiate an `AbstractSystemMessage`.
  : bsgalone::core::AbstractSystemMessage(bsgalone::core::MessageType::DOCK, systemDbId)
{}

auto TestSystemMessage::serialize(std::ostream &out) const -> std::ostream &
{
  ::core::serialize(out, m_messageType);
  ::core::serialize(out, m_systemDbId);

  return out;
}

bool TestSystemMessage::deserialize(std::istream &in)
{
  bool ok{true};
  ok &= ::core::deserialize(in, m_messageType);
  ok &= ::core::deserialize(in, m_systemDbId);

  return ok;
}

auto TestSystemMessage::clone() const -> std::unique_ptr<IMessage>
{
  return std::make_unique<TestSystemMessage>(m_systemDbId);
}

} // namespace test
