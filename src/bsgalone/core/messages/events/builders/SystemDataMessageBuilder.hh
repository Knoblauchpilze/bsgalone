
#pragma once

#include "SystemDataMessage.hh"
#include <optional>
#include <vector>

namespace bsgalone::core {

class SystemDataMessageBuilder
{
  public:
  SystemDataMessageBuilder(const Uuid playerDbId);
  ~SystemDataMessageBuilder() = default;

  auto withSystemName(std::string name) -> SystemDataMessageBuilder &;
  auto withCurrentTick(chrono::Tick currentTick) -> SystemDataMessageBuilder &;
  auto withTimeStep(chrono::TimeStep timeStep) -> SystemDataMessageBuilder &;
  auto withAsteroids(std::vector<Asteroid> asteroids) -> SystemDataMessageBuilder &;

  auto build() const -> SystemDataMessage;

  private:
  Uuid m_playerDbId{};

  std::optional<std::string> m_name{};
  std::optional<chrono::Tick> m_currentTick{};
  std::optional<chrono::TimeStep> m_timeStep{};
  std::optional<std::vector<Asteroid>> m_asteroids{};

  void validate() const;
};

} // namespace bsgalone::core
