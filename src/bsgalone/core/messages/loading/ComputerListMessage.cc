
#include "ComputerListMessage.hh"
#include "SerializationUtils.hh"

namespace bsgo {

ComputerListMessage::ComputerListMessage()
  : NetworkMessage(bsgalone::core::MessageType::COMPUTER_LIST)
{}

ComputerListMessage::ComputerListMessage(const std::vector<ComputerData> &computersData)
  : NetworkMessage(bsgalone::core::MessageType::COMPUTER_LIST)
  , m_computersData(computersData)
{}

auto ComputerListMessage::getComputersData() const -> const std::vector<ComputerData> &
{
  return m_computersData;
}

auto ComputerListMessage::serialize(std::ostream &out) const -> std::ostream &
{
  core::serialize(out, m_messageType);
  core::serialize(out, m_clientId);

  core::serialize(out, m_computersData);

  return out;
}

bool ComputerListMessage::deserialize(std::istream &in)
{
  bool ok{true};
  ok &= core::deserialize(in, m_messageType);
  ok &= core::deserialize(in, m_clientId);

  ok &= core::deserialize(in, m_computersData);

  return ok;
}

auto ComputerListMessage::clone() const -> bsgalone::core::IMessagePtr
{
  auto clone = std::make_unique<ComputerListMessage>(m_computersData);
  clone->copyClientIdIfDefined(*this);

  return clone;
}

} // namespace bsgo
