
#include "DockMessage.hh"
#include "SerializationUtils.hh"

namespace bsgalone::core {

DockMessage::DockMessage()
  : AbstractPlayerMessage(MessageType::DOCK, Uuid{0}, Uuid{0})
{}

DockMessage::DockMessage(const Uuid playerDbId,
                         const Uuid systemDbId,
                         const Uuid shipDbId,
                         const DockTransition transition)
  : AbstractPlayerMessage(MessageType::DOCK, playerDbId, systemDbId)
  , m_shipDbId(shipDbId)
  , m_transition(transition)
{}

auto DockMessage::getShipDbId() const -> Uuid
{
  return m_shipDbId;
}

auto DockMessage::getTransition() const -> DockTransition
{
  return m_transition;
}

auto DockMessage::clone() const -> IMessagePtr
{
  return std::make_unique<DockMessage>(m_playerDbId, m_systemDbId, m_shipDbId, m_transition);
}

auto DockMessage::readFromStream(std::istream &in) -> std::optional<IMessagePtr>
{
  DockMessage message;

  bool ok{true};
  ok &= ::core::deserialize(in, message.m_type);
  ok &= ::core::deserialize(in, message.m_playerDbId);
  ok &= ::core::deserialize(in, message.m_systemDbId);
  ok &= ::core::deserialize(in, message.m_shipDbId);
  ok &= ::core::deserialize(in, message.m_transition);

  if (!ok)
  {
    return {};
  }

  return message.clone();
}

auto operator<<(std::ostream &out, const DockMessage &message) -> std::ostream &
{
  ::core::serialize(out, message.m_type);
  ::core::serialize(out, message.m_playerDbId);
  ::core::serialize(out, message.m_systemDbId);
  ::core::serialize(out, message.m_shipDbId);
  ::core::serialize(out, message.m_transition);

  return out;
}

} // namespace bsgalone::core
