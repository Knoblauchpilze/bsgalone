
#include "SignupMessage.hh"
#include <core_utils/SerializationUtils.hh>

namespace bsgo {

SignupMessage::SignupMessage()
  : ValidatableMessage(MessageType::SIGNUP)
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
  : ValidatableMessage(MessageType::SIGNUP)
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
  utils::serialize(out, m_messageType);
  utils::serialize(out, m_clientId);
  utils::serialize(out, m_validated);

  utils::serialize(out, m_name);
  utils::serialize(out, m_password);
  utils::serialize(out, m_faction);

  utils::serialize(out, m_playerDbId);

  return out;
}

bool SignupMessage::deserialize(std::istream &in)
{
  bool ok{true};
  ok &= utils::deserialize(in, m_messageType);
  ok &= utils::deserialize(in, m_clientId);
  ok &= utils::deserialize(in, m_validated);

  ok &= utils::deserialize(in, m_name);
  ok &= utils::deserialize(in, m_password);
  ok &= utils::deserialize(in, m_faction);

  ok &= utils::deserialize(in, m_playerDbId);

  return ok;
}

auto SignupMessage::clone() const -> IMessagePtr
{
  auto clone = std::make_unique<SignupMessage>(m_name, m_password, m_faction, m_playerDbId);
  clone->copyClientIdIfDefined(*this);
  clone->validate(validated());

  return clone;
}

} // namespace bsgo
