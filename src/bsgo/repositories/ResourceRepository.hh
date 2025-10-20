
#pragma once

#include "AbstractRepository.hh"
#include "Uuid.hh"
#include <memory>
#include <unordered_set>

namespace bsgo {

struct Resource
{
  Uuid id{};
  std::string name{};
};

class ResourceRepository : public AbstractRepository
{
  public:
  ResourceRepository(const DbConnectionShPtr &connection);
  ~ResourceRepository() override = default;

  void initialize() override;

  auto findAll() const -> std::unordered_set<Uuid>;
  auto findOneById(const Uuid resource) const -> Resource;
  auto findOneByName(const std::string &name) const -> std::optional<Resource>;
};

using ResourceRepositoryShPtr = std::shared_ptr<ResourceRepository>;

} // namespace bsgo
