
#include "Uuid.hh"
#include "SerializationUtils.hh"

namespace bsgalone::core {

Uuid::Uuid(uuids::uuid uuid)
  : m_uuid(std::move(uuid))
{}

auto Uuid::str() const -> std::string
{
  return uuids::to_string(m_uuid);
}

auto Uuid::toDbId() const -> std::string
{
  return str();
}

auto Uuid::random() -> Uuid
{
  // https://github.com/mariusbancila/stduuid?tab=readme-ov-file#using-the-library
  std::mt19937 device;
  uuids::uuid_random_generator generator{device};
  return Uuid(generator());
}

auto Uuid::fromDbId(const std::string_view dbId) -> std::optional<Uuid>
{
  const auto maybeUuid = uuids::uuid::from_string(dbId);
  if (maybeUuid)
  {
    return Uuid(*maybeUuid);
  }

  return {};
}

auto Uuid::readFromStream(std::istream &in) -> std::optional<Uuid>
{
  Uuid uuid;

  bool ok{true};
  ok &= ::core::deserialize(in, uuid.m_uuid);

  if (!ok)
  {
    return {};
  }

  return uuid;
}

auto operator<<(std::ostream &out, const Uuid &uuid) -> std::ostream &
{
  ::core::serialize(out, uuid.m_uuid);

  return out;
}

bool Uuid::operator==(const Uuid &rhs) const
{
  return m_uuid == rhs.m_uuid;
}

} // namespace bsgalone::core
