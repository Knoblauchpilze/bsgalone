
#include "SignupMessage.hh"
#include "SerializationUtils.hh"

namespace bsgalone::core {

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

auto SignupMessage::clone() const -> IMessagePtr
{
  auto clone = std::make_unique<SignupMessage>(m_name, m_password, m_faction, m_playerDbId);
  clone->copyClientIdIfDefined(*this);

  return clone;
}

auto operator<<(std::ostream &out, const SignupMessage &message) -> std::ostream &
{
  ::core::serialize(out, message.m_type);
  ::core::serialize(out, message.m_clientId);
  ::core::serialize(out, message.m_name);
  ::core::serialize(out, message.m_password);
  ::core::serialize(out, message.m_faction);
  ::core::serialize(out, message.m_playerDbId);

  return out;
}

auto operator>>(std::istream &in, SignupMessage &message) -> std::istream &
{
  ::core::deserialize(in, message.m_type);
  ::core::deserialize(in, message.m_clientId);
  ::core::deserialize(in, message.m_name);
  ::core::deserialize(in, message.m_password);
  ::core::deserialize(in, message.m_faction);
  ::core::deserialize(in, message.m_playerDbId);

  return in;
}

} // namespace bsgalone::core
