

#pragma once

#include "AsteroidData.hh"
#include "EntityKind.hh"
#include "NetworkMessage.hh"
#include "OutpostData.hh"
#include "PlayerData.hh"
#include "ShipData.hh"
#include "Uuid.hh"
#include <optional>

namespace bsgo {

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

  void setShipData(const ShipData &data);
  auto tryGetShipData() const -> std::optional<ShipData>;

  void setOutpostData(const OutpostData &data);
  auto tryGetOutpostData() const -> std::optional<OutpostData>;

  void setPlayerData(const PlayerData &data);
  auto tryGetPlayerData() const -> std::optional<PlayerData>;

  auto serialize(std::ostream &out) const -> std::ostream & override;
  bool deserialize(std::istream &in) override;

  auto clone() const -> IMessagePtr override;

  private:
  Uuid m_systemDbId{};
  std::optional<EntityKind> m_entityKind{};

  std::optional<AsteroidData> m_asteroidData{};
  std::optional<ShipData> m_shipData{};
  std::optional<OutpostData> m_outpostData{};
  std::optional<PlayerData> m_playerData{};
};

} // namespace bsgo
