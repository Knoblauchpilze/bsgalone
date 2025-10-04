
#include "FireNode.hh"
#include "Entity.hh"

namespace bsgo {

FireNode::FireNode()
  : LeafNode("fire")
{}

void FireNode::run(const BehaviorData &data)
{
  for (const auto &weapon : data.ent.weapons)
  {
    if (!weapon->active())
    {
      weapon->toggle();
    }
    weapon->registerFireRequest();
  }
}

} // namespace bsgo
