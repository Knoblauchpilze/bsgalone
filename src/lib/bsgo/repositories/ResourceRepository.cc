
#include "ResourceRepository.hh"

namespace bsgo {

ResourceRepository::ResourceRepository()
  : IRepository("resource")
{}

auto ResourceRepository::findOneById(const Uuid &resource) const -> Resource
{
  Resource out;

  switch (resource)
  {
    case Uuid{0}:
      out.name = "tylium";
      break;
    case Uuid{1}:
      out.name = "titane";
      break;
    default:
      error("Resource " + str(resource) + " not found");
      break;
  }

  return out;
}

} // namespace bsgo
