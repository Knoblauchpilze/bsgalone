
#include "PlayerWeaponListMessage.hh"
#include "SerializationUtils.hh"

namespace bsgalone::core {

PlayerWeaponListMessage::PlayerWeaponListMessage()
  : NetworkMessage(MessageType::PLAYER_WEAPON_LIST)
{}

PlayerWeaponListMessage::PlayerWeaponListMessage(const std::vector<PlayerWeaponData> &weaponsData)
  : NetworkMessage(MessageType::PLAYER_WEAPON_LIST)
  , m_weaponsData(weaponsData)
{}

auto PlayerWeaponListMessage::getWeaponsData() const -> const std::vector<PlayerWeaponData> &
{
  return m_weaponsData;
}

auto PlayerWeaponListMessage::clone() const -> IMessagePtr
{
  auto clone = std::make_unique<PlayerWeaponListMessage>(m_weaponsData);
  clone->copyClientIdIfDefined(*this);

  return clone;
}

auto operator<<(std::ostream &out, const PlayerWeaponListMessage &message) -> std::ostream &
{
  ::core::serialize(out, message.m_type);
  ::core::serialize(out, message.m_clientId);
  ::core::serialize(out, message.m_weaponsData);

  return out;
}

auto operator>>(std::istream &in, PlayerWeaponListMessage &message) -> std::istream &
{
  ::core::deserialize(in, message.m_type);
  ::core::deserialize(in, message.m_clientId);
  ::core::deserialize(in, message.m_weaponsData);

  return in;
}

} // namespace bsgalone::core
