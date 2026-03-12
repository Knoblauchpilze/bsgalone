
#include "ComputerListMessage.hh"
#include "SerializationUtils.hh"

namespace bsgalone::core {

ComputerListMessage::ComputerListMessage()
  : NetworkMessage(MessageType::COMPUTER_LIST)
{}

ComputerListMessage::ComputerListMessage(const std::vector<ComputerData> &computersData)
  : NetworkMessage(MessageType::COMPUTER_LIST)
  , m_computersData(computersData)
{}

auto ComputerListMessage::getComputersData() const -> const std::vector<ComputerData> &
{
  return m_computersData;
}

auto ComputerListMessage::clone() const -> IMessagePtr
{
  auto clone = std::make_unique<ComputerListMessage>(m_computersData);
  clone->copyClientIdIfDefined(*this);

  return clone;
}

auto operator<<(std::ostream &out, const ComputerListMessage &message) -> std::ostream &
{
  ::core::serialize(out, message.m_type);
  ::core::serialize(out, message.m_clientId);
  ::core::serialize(out, message.m_computersData);

  return out;
}

auto operator>>(std::istream &in, ComputerListMessage &message) -> std::istream &
{
  ::core::deserialize(in, message.m_type);
  ::core::deserialize(in, message.m_clientId);
  ::core::deserialize(in, message.m_computersData);

  return in;
}

} // namespace bsgalone::core
