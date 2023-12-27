
#pragma once

#include "IView.hh"
#include <memory>

namespace bsgo {

class ResourceView : public IView
{
  public:
  ResourceView(const CoordinatorShPtr &coordinator, const Repositories &repositories);
  ~ResourceView() override = default;

  auto getResourceName(const Uuid &resource) const -> std::string;
};

using ResourceViewShPtr = std::shared_ptr<ResourceView>;

} // namespace bsgo
