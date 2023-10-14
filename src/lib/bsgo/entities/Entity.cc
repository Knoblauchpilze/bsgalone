
#include "Entity.hh"

namespace bsgo {
namespace details {
template<typename Component>
inline bool checkComponentExists(const std::optional<std::shared_ptr<Component>> &comp)
{
  return comp && (nullptr != *comp);
}

template<typename Component>
inline void checkAccessIsSafe(const std::optional<std::shared_ptr<Component>> &comp,
                              const Entity &entity,
                              const std::string &compName)
{
  if (!comp)
  {
    throw std::invalid_argument("Expected entity " + bsgo::str(entity.uuid) + "/"
                                + bsgo::str(entity.kind) + " to have a " + compName + " component");
  }
  if (nullptr == *comp)
  {
    throw std::invalid_argument("Expected entity " + bsgo::str(entity.uuid) + "/"
                                + bsgo::str(entity.kind) + " to have a valid " + compName
                                + " component");
  }
}

template<typename Component>
inline auto safeConstAccess(const std::optional<std::shared_ptr<Component>> &comp,
                            const Entity &entity,
                            const std::string &compName) -> const Component &
{
  checkAccessIsSafe(comp, entity, compName);
  return **comp;
}

template<typename Component>
inline auto safeAccess(const std::optional<std::shared_ptr<Component>> &comp,
                       const Entity &entity,
                       const std::string &compName) -> Component &
{
  checkAccessIsSafe(comp, entity, compName);
  return **comp;
}
} // namespace details

auto Entity::str() const noexcept -> std::string
{
  std::string out("[");

  out += std::to_string(uuid);
  out += ",";
  out += bsgo::str(kind);

  if (transform)
  {
    const auto p = (*transform)->position();
    out += ",";
    out += std::to_string(p(0));
    out += "x";
    out += std::to_string(p(1));
    out += "x";
    out += std::to_string(p(2));
  }

  out += "]";
  return out;
}

template<>
bool Entity::exists<Transform>() const
{
  return details::checkComponentExists(transform);
}

template<>
bool Entity::exists<Velocity>() const
{
  return details::checkComponentExists(velocity);
}

template<>
bool Entity::exists<Health>() const
{
  return details::checkComponentExists(health);
}

template<>
bool Entity::exists<Power>() const
{
  return details::checkComponentExists(power);
}

template<>
auto Entity::access<Transform>() const -> const Transform &
{
  return details::safeConstAccess(transform, *this, "Transform");
}

template<>
auto Entity::access<Velocity>() const -> const Velocity &
{
  return details::safeConstAccess(velocity, *this, "Velocity");
}

template<>
auto Entity::access<Health>() const -> const Health &
{
  return details::safeConstAccess(health, *this, "Health");
}

template<>
auto Entity::access<Power>() const -> const Power &
{
  return details::safeConstAccess(power, *this, "Power");
}

template<>
auto Entity::access<Velocity>() -> Velocity &
{
  return details::safeAccess(velocity, *this, "Velocity");
}

} // namespace bsgo
