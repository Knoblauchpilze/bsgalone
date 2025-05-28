
#include "ScannedMessage.hh"
#include "SerializationUtils.hh"

namespace bsgo {

ScannedMessage::ScannedMessage()
  : ValidatableMessage(MessageType::SCANNED)
{}

ScannedMessage::ScannedMessage(const Uuid playerDbId, const Uuid asteroidDbId)
  : ValidatableMessage(MessageType::SCANNED)
  , m_playerDbId(playerDbId)
  , m_asteroidDbId(asteroidDbId)
{}

auto ScannedMessage::getPlayerDbId() const -> Uuid
{
  return m_playerDbId;
}

auto ScannedMessage::getAsteroidDbId() const -> Uuid
{
  return m_asteroidDbId;
}

auto ScannedMessage::serialize(std::ostream &out) const -> std::ostream &
{
  core::serialize(out, m_messageType);
  core::serialize(out, m_clientId);
  core::serialize(out, m_validated);

  core::serialize(out, m_playerDbId);
  core::serialize(out, m_asteroidDbId);

  return out;
}

bool ScannedMessage::deserialize(std::istream &in)
{
  bool ok{true};
  ok &= core::deserialize(in, m_messageType);
  ok &= core::deserialize(in, m_clientId);
  ok &= core::deserialize(in, m_validated);

  ok &= core::deserialize(in, m_playerDbId);
  ok &= core::deserialize(in, m_asteroidDbId);

  return ok;
}

auto ScannedMessage::clone() const -> IMessagePtr
{
  auto clone = std::make_unique<ScannedMessage>(m_playerDbId, m_asteroidDbId);
  clone->copyClientIdIfDefined(*this);
  clone->validate(validated());

  return clone;
}

} // namespace bsgo
