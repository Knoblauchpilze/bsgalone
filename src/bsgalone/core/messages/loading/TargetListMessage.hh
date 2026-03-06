
#pragma once

#include "NetworkMessage.hh"
#include "Target.hh"
#include "Uuid.hh"

namespace bsgalone::core {

class TargetListMessage : public NetworkMessage
{
  public:
  TargetListMessage();
  TargetListMessage(const Uuid systemDbId, const std::vector<Target> &targetsData);
  ~TargetListMessage() override = default;

  auto getSystemDbId() const -> Uuid;
  auto getTargetsData() const -> const std::vector<Target> &;

  auto serialize(std::ostream &out) const -> std::ostream & override;
  bool deserialize(std::istream &in) override;

  auto clone() const -> IMessagePtr override;

  private:
  Uuid m_systemDbId{};
  std::vector<Target> m_targetsData{};
};

} // namespace bsgalone::core
