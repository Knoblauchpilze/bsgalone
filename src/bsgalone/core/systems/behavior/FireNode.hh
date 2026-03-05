
#pragma once

#include "LeafNode.hh"

namespace bsgo {

class FireNode : public LeafNode
{
  public:
  FireNode();
  ~FireNode() override = default;

  protected:
  void run(const BehaviorData &data) override;

  private:
  /// @brief - Enumeration describing the possible state of the firing for the
  /// entity. This allows to distinguish the possible reasons why the node can
  /// not continue firing on the entity.
  /// It is used to direct the final state of this node, between failed and
  /// succeeded.
  enum class FiringState
  {
    NO_TARGET,
    TARGET_REACHABLE,
    TARGET_IS_DEAD,
    TARGET_DOES_NOT_EXIST,
    TARGET_IS_TOO_FAR
  };

  void setFiringState();
  auto determineFiringState(const BehaviorData &data) const -> FiringState;
  void fireAllWeapons(Entity &ent);
};

} // namespace bsgo
