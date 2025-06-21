
#pragma once

#include "AsteroidData.hh"
#include "NetworkMessage.hh"
#include "Uuid.hh"

namespace bsgo {

class AsteroidListMessage : public NetworkMessage
{
  public:
  AsteroidListMessage();
  AsteroidListMessage(const Uuid systemDbId, const std::vector<AsteroidData> &asteroidsData);
  ~AsteroidListMessage() override = default;

  auto getSystemDbId() const -> Uuid;
  auto getAsteroidsData() const -> const std::vector<AsteroidData> &;

  auto serialize(std::ostream &out) const -> std::ostream & override;
  bool deserialize(std::istream &in) override;

  auto clone() const -> IMessagePtr override;

  private:
  Uuid m_systemDbId{};
  std::vector<AsteroidData> m_asteroidsData{};
};

} // namespace bsgo
