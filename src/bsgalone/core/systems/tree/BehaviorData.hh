
#pragma once

#include "DataContext.hh"

namespace bsgo {

class Entity;
class Coordinator;

struct BehaviorData
{
  Entity &ent;
  Coordinator &coordinator;
  DataContext &context;
};

} // namespace bsgo
