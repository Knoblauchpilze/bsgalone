

#pragma once

#include "AsteroidData.hh"
#include "EntityKind.hh"
#include "NetworkMessage.hh"
#include "OutpostData.hh"
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

  auto serialize(std::ostream &out) const -> std::ostream & override;
  bool deserialize(std::istream &in) override;

  auto clone() const -> IMessagePtr override;

  private:
  Uuid m_systemDbId{};
  std::optional<EntityKind> m_entityKind{};

  std::optional<AsteroidData> m_asteroidData{};
  std::optional<ShipData> m_shipData{};
  std::optional<OutpostData> m_outpostData{};
};

} // namespace bsgo
