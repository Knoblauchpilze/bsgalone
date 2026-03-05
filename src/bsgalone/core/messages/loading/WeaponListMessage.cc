
#include "WeaponListMessage.hh"
#include "SerializationUtils.hh"

namespace bsgo {

WeaponListMessage::WeaponListMessage()
  : NetworkMessage(bsgalone::core::MessageType::WEAPON_LIST)
{}

WeaponListMessage::WeaponListMessage(const std::vector<WeaponData> &weaponsData)
  : NetworkMessage(bsgalone::core::MessageType::WEAPON_LIST)
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

  core::serialize(out, m_weaponsData);

  return out;
}

bool WeaponListMessage::deserialize(std::istream &in)
{
  bool ok{true};
  ok &= core::deserialize(in, m_messageType);
  ok &= core::deserialize(in, m_clientId);

  ok &= core::deserialize(in, m_weaponsData);

  return ok;
}

auto WeaponListMessage::clone() const -> bsgalone::core::IMessagePtr
{
  auto clone = std::make_unique<WeaponListMessage>(m_weaponsData);
  clone->copyClientIdIfDefined(*this);

  return clone;
}

} // namespace bsgo
