
#include "ScannedMessage.hh"
#include <core_utils/SerializationUtils.hh>

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
  utils::serialize(out, m_messageType);
  utils::serialize(out, m_clientId);
  utils::serialize(out, m_validated);

  utils::serialize(out, m_playerDbId);
  utils::serialize(out, m_asteroidDbId);

  return out;
}

bool ScannedMessage::deserialize(std::istream &in)
{
  bool ok{true};
  ok &= utils::deserialize(in, m_messageType);
  ok &= utils::deserialize(in, m_clientId);
  ok &= utils::deserialize(in, m_validated);

  ok &= utils::deserialize(in, m_playerDbId);
  ok &= utils::deserialize(in, m_asteroidDbId);

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
