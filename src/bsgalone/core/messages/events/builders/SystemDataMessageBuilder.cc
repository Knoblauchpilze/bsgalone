
#include "SystemDataMessageBuilder.hh"

namespace bsgalone::core {

SystemDataMessageBuilder::SystemDataMessageBuilder(const Uuid playerDbId)
  : m_playerDbId(playerDbId)
{}

auto SystemDataMessageBuilder::withSystemName(std::string name) -> SystemDataMessageBuilder &
{
  m_name = std::move(name);
  return *this;
}

auto SystemDataMessageBuilder::withCurrentTick(chrono::Tick currentTick)
  -> SystemDataMessageBuilder &
{
  m_currentTick = std::move(currentTick);
  return *this;
}

auto SystemDataMessageBuilder::withTimeStep(chrono::TimeStep timeStep) -> SystemDataMessageBuilder &
{
  m_timeStep = std::move(timeStep);
  return *this;
}

auto SystemDataMessageBuilder::withAsteroids(std::vector<Asteroid> asteroids)
  -> SystemDataMessageBuilder &
{
  m_asteroids = std::move(asteroids);
  return *this;
}

auto SystemDataMessageBuilder::build() const -> SystemDataMessage
{
  validate();

  SystemDataMessage message;

  message.m_playerDbId  = m_playerDbId;
  message.m_name        = *m_name;
  message.m_currentTick = *m_currentTick;
  message.m_timeStep    = *m_timeStep;
  message.m_asteroids   = *m_asteroids;

  return message;
}

void SystemDataMessageBuilder::validate() const
{
  if (!m_name.has_value())
  {
    throw std::invalid_argument("Expected value for field name");
  }
  if (!m_currentTick.has_value())
  {
    throw std::invalid_argument("Expected value for field current tick");
  }
  if (!m_timeStep.has_value())
  {
    throw std::invalid_argument("Expected value for field time step");
  }
  if (!m_asteroids.has_value())
  {
    throw std::invalid_argument("Expected value for field asteroids");
  }
}

} // namespace bsgalone::core
