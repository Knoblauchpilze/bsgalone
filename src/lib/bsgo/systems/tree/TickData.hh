
#pragma once

namespace bsgo {

class Entity;
class Coordinator;

struct TickData
{
  Entity &ent;
  Coordinator &coordinator;
};

} // namespace bsgo
