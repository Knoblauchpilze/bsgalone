
#pragma once

#include "AbstractComponent.hh"
#include "INode.hh"

namespace bsgo {

class AiComponent : public AbstractComponent
{
  public:
  AiComponent(INodePtr behavior);
  ~AiComponent() override = default;

  auto behavior() -> INode &;
  auto behavior() const -> const INode &;

  void update(const chrono::TickData &data) override;

  private:
  INodePtr m_behavior;
};

using AiComponentShPtr = std::shared_ptr<AiComponent>;

} // namespace bsgo
