
#pragma once

#include "AsteroidData.hh"
#include "NetworkMessage.hh"
#include "Uuid.hh"

namespace bsgalone::core {

class AsteroidListMessage : public NetworkMessage
{
  public:
  AsteroidListMessage();
  AsteroidListMessage(const Uuid systemDbId, const std::vector<AsteroidData> &asteroidsData);
  ~AsteroidListMessage() override = default;

  auto getSystemDbId() const -> Uuid;
  auto getAsteroidsData() const -> const std::vector<AsteroidData> &;

  auto clone() const -> IMessagePtr override;

  private:
  Uuid m_systemDbId{};
  std::vector<AsteroidData> m_asteroidsData{};

  friend auto operator<<(std::ostream &out, const AsteroidListMessage &message) -> std::ostream &;
  friend auto operator>>(std::istream &in, AsteroidListMessage &message) -> std::istream &;
};

auto operator<<(std::ostream &out, const AsteroidListMessage &message) -> std::ostream &;
auto operator>>(std::istream &in, AsteroidListMessage &message) -> std::istream &;

} // namespace bsgalone::core
