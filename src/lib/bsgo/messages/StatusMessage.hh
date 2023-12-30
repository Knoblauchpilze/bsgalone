
#pragma once

#include "SystemMessage.hh"
#include "Uuid.hh"

namespace bsgo {

class StatusMessage : public SystemMessage
{
  public:
  StatusMessage(const Uuid &playerDbId);
  ~StatusMessage() override = default;

  auto getPlayerDbId() const -> Uuid;

  private:
  Uuid m_playerDbId{};
};

} // namespace bsgo
