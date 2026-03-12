
#include "WeaponComponentMessage.hh"
#include "SerializationUtils.hh"

namespace bsgalone::core {

WeaponComponentMessage::WeaponComponentMessage()
  : ComponentUpdatedMessage(MessageType::WEAPON_COMPONENT_UPDATED)
{}

WeaponComponentMessage::WeaponComponentMessage(const Uuid shipDbId,
                                               const Uuid weaponDbId,
                                               const bool active)
  : ComponentUpdatedMessage(MessageType::WEAPON_COMPONENT_UPDATED,
                            shipDbId,
                            ComponentType::WEAPON_SLOT)
  , m_weaponDbId(weaponDbId)
  , m_active(active)
{}

auto WeaponComponentMessage::getWeaponDbId() const -> int
{
  return m_weaponDbId;
}

bool WeaponComponentMessage::isActive() const
{
  return m_active;
}

auto WeaponComponentMessage::clone() const -> IMessagePtr
{
  auto clone          = std::make_unique<WeaponComponentMessage>();
  clone->m_shipDbId   = m_shipDbId;
  clone->m_component  = m_component;
  clone->m_weaponDbId = m_weaponDbId;
  clone->m_active     = m_active;

  clone->copyClientIdIfDefined(*this);

  return clone;
}

auto operator<<(std::ostream &out, const WeaponComponentMessage &message) -> std::ostream &
{
  ::core::serialize(out, message.m_type);
  ::core::serialize(out, message.m_clientId);
  ::core::serialize(out, message.m_shipDbId);
  ::core::serialize(out, message.m_component);
  ::core::serialize(out, message.m_weaponDbId);
  ::core::serialize(out, message.m_active);

  return out;
}

auto operator>>(std::istream &in, WeaponComponentMessage &message) -> std::istream &
{
  ::core::deserialize(in, message.m_type);
  ::core::deserialize(in, message.m_clientId);
  ::core::deserialize(in, message.m_shipDbId);
  ::core::deserialize(in, message.m_component);
  ::core::deserialize(in, message.m_weaponDbId);
  ::core::deserialize(in, message.m_active);

  return in;
}

} // namespace bsgalone::core
