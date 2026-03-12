
#pragma once

#include "NetworkMessage.hh"
#include "Uuid.hh"

namespace bsgalone::core {

class LootMessage : public NetworkMessage
{
  public:
  LootMessage();
  LootMessage(const Uuid playerDbId, const Uuid resourceDbId, const int amount);
  ~LootMessage() override = default;

  auto getPlayerDbId() const -> Uuid;
  auto getResourceDbId() const -> Uuid;
  auto amount() const -> int;

  auto clone() const -> IMessagePtr override;

  private:
  Uuid m_playerDbId{};
  Uuid m_resourceDbId{};
  int m_amount{};

  friend auto operator<<(std::ostream &out, const LootMessage &message) -> std::ostream &;
  friend auto operator>>(std::istream &in, LootMessage &message) -> std::istream &;
};

auto operator<<(std::ostream &out, const LootMessage &message) -> std::ostream &;
auto operator>>(std::istream &in, LootMessage &message) -> std::istream &;

} // namespace bsgalone::core
