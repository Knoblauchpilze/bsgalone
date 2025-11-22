
#pragma once

#include "NetworkMessage.hh"
#include "TargetData.hh"
#include "Uuid.hh"

namespace bsgo {

class TargetListMessage : public NetworkMessage
{
  public:
  TargetListMessage();
  TargetListMessage(const Uuid systemDbId, const std::vector<TargetData> &targetsData);
  ~TargetListMessage() override = default;

  auto getSystemDbId() const -> Uuid;
  auto getTargetsData() const -> const std::vector<TargetData> &;

  auto serialize(std::ostream &out) const -> std::ostream & override;
  bool deserialize(std::istream &in) override;

  auto clone() const -> IMessagePtr override;

  private:
  Uuid m_systemDbId{};
  std::vector<TargetData> m_targetsData{};
};

} // namespace bsgo
