
#pragma once

#include "NetworkMessage.hh"
#include "Uuid.hh"

namespace bsgo {

class LootMessage : public NetworkMessage
{
  public:
  LootMessage();
  LootMessage(const Uuid playerDbId, const Uuid resourceDbId, const int amount);
  ~LootMessage() override = default;

  auto getPlayerDbId() const -> Uuid;
  auto getResourceDbId() const -> Uuid;
  auto amount() const -> int;

  auto serialize(std::ostream &out) const -> std::ostream & override;
  bool deserialize(std::istream &in) override;

  auto clone() const -> IMessagePtr override;

  private:
  Uuid m_playerDbId{};
  Uuid m_resourceDbId{};
  int m_amount{};
};

} // namespace bsgo
