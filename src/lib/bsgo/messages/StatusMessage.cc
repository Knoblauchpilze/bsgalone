
#include "StatusMessage.hh"

namespace bsgo {

StatusMessage::StatusMessage(const Uuid &shipEntityId)
  : SystemMessage(SystemType::STATUS)
  , m_shipEntityId(shipEntityId)
{}

auto StatusMessage::getShipEntityId() const -> Uuid
{
  return m_shipEntityId;
}

} // namespace bsgo
