
#pragma once

#include "AbstractView.hh"
#include "Repositories.hh"
#include "Uuid.hh"
#include <memory>

namespace pge {

class ResourceView : public AbstractView
{
  public:
  ResourceView(const bsgo::Repositories &repositories);
  ~ResourceView() override = default;

  auto getResourceName(const bsgo::Uuid resource) const -> std::string;

  private:
  bsgo::Repositories m_repositories{};
};

using ResourceViewShPtr = std::shared_ptr<ResourceView>;

} // namespace pge
