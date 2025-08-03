
#include "PlayerWeaponListMessage.hh"
#include "DataSerialization.hh"
#include "SerializationUtils.hh"

namespace bsgo {

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

auto PlayerWeaponListMessage::serialize(std::ostream &out) const -> std::ostream &
{
  core::serialize(out, m_messageType);
  core::serialize(out, m_clientId);

  core::serialize(out, m_weaponsData.size());
  for (const auto &weaponData : m_weaponsData)
  {
    serializePlayerWeaponData(out, weaponData);
  }

  return out;
}

bool PlayerWeaponListMessage::deserialize(std::istream &in)
{
  bool ok{true};
  ok &= core::deserialize(in, m_messageType);
  ok &= core::deserialize(in, m_clientId);

  std::size_t count;
  ok &= core::deserialize(in, count);

  m_weaponsData.clear();

  for (std::size_t id = 0u; id < count; ++id)
  {
    PlayerWeaponData data;

    ok &= deserializePlayerWeaponData(in, data);

    m_weaponsData.emplace_back(data);
  }

  return ok;
}

auto PlayerWeaponListMessage::clone() const -> IMessagePtr
{
  auto clone = std::make_unique<PlayerWeaponListMessage>(m_weaponsData);
  clone->copyClientIdIfDefined(*this);

  return clone;
}

} // namespace bsgo
