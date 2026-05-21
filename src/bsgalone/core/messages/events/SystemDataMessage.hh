
#pragma once

#include "Asteroid.hh"
#include "IMessage.hh"
#include "Tick.hh"
#include "TimeStep.hh"
#include "Uuid.hh"
#include <vector>

namespace bsgalone::core {

class SystemDataMessageBuilder;

class SystemDataMessage : public IMessage
{
  public:
  ~SystemDataMessage() override = default;

  auto getPlayerDbId() const -> Uuid;
  auto getSystemName() const -> std::string;
  auto getCurrentTick() const -> chrono::Tick;
  auto getTimeStep() const -> chrono::TimeStep;
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

  std::string m_name{};
  chrono::Tick m_currentTick{};
  chrono::TimeStep m_timeStep{};

  std::vector<Asteroid> m_asteroids{};

  SystemDataMessage();

  // https://stackoverflow.com/questions/8147027/how-do-i-call-stdmake-shared-on-a-class-with-only-protected-or-private-const/8147213#8147213
  struct ptrEnabler;
  friend class ptrEnabler;

  friend class SystemDataMessageBuilder;
  friend auto operator<<(std::ostream &out, const SystemDataMessage &message) -> std::ostream &;
};

auto operator<<(std::ostream &out, const SystemDataMessage &message) -> std::ostream &;

} // namespace bsgalone::core
