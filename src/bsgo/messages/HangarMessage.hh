

#pragma once

#include "AbstractMessage.hh"
#include "Uuid.hh"
#include <eigen3/Eigen/Eigen>

namespace bsgo {

enum class ShipSwitchRequestState
{
  REQUESTED,
  COMPLETED
};

class HangarMessage : public AbstractMessage
{
  public:
  HangarMessage(const Uuid &shipDbId, const ShipSwitchRequestState &state);
  ~HangarMessage() override = default;

  auto getShipDbId() const -> Uuid;
  auto getRequestState() const -> ShipSwitchRequestState;

  auto serialize(std::ostream &out) const -> std::ostream & override;
  auto deserialize(std::istream &in) -> std::istream & override;

  private:
  Uuid m_shipDbId{};
  ShipSwitchRequestState m_state{};
};

} // namespace bsgo
