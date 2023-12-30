
#pragma once

#include "JumpService.hh"
#include "LockerService.hh"
#include "PurchaseService.hh"
#include "Repositories.hh"
#include "ShipService.hh"
#include "SignupService.hh"

namespace bsgo {

struct Services
{
  SignupServiceShPtr signup{};
  PurchaseServiceShPtr purchase{};
  LockerServiceShPtr locker{};
  ShipServiceShPtr ship{};
  JumpServiceShPtr jump{};
};

auto createServices(const Repositories &repositories) -> Services;

} // namespace bsgo
