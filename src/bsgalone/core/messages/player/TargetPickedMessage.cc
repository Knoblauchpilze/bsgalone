

#include "TargetPickedMessage.hh"
#include "SerializationUtils.hh"
#include "TargetSerialization.hh"

namespace bsgalone::core {

TargetPickedMessage::TargetPickedMessage()
  : AbstractPlayerCommand(MessageType::TARGET_PICKED, Uuid{0}, Uuid{0})
{}

TargetPickedMessage::TargetPickedMessage(const Uuid playerDbId,
                                         const Uuid systemDbId,
                                         Target data,
                                         const Eigen::Vector3f &position)
  : AbstractPlayerCommand(MessageType::TARGET_PICKED, playerDbId, systemDbId)
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

auto TargetPickedMessage::clone() const -> IMessagePtr
{
  return std::make_unique<TargetPickedMessage>(m_playerDbId, m_systemDbId, m_data, m_position);
}

auto TargetPickedMessage::readFromStream(std::istream &in) -> std::optional<IMessagePtr>
{
  TargetPickedMessage message;

  bool ok{true};
  ok &= ::core::deserialize(in, message.m_type);
  ok &= ::core::deserialize(in, message.m_playerDbId);
  ok &= ::core::deserialize(in, message.m_systemDbId);
  ok &= ::core::deserialize(in, message.m_data);
  ok &= ::core::deserialize(in, message.m_position);

  if (!ok)
  {
    return {};
  }

  return message.clone();
}

auto operator<<(std::ostream &out, const TargetPickedMessage &message) -> std::ostream &
{
  ::core::serialize(out, message.m_type);
  ::core::serialize(out, message.m_playerDbId);
  ::core::serialize(out, message.m_systemDbId);
  ::core::serialize(out, message.m_data);
  ::core::serialize(out, message.m_position);

  return out;
}

} // namespace bsgalone::core
