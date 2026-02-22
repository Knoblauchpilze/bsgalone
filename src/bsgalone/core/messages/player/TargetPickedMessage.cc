

#include "TargetPickedMessage.hh"
#include "SerializationUtils.hh"

namespace bsgalone::core {

TargetPickedMessage::TargetPickedMessage()
  : AbstractPlayerMessage(MessageType::TARGET_PICKED, bsgo::Uuid{0}, bsgo::Uuid{0})
{}

TargetPickedMessage::TargetPickedMessage(const bsgo::Uuid playerDbId,
                                         const bsgo::Uuid systemDbId,
                                         Target data,
                                         const Eigen::Vector3f &position)
  : AbstractPlayerMessage(MessageType::TARGET_PICKED, playerDbId, systemDbId)
  , m_data(std::move(data))
  , m_position(position)
{}

auto TargetPickedMessage::getTargetData() const -> Target
{
  return m_data;
}

auto TargetPickedMessage::getPosition() const -> Eigen::Vector3f
{
  return m_position;
}

auto TargetPickedMessage::serialize(std::ostream &out) const -> std::ostream &
{
  ::core::serialize(out, m_messageType);
  ::core::serialize(out, m_playerDbId);
  ::core::serialize(out, m_systemDbId);

  ::core::serialize(out, m_data);
  ::core::serialize(out, m_position);

  return out;
}

bool TargetPickedMessage::deserialize(std::istream &in)
{
  bool ok{true};
  ok &= ::core::deserialize(in, m_messageType);
  ok &= ::core::deserialize(in, m_playerDbId);
  ok &= ::core::deserialize(in, m_systemDbId);

  ok &= ::core::deserialize(in, m_data);
  ok &= ::core::deserialize(in, m_position);

  return ok;
}

auto TargetPickedMessage::clone() const -> IMessagePtr
{
  return std::make_unique<TargetPickedMessage>(m_playerDbId, m_systemDbId, m_data, m_position);
}

auto TargetPickedMessage::readFromStream(std::istream &in) -> std::optional<IMessagePtr>
{
  TargetPickedMessage message;

  if (!message.deserialize(in))
  {
    return {};
  }

  return message.clone();
}

} // namespace bsgalone::core
