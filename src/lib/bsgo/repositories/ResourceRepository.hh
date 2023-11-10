
#pragma once

#include "AbstractRepository.hh"
#include "Uuid.hh"
#include <core_utils/TimeUtils.hh>
#include <memory>

namespace bsgo {

struct Resource
{
  std::string name;
};

class ResourceRepository : public AbstractRepository
{
  public:
  ResourceRepository(const DbConnectionShPtr &connection);
  ~ResourceRepository() override = default;

  auto findOneById(const Uuid &resource) const -> Resource;
};

using ResourceRepositoryShPtr = std::shared_ptr<ResourceRepository>;

} // namespace bsgo
