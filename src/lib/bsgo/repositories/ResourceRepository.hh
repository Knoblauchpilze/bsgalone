
#pragma once

#include "IRepository.hh"
#include "Uuid.hh"
#include <core_utils/TimeUtils.hh>
#include <memory>

namespace bsgo {

struct Resource
{
  std::string name;
};

class ResourceRepository : public IRepository
{
  public:
  ResourceRepository();
  ~ResourceRepository() override = default;

  auto findOneById(const Uuid &resource) const -> Resource;
};

using ResourceRepositoryShPtr = std::shared_ptr<ResourceRepository>;

} // namespace bsgo
