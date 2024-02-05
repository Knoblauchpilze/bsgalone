
#pragma once

#include "AbstractView.hh"
#include "Repositories.hh"
#include "Uuid.hh"
#include <memory>

namespace bsgo {

class ResourceView : public AbstractView
{
  public:
  ResourceView(const Repositories &repositories);
  ~ResourceView() override = default;

  auto getResourceName(const Uuid resource) const -> std::string;

  private:
  Repositories m_repositories{};
};

using ResourceViewShPtr = std::shared_ptr<ResourceView>;

} // namespace bsgo
