
#include "WeaponComponentMessage.hh"
#include "SerializationUtils.hh"

namespace bsgo {

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

auto WeaponComponentMessage::serialize(std::ostream &out) const -> std::ostream &
{
  core::serialize(out, m_messageType);
  core::serialize(out, m_clientId);

  core::serialize(out, m_shipDbId);
  core::serialize(out, m_component);
  core::serialize(out, m_weaponDbId);
  core::serialize(out, m_active);

  return out;
}

bool WeaponComponentMessage::deserialize(std::istream &in)
{
  bool ok{true};
  ok &= core::deserialize(in, m_messageType);
  ok &= core::deserialize(in, m_clientId);

  ok &= core::deserialize(in, m_shipDbId);
  ok &= core::deserialize(in, m_component);
  ok &= core::deserialize(in, m_weaponDbId);
  ok &= core::deserialize(in, m_active);

  return ok;
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

} // namespace bsgo
