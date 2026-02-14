
#include "SignupMessage.hh"
#include "SerializationUtils.hh"

namespace bsgo {

SignupMessage::SignupMessage()
  : NetworkMessage(MessageType::SIGNUP)
{}

SignupMessage::SignupMessage(const std::string &name,
                             const std::string &password,
                             const Faction &faction)
  : SignupMessage(name, password, faction, {})
{}

SignupMessage::SignupMessage(const std::string &name,
                             const std::string &password,
                             const Faction &faction,
                             const std::optional<Uuid> &playerDbId)
  : NetworkMessage(MessageType::SIGNUP)
  , m_name(name)
  , m_password(password)
  , m_faction(faction)
  , m_playerDbId(playerDbId)
{}

auto SignupMessage::getUserName() const -> std::string
{
  return m_name;
}

auto SignupMessage::getUserPassword() const -> std::string
{
  return m_password;
}

auto SignupMessage::getFaction() const -> Faction
{
  return m_faction;
}

bool SignupMessage::successfullySignedup() const
{
  return m_playerDbId.has_value();
}

auto SignupMessage::getPlayerDbId() const -> std::optional<Uuid>
{
  return m_playerDbId;
}

auto SignupMessage::serialize(std::ostream &out) const -> std::ostream &
{
  core::serialize(out, m_messageType);
  core::serialize(out, m_clientId);

  core::serialize(out, m_name);
  core::serialize(out, m_password);
  core::serialize(out, m_faction);

  core::serialize(out, m_playerDbId);

  return out;
}

bool SignupMessage::deserialize(std::istream &in)
{
  bool ok{true};
  ok &= core::deserialize(in, m_messageType);
  ok &= core::deserialize(in, m_clientId);

  ok &= core::deserialize(in, m_name);
  ok &= core::deserialize(in, m_password);
  ok &= core::deserialize(in, m_faction);

  ok &= core::deserialize(in, m_playerDbId);

  return ok;
}

auto SignupMessage::clone() const -> bsgalone::core::IMessagePtr
{
  auto clone = std::make_unique<SignupMessage>(m_name, m_password, m_faction, m_playerDbId);
  clone->copyClientIdIfDefined(*this);

  return clone;
}

} // namespace bsgo
