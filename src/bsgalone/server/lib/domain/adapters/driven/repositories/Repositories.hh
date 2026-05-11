
#pragma once

#include "AccountRepository.hh"
#include "AsteroidRepository.hh"
#include "PlayerRepository.hh"
#include "SystemRepository.hh"

namespace bsgalone::server {

struct Repositories
{
  Repositories();
  ~Repositories() = default;

  AccountRepositoryShPtr accountRepository{};
  AsteroidRepositoryShPtr asteroidRepository{};
  PlayerRepositoryShPtr playerRepository{};
  SystemRepositoryShPtr systemRepository{};
};

} // namespace bsgalone::server
