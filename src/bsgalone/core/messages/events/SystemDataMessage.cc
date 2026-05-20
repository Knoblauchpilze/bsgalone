
#include "SystemDataMessage.hh"
#include "SerializationUtils.hh"
#include "SystemDataMessageBuilder.hh"

namespace bsgalone::core {

struct SystemDataMessage::ptrEnabler : public SystemDataMessage
{
  ptrEnabler()
    : SystemDataMessage() {};
};

SystemDataMessage::SystemDataMessage()
  : IMessage(MessageType::SYSTEM_DATA)
{}

auto SystemDataMessage::getPlayerDbId() const -> Uuid
{
  return m_playerDbId;
}

auto SystemDataMessage::getName() const -> std::string
{
  return m_name;
}

auto SystemDataMessage::getCurrentTick() const -> chrono::Tick
{
  return m_currentTick;
}

auto SystemDataMessage::getTimeStep() const -> chrono::TimeStep
{
  return m_timeStep;
}

auto SystemDataMessage::getAsteroids() const -> const std::vector<Asteroid> &
{
  return m_asteroids;
}

auto SystemDataMessage::clone() const -> IMessagePtr
{
  auto out = std::make_unique<ptrEnabler>();

  out->m_playerDbId  = m_playerDbId;
  out->m_name        = m_name;
  out->m_currentTick = m_currentTick;
  out->m_timeStep    = m_timeStep;
  out->m_asteroids   = m_asteroids;

  return out;
}

bool operator>>(std::istream &in, Loot &loot)
{
  bool ok{true};
  ok &= ::core::deserialize(in, loot.resource);
  ok &= ::core::deserialize(in, loot.amount);

  return ok;
}

bool operator>>(std::istream &in, Asteroid &asteroid)
{
  bool ok{true};
  ok &= ::core::deserialize(in, asteroid.dbId);
  ok &= ::core::deserialize(in, asteroid.position);
  ok &= ::core::deserialize(in, asteroid.radius);
  ok &= ::core::deserialize(in, asteroid.health);
  ok &= ::core::deserialize(in, asteroid.maxHealth);
  ok &= ::core::deserialize(in, asteroid.loot);

  return ok;
}

auto SystemDataMessage::readFromStream(std::istream &in) -> std::optional<IMessagePtr>
{
  SystemDataMessage message;

  bool ok{true};
  ok &= ::core::deserialize(in, message.m_type);
  ok &= ::core::deserialize(in, message.m_playerDbId);
  ok &= ::core::deserialize(in, message.m_name);
  ok &= ::core::deserialize(in, message.m_currentTick);
  ok &= ::core::deserialize(in, message.m_timeStep);
  ok &= ::core::deserialize(in, message.m_asteroids);

  if (!ok)
  {
    return {};
  }

  return message.clone();
}

auto operator<<(std::ostream &out, const Loot &loot) -> std::ostream &
{
  ::core::serialize(out, loot.resource);
  ::core::serialize(out, loot.amount);

  return out;
}

auto operator<<(std::ostream &out, const Asteroid &asteroid) -> std::ostream &
{
  ::core::serialize(out, asteroid.dbId);
  ::core::serialize(out, asteroid.position);
  ::core::serialize(out, asteroid.radius);
  ::core::serialize(out, asteroid.health);
  ::core::serialize(out, asteroid.maxHealth);
  ::core::serialize(out, asteroid.loot);

  return out;
}

auto operator<<(std::ostream &out, const SystemDataMessage &message) -> std::ostream &
{
  ::core::serialize(out, message.m_type);
  ::core::serialize(out, message.m_playerDbId);
  ::core::serialize(out, message.m_name);
  ::core::serialize(out, message.m_currentTick);
  ::core::serialize(out, message.m_timeStep);
  ::core::serialize(out, message.m_asteroids);

  return out;
}

} // namespace bsgalone::core
