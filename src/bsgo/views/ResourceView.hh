
#pragma once

#include "AbstractView.hh"
#include <memory>

namespace bsgo {

class ResourceView : public AbstractView
{
  public:
  ResourceView(const CoordinatorShPtr &coordinator,
               const Repositories &repositories,
               IMessageQueue *const messageQueue);
  ~ResourceView() override = default;

  auto getResourceName(const Uuid &resource) const -> std::string;
};

using ResourceViewShPtr = std::shared_ptr<ResourceView>;

} // namespace bsgo
