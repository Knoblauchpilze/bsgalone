
#pragma once

#include "CoordinateFrame.hh"
#include "Entity.hh"
#include "EntityKind.hh"
#include "SystemView.hh"
#include <vector>

namespace bsgo {

auto getEntitiesWithinViewport(const SystemView &system,
                               const pge::CoordinateFrame &frame,
                               const EntityKind &filter) -> std::vector<Entity>;

}
