

#pragma once

#include "AsteroidData.hh"
#include "EntityKind.hh"
#include "NetworkMessage.hh"
#include "OutpostData.hh"
#include "PlayerData.hh"
#include "PlayerShipData.hh"
#include "Uuid.hh"
#include <optional>

namespace bsgalone::core {

class EntityAddedMessage : public NetworkMessage
{
  public:
  EntityAddedMessage();
  EntityAddedMessage(const Uuid systemDbId);
  ~EntityAddedMessage() override = default;

  auto getSystemDbId() const -> Uuid;
  auto getEntityKind() const -> EntityKind;

  void setAsteroidData(const AsteroidData &data);
  auto tryGetAsteroidData() const -> std::optional<AsteroidData>;

  void setShipData(const PlayerShipData &data);
  auto tryGetShipData() const -> std::optional<PlayerShipData>;

  void setOutpostData(const OutpostData &data);
  auto tryGetOutpostData() const -> std::optional<OutpostData>;

  void setPlayerData(const PlayerData &data);
  auto tryGetPlayerData() const -> std::optional<PlayerData>;

  auto clone() const -> IMessagePtr override;

  private:
  Uuid m_systemDbId{};
  std::optional<EntityKind> m_entityKind{};

  std::optional<AsteroidData> m_asteroidData{};
  std::optional<PlayerShipData> m_shipData{};
  std::optional<OutpostData> m_outpostData{};
  std::optional<PlayerData> m_playerData{};

  friend auto operator<<(std::ostream &out, const EntityAddedMessage &message) -> std::ostream &;
  friend auto operator>>(std::istream &in, EntityAddedMessage &message) -> std::istream &;
};

auto operator<<(std::ostream &out, const EntityAddedMessage &message) -> std::ostream &;
auto operator>>(std::istream &in, EntityAddedMessage &message) -> std::istream &;

} // namespace bsgalone::core
