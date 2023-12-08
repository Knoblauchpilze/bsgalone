
#pragma once

#include "PurchaseService.hh"
#include "Repositories.hh"
#include "SignupService.hh"

namespace bsgo {

struct Services
{
  SignupServiceShPtr signup{};
  PurchaseServiceShPtr purchase{};
};

auto createServices(const Repositories &repositories) -> Services;

} // namespace bsgo
