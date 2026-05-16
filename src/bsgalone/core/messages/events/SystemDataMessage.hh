
#pragma once

#include "Asteroid.hh"
#include "IMessage.hh"
#include "Uuid.hh"
#include <vector>

namespace bsgalone::core {

class SystemDataMessage : public IMessage
{
  public:
  SystemDataMessage(const Uuid playerDbId, std::vector<Asteroid> asteroids);
  ~SystemDataMessage() override = default;

  auto getPlayerDbId() const -> Uuid;
  auto getAsteroids() const -> const std::vector<Asteroid> &;

  auto clone() const -> IMessagePtr override;

  /// @brief - Tries to read a message from the input stream. If it
  /// succeeds, a valid message will be returned, otherwise an empty
  /// optional.
  /// The stream will **not be reset** in case the reading fails.
  /// @param in - the stream to read from
  /// @return - the deserialized message or an empty optional if reading
  /// the message's data fails
  static auto readFromStream(std::istream &in) -> std::optional<IMessagePtr>;

  private:
  Uuid m_playerDbId{};
  std::vector<Asteroid> m_asteroids{};

  SystemDataMessage();

  friend auto operator<<(std::ostream &out, const SystemDataMessage &message) -> std::ostream &;
};

auto operator<<(std::ostream &out, const SystemDataMessage &message) -> std::ostream &;

} // namespace bsgalone::core
