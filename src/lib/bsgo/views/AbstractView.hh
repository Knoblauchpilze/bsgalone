
#pragma once

#include "Coordinator.hh"
#include "Entity.hh"
#include "IMessageQueue.hh"
#include "IView.hh"
#include "Repositories.hh"
#include <core_utils/CoreObject.hh>
#include <memory>

namespace bsgo {

class AbstractView : public utils::CoreObject, public IView
{
  public:
  AbstractView(const std::string &name,
               const CoordinatorShPtr &coordinator,
               const Repositories &repositories,
               IMessageQueue *const messageQueue);
  ~AbstractView() override = default;

  bool isReady() const noexcept override;

  protected:
  CoordinatorShPtr m_coordinator{};
  Repositories m_repositories{};
  IMessageQueue *const m_messageQueue{};
};

} // namespace bsgo
