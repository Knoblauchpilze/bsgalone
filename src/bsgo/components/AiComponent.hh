
#pragma once

#include "AbstractComponent.hh"
#include "DataContext.hh"
#include "INode.hh"

namespace bsgo {

class AiComponent : public AbstractComponent
{
  public:
  AiComponent(INodePtr behavior, DataContext context);
  ~AiComponent() override = default;

  auto behavior() -> INode &;
  auto behavior() const -> const INode &;

  auto dataContext() -> DataContext &;

  void update(const chrono::TickData &data) override;

  private:
  INodePtr m_behavior;
  DataContext m_context{};
};

using AiComponentShPtr = std::shared_ptr<AiComponent>;

} // namespace bsgo
