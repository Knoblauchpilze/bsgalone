
#include "PlayerComputerListMessage.hh"
#include "SerializationUtils.hh"

namespace bsgalone::core {

PlayerComputerListMessage::PlayerComputerListMessage()
  : NetworkMessage(MessageType::PLAYER_COMPUTER_LIST)
{}

PlayerComputerListMessage::PlayerComputerListMessage(
  const std::vector<PlayerComputerData> &computersData)
  : NetworkMessage(MessageType::PLAYER_COMPUTER_LIST)
  , m_computersData(computersData)
{}

auto PlayerComputerListMessage::getComputersData() const -> const std::vector<PlayerComputerData> &
{
  return m_computersData;
}

auto PlayerComputerListMessage::clone() const -> IMessagePtr
{
  auto clone = std::make_unique<PlayerComputerListMessage>(m_computersData);
  clone->copyClientIdIfDefined(*this);

  return clone;
}

auto operator<<(std::ostream &out, const PlayerComputerListMessage &message) -> std::ostream &
{
  ::core::serialize(out, message.m_type);
  ::core::serialize(out, message.m_clientId);
  ::core::serialize(out, message.m_computersData);

  return out;
}

auto operator>>(std::istream &in, PlayerComputerListMessage &message) -> std::istream &
{
  ::core::deserialize(in, message.m_type);
  ::core::deserialize(in, message.m_clientId);
  ::core::deserialize(in, message.m_computersData);

  return in;
}

} // namespace bsgalone::core
