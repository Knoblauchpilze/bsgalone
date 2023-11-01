
#pragma once

#include "IRepository.hh"
#include "Uuid.hh"
#include <memory>

namespace bsgo {

class PlayerComputerRepository : public IRepository
{
  public:
  PlayerComputerRepository();
  ~PlayerComputerRepository() override = default;

  auto findAllById(const Uuid &player) const -> std::vector<Uuid>;
};

using PlayerComputerRepositoryShPtr = std::shared_ptr<PlayerComputerRepository>;

} // namespace bsgo
