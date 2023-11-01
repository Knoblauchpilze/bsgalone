
#pragma once

#include "Coordinator.hh"
#include "Entity.hh"
#include "Repositories.hh"
#include <core_utils/CoreObject.hh>
#include <memory>

namespace bsgo {

class IView : public utils::CoreObject
{
  public:
  IView(const std::string &name,
        const CoordinatorShPtr &coordinator,
        const Repositories &repositories);
  virtual ~IView() = default;

  protected:
  CoordinatorShPtr m_coordinator;
  Repositories m_repositories;
};

using IViewShPtr = std::shared_ptr<IView>;

} // namespace bsgo
