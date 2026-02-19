
#include "DockMessage.hh"
#include "SerializationUtils.hh"

namespace bsgalone::core {

DockMessage::DockMessage()
  : AbstractPlayerMessage(MessageType::DOCK, bsgo::Uuid{0}, bsgo::Uuid{0})
{}

DockMessage::DockMessage(const bsgo::Uuid playerDbId,
                         const bsgo::Uuid systemDbId,
                         const bsgo::Uuid shipDbId,
                         const DockTransition transition)
  : AbstractPlayerMessage(MessageType::DOCK, playerDbId, systemDbId)
  , m_shipDbId(shipDbId)
  , m_transition(transition)
{}

auto DockMessage::getShipDbId() const -> bsgo::Uuid
{
  return m_shipDbId;
}

auto DockMessage::getTransition() const -> DockTransition
{
  return m_transition;
}

auto DockMessage::serialize(std::ostream &out) const -> std::ostream &
{
  ::core::serialize(out, m_messageType);
  ::core::serialize(out, m_playerDbId);
  ::core::serialize(out, m_systemDbId);

  ::core::serialize(out, m_shipDbId);
  ::core::serialize(out, m_transition);

  return out;
}

bool DockMessage::deserialize(std::istream &in)
{
  bool ok{true};
  ok &= ::core::deserialize(in, m_messageType);
  ok &= ::core::deserialize(in, m_playerDbId);
  ok &= ::core::deserialize(in, m_systemDbId);

  ok &= ::core::deserialize(in, m_shipDbId);
  ok &= ::core::deserialize(in, m_transition);

  return ok;
}

auto DockMessage::clone() const -> IMessagePtr
{
  return std::make_unique<DockMessage>(m_playerDbId, m_systemDbId, m_shipDbId, m_transition);
}

auto DockMessage::readFromStream(std::istream &in) -> std::optional<IMessagePtr>
{
  DockMessage message;

  if (!message.deserialize(in))
  {
    return {};
  }

  return message.clone();
}

} // namespace bsgalone::core
