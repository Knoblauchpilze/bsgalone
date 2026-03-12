
#pragma once

#include "NetworkMessage.hh"
#include "Uuid.hh"

namespace bsgalone::core {

class ScannedMessage : public NetworkMessage
{
  public:
  ScannedMessage();
  ScannedMessage(const Uuid playerDbId, const Uuid asteroidDbId);
  ~ScannedMessage() override = default;

  auto getPlayerDbId() const -> Uuid;
  auto getAsteroidDbId() const -> Uuid;

  auto clone() const -> IMessagePtr override;

  private:
  Uuid m_playerDbId{};
  Uuid m_asteroidDbId{};

  friend auto operator<<(std::ostream &out, const ScannedMessage &message) -> std::ostream &;
  friend auto operator>>(std::istream &in, ScannedMessage &message) -> std::istream &;
};

auto operator<<(std::ostream &out, const ScannedMessage &message) -> std::ostream &;
auto operator>>(std::istream &in, ScannedMessage &message) -> std::istream &;

} // namespace bsgalone::core
