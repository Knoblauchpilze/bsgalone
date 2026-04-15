
#pragma once

#include "AccountRepository.hh"
#include "PlayerRepository.hh"
#include "SystemRepository.hh"

namespace bsgalone::server {

struct Repositories
{
  Repositories();
  ~Repositories() = default;

  AccountRepositoryShPtr accountRepository{};
  PlayerRepositoryShPtr playerRepository{};
  SystemRepositoryShPtr systemRepository{};
};

} // namespace bsgalone::server
