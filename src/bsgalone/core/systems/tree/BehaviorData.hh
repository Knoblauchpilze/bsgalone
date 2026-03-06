
#pragma once

#include "DataContext.hh"

namespace bsgalone::core {

class Entity;
class Coordinator;

struct BehaviorData
{
  Entity &ent;
  Coordinator &coordinator;
  DataContext &context;
};

} // namespace bsgalone::core
