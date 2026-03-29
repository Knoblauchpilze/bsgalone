
#pragma once

#include "AccountRepository.hh"
#include "PlayerRepository.hh"

namespace bsgalone::core {

struct Repositories
{
  Repositories();
  ~Repositories() = default;

  AccountRepositoryShPtr accountRepository{};
  PlayerRepositoryShPtr playerRepository{};
};

} // namespace bsgalone::core
