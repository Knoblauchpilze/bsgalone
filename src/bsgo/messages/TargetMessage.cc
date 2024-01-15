

#include "TargetMessage.hh"
#include <core_utils/SerializationUtils.hh>

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

auto TargetMessage::operator<<(std::ostream &out) const -> std::ostream &
{
  utils::serialize(out, m_messageType);

  out << m_shipEntityId;
  /// TODO: Position Eigen::Vector3f m_position{};

  return out;
}

auto TargetMessage::operator>>(std::istream &in) -> std::istream &
{
  utils::deserialize(in, m_messageType);

  in >> m_shipEntityId;
  /// TODO: Position Eigen::Vector3f m_position{};

  return in;
}

} // namespace bsgo
