

#include "TargetMessage.hh"

namespace bsgo {

TargetMessage::TargetMessage(const Uuid &shipEntityId, const Eigen::Vector3f &position)
  : AbstractMessage(MessageType::TARGET)
  , m_shipEntityId(shipEntityId)
  , m_position(position)
{}

auto TargetMessage::getShipEntityId() const -> Uuid
{
  return m_shipEntityId;
}

auto TargetMessage::getPosition() const -> Eigen::Vector3f
{
  return m_position;
}

} // namespace bsgo
