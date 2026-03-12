
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

  auto clone() const -> IMessagePtr override;

  private:
  Uuid m_systemDbId{};
  std::vector<Target> m_targetsData{};

  friend auto operator<<(std::ostream &out, const TargetListMessage &message) -> std::ostream &;
  friend auto operator>>(std::istream &in, TargetListMessage &message) -> std::istream &;
};

auto operator<<(std::ostream &out, const TargetListMessage &message) -> std::ostream &;
auto operator>>(std::istream &in, TargetListMessage &message) -> std::istream &;

} // namespace bsgalone::core
