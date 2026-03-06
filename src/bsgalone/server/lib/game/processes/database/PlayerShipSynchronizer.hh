
#pragma once

#include "Entity.hh"
#include "Repositories.hh"

namespace bsgalone::server {

void syncPlayerShip(const core::Entity &entity, const core::Repositories &repositories);

} // namespace bsgalone::server
