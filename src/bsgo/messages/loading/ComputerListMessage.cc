
#include "ComputerListMessage.hh"
#include "DataSerialization.hh"
#include "SerializationUtils.hh"

namespace bsgo {

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

auto ComputerListMessage::serialize(std::ostream &out) const -> std::ostream &
{
  core::serialize(out, m_messageType);
  core::serialize(out, m_clientId);

  core::serialize(out, m_computersData.size());
  for (const auto &computerData : m_computersData)
  {
    serializeComputerData(out, computerData);
  }

  return out;
}

bool ComputerListMessage::deserialize(std::istream &in)
{
  bool ok{true};
  ok &= core::deserialize(in, m_messageType);
  ok &= core::deserialize(in, m_clientId);

  std::size_t count;
  ok &= core::deserialize(in, count);

  m_computersData.clear();

  for (std::size_t id = 0u; id < count; ++id)
  {
    ComputerData data;

    ok &= deserializeComputerData(in, data);

    m_computersData.emplace_back(data);
  }

  return ok;
}

auto ComputerListMessage::clone() const -> IMessagePtr
{
  auto clone = std::make_unique<ComputerListMessage>(m_computersData);
  clone->copyClientIdIfDefined(*this);

  return clone;
}

} // namespace bsgo
