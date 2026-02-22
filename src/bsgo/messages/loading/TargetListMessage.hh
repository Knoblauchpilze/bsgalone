
#pragma once

#include "NetworkMessage.hh"
#include "Target.hh"
#include "Uuid.hh"

namespace bsgo {

class TargetListMessage : public bsgalone::core::NetworkMessage
{
  public:
  TargetListMessage();
  TargetListMessage(const Uuid systemDbId, const std::vector<bsgalone::core::Target> &targetsData);
  ~TargetListMessage() override = default;

  auto getSystemDbId() const -> Uuid;
  auto getTargetsData() const -> const std::vector<bsgalone::core::Target> &;

  auto serialize(std::ostream &out) const -> std::ostream & override;
  bool deserialize(std::istream &in) override;

  auto clone() const -> bsgalone::core::IMessagePtr override;

  private:
  Uuid m_systemDbId{};
  std::vector<bsgalone::core::Target> m_targetsData{};
};

} // namespace bsgo
