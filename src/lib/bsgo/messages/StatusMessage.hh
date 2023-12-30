
#pragma once

#include "SystemMessage.hh"
#include "Uuid.hh"

namespace bsgo {

enum class JumpState
{
  STARTED,
  CANCELLED,
  FINISHED
};

class StatusMessage : public SystemMessage
{
  public:
  StatusMessage(const Uuid &shipDbId, const JumpState &jumpState);
  ~StatusMessage() override = default;

  auto getShipDbId() const -> Uuid;
  auto getJumpState() const -> JumpState;

  private:
  Uuid m_shipDbId{};
  JumpState m_jumpState{};
};

} // namespace bsgo
