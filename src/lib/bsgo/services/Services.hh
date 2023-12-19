
#pragma once

#include "LockerService.hh"
#include "PurchaseService.hh"
#include "Repositories.hh"
#include "SignupService.hh"

namespace bsgo {

struct Services
{
  SignupServiceShPtr signup{};
  PurchaseServiceShPtr purchase{};
  LockerServiceShPtr locker{};
};

auto createServices(const Repositories &repositories) -> Services;

} // namespace bsgo
