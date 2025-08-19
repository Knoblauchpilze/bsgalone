
#include "WeaponListMessage.hh"
#include "DataSerialization.hh"
#include "SerializationUtils.hh"

namespace bsgo {

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

auto WeaponListMessage::serialize(std::ostream &out) const -> std::ostream &
{
  core::serialize(out, m_messageType);
  core::serialize(out, m_clientId);

  core::serialize(out, m_weaponsData.size());
  for (const auto &weaponData : m_weaponsData)
  {
    serializeWeaponData(out, weaponData);
  }

  return out;
}

bool WeaponListMessage::deserialize(std::istream &in)
{
  bool ok{true};
  ok &= core::deserialize(in, m_messageType);
  ok &= core::deserialize(in, m_clientId);

  std::size_t count;
  ok &= core::deserialize(in, count);

  m_weaponsData.clear();

  for (std::size_t id = 0u; id < count; ++id)
  {
    WeaponData data;

    ok &= deserializeWeaponData(in, data);

    m_weaponsData.emplace_back(data);
  }

  return ok;
}

auto WeaponListMessage::clone() const -> IMessagePtr
{
  auto clone = std::make_unique<WeaponListMessage>(m_weaponsData);
  clone->copyClientIdIfDefined(*this);

  return clone;
}

} // namespace bsgo
