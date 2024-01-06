
#pragma once

#include "AbstractComponent.hh"
#include "INode.hh"

namespace bsgo {

class AIComponent : public AbstractComponent
{
  public:
  AIComponent(INodePtr behavior);
  ~AIComponent() override = default;

  auto behavior() -> INode &;
  auto behavior() const -> const INode &;

  void update(const float elapsedSeconds) override;

  private:
  INodePtr m_behavior;
};

using AIComponentShPtr = std::shared_ptr<AIComponent>;

} // namespace bsgo
