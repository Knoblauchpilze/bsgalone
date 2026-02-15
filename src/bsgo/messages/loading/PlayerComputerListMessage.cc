
#include "PlayerComputerListMessage.hh"
#include "SerializationUtils.hh"

namespace bsgo {

PlayerComputerListMessage::PlayerComputerListMessage()
  : NetworkMessage(bsgalone::core::MessageType::PLAYER_COMPUTER_LIST)
{}

PlayerComputerListMessage::PlayerComputerListMessage(
  const std::vector<PlayerComputerData> &computersData)
  : NetworkMessage(bsgalone::core::MessageType::PLAYER_COMPUTER_LIST)
  , m_computersData(computersData)
{}

auto PlayerComputerListMessage::getComputersData() const -> const std::vector<PlayerComputerData> &
{
  return m_computersData;
}

auto PlayerComputerListMessage::serialize(std::ostream &out) const -> std::ostream &
{
  core::serialize(out, m_messageType);
  core::serialize(out, m_clientId);

  core::serialize(out, m_computersData);

  return out;
}

bool PlayerComputerListMessage::deserialize(std::istream &in)
{
  bool ok{true};
  ok &= core::deserialize(in, m_messageType);
  ok &= core::deserialize(in, m_clientId);

  ok &= core::deserialize(in, m_computersData);

  return ok;
}

auto PlayerComputerListMessage::clone() const -> bsgalone::core::IMessagePtr
{
  auto clone = std::make_unique<PlayerComputerListMessage>(m_computersData);
  clone->copyClientIdIfDefined(*this);

  return clone;
}

} // namespace bsgo
