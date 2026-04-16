
#include "Uuid.hh"
#include <array>

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

auto Uuid::fromDbId(const std::string_view dbId) -> Uuid
{
  const auto maybeUuid = uuids::uuid::from_string(dbId);
  if (!maybeUuid.has_value())
  {
    throw std::invalid_argument("Unrecognized uuid " + std::string(dbId));
  }

  return Uuid(*maybeUuid);
}

auto Uuid::readFromStream(std::istream &in) -> std::optional<Uuid>
{
  std::array<uuids::uuid::value_type, 16> rawUuid{};
  in.read(reinterpret_cast<char *>(rawUuid.data()), rawUuid.size());
  if (!in.good())
  {
    return {};
  }

  return Uuid(uuids::uuid(rawUuid));
}

auto operator<<(std::ostream &out, const Uuid &uuid) -> std::ostream &
{
  const auto bytes = uuid.m_uuid.as_bytes();
  out.write(reinterpret_cast<const char *>(bytes.data()),
            static_cast<std::streamsize>(bytes.size()));

  return out;
}

auto operator>>(std::istream &in, Uuid &uuid) -> std::istream &
{
  const auto maybeUuid = Uuid::readFromStream(in);
  if (!maybeUuid)
  {
    in.setstate(std::ios::failbit);
    return in;
  }

  uuid = *maybeUuid;

  return in;
}

bool Uuid::operator==(const Uuid &rhs) const
{
  return m_uuid == rhs.m_uuid;
}

} // namespace bsgalone::core
