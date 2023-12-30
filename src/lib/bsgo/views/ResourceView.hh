
#pragma once

#include "AbstractView.hh"
#include <memory>

namespace bsgo {

class ResourceView : public AbstractView
{
  public:
  ResourceView(const CoordinatorShPtr &coordinator,
               const Repositories &repositories,
               const IMessageQueue *messageQueue);
  ~ResourceView() override = default;

  auto getResourceName(const Uuid &resource) const -> std::string;
};

using ResourceViewShPtr = std::shared_ptr<ResourceView>;

} // namespace bsgo
