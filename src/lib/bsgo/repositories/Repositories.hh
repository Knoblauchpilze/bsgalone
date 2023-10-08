
#pragma once

#include "AsteroidRepository.hh"
#include "SystemRepository.hh"

namespace bsgo {

struct Repositories
{
  AsteroidRepositoryShPtr asteroid{};
  SystemRepositoryShPtr system{};
};

} // namespace bsgo
