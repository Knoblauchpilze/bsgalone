
#pragma once

#include "Coordinator.hh"
#include "DatabaseEntityMapper.hh"
#include "JumpService.hh"
#include "LockerService.hh"
#include "PurchaseService.hh"
#include "Repositories.hh"
#include "ShipService.hh"
#include "SlotService.hh"

namespace bsgo {

struct Services
{
  PurchaseServiceShPtr purchase{};
  LockerServiceShPtr locker{};
  ShipServiceShPtr ship{};
  JumpServiceShPtr jump{};
  SlotServiceShPtr slot{};
};

auto createServices(const Repositories &repositories,
                    const CoordinatorShPtr &coordinator,
                    const DatabaseEntityMapper &entityMapper) -> Services;

} // namespace bsgo
