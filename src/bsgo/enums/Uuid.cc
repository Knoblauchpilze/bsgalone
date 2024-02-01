
#include "Uuid.hh"

namespace bsgo {

auto str(const Uuid id) -> std::string
{
  return std::to_string(id);
}

auto toDbId(const Uuid id) -> int
{
  return static_cast<int>(id) + 1u;
}

auto fromDbId(const int dbId) -> Uuid
{
  return Uuid{dbId - 1};
}

} // namespace bsgo
