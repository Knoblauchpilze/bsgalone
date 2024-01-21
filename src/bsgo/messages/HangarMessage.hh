

#pragma once

#include "Uuid.hh"
#include "ValidatableMessage.hh"
#include <eigen3/Eigen/Eigen>

namespace bsgo {

class HangarMessage : public ValidatableMessage
{
  public:
  HangarMessage();
  HangarMessage(const Uuid &shipDbId);
  ~HangarMessage() override = default;

  auto getShipDbId() const -> Uuid;

  auto serialize(std::ostream &out) const -> std::ostream & override;
  bool deserialize(std::istream &in) override;

  private:
  Uuid m_shipDbId{};
};

} // namespace bsgo
