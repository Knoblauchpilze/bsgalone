
#pragma once

#include "AccountRepository.hh"
#include "PlayerRepository.hh"

namespace bsgalone::server {

struct Repositories
{
  Repositories();
  ~Repositories() = default;

  AccountRepositoryShPtr accountRepository{};
  PlayerRepositoryShPtr playerRepository{};
};

} // namespace bsgalone::server
