
#pragma once

#include "Coordinator.hh"
#include "Entity.hh"
#include <core_utils/CoreObject.hh>
#include <memory>

namespace bsgo {

class IView : public utils::CoreObject
{
  public:
  IView(const std::string &name, const CoordinatorShPtr &coordinator);
  virtual ~IView() = default;

  auto getEntity(const Uuid &ent) const -> Entity;

  protected:
  CoordinatorShPtr m_coordinator;
};

using IViewShPtr = std::shared_ptr<IView>;

} // namespace bsgo
