
#pragma once

#include "Coordinator.hh"
#include "DatabaseEntityMapper.hh"
#include "EntityService.hh"
#include "JumpService.hh"
#include "LoadingService.hh"
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
  EntityServiceShPtr entity{};
  LoadingServiceShPtr loading{};
};

auto createServices(const Repositories &repositories,
                    const CoordinatorShPtr &coordinator,
                    DatabaseEntityMapper &entityMapper) -> Services;

} // namespace bsgo
