
#pragma once

#include "Coordinator.hh"
#include "JumpService.hh"
#include "LockerService.hh"
#include "LoginService.hh"
#include "PurchaseService.hh"
#include "Repositories.hh"
#include "ShipService.hh"
#include "SignupService.hh"
#include "SlotService.hh"

namespace bsgo {

struct Services
{
  SignupServiceShPtr signup{};
  LoginServiceShPtr login{};
  PurchaseServiceShPtr purchase{};
  LockerServiceShPtr locker{};
  ShipServiceShPtr ship{};
  JumpServiceShPtr jump{};
  SlotServiceShPtr slot{};
};

auto createServices(const Repositories &repositories, const CoordinatorShPtr &coordinator)
  -> Services;

} // namespace bsgo
