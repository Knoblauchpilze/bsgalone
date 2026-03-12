
#include "WeaponListMessage.hh"
#include "SerializationUtils.hh"

namespace bsgalone::core {

WeaponListMessage::WeaponListMessage()
  : NetworkMessage(MessageType::WEAPON_LIST)
{}

WeaponListMessage::WeaponListMessage(const std::vector<WeaponData> &weaponsData)
  : NetworkMessage(MessageType::WEAPON_LIST)
  , m_weaponsData(weaponsData)
{}

auto WeaponListMessage::getWeaponsData() const -> const std::vector<WeaponData> &
{
  return m_weaponsData;
}

auto WeaponListMessage::clone() const -> IMessagePtr
{
  auto clone = std::make_unique<WeaponListMessage>(m_weaponsData);
  clone->copyClientIdIfDefined(*this);

  return clone;
}

auto operator<<(std::ostream &out, const WeaponListMessage &message) -> std::ostream &
{
  ::core::serialize(out, message.m_type);
  ::core::serialize(out, message.m_clientId);
  ::core::serialize(out, message.m_weaponsData);

  return out;
}

auto operator>>(std::istream &in, WeaponListMessage &message) -> std::istream &
{
  ::core::deserialize(in, message.m_type);
  ::core::deserialize(in, message.m_clientId);
  ::core::deserialize(in, message.m_weaponsData);

  return in;
}

} // namespace bsgalone::core
