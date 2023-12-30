
#pragma once

#include "SystemMessage.hh"
#include "Uuid.hh"

namespace bsgo {

class StatusMessage : public SystemMessage
{
  public:
  StatusMessage(const Uuid &shipEntityId);
  ~StatusMessage() override = default;

  auto getShipEntityId() const -> Uuid;

  private:
  Uuid m_shipEntityId{};
};

} // namespace bsgo
