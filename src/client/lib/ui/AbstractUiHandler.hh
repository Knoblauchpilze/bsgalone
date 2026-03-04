
#pragma once

#include "CoreObject.hh"
#include "IMessageQueue.hh"
#include "IUiHandler.hh"
#include "Views.hh"
#include <memory>

namespace pge {

class AbstractUiHandler : public bsgalone::client::IUiHandler, public core::CoreObject
{
  public:
  AbstractUiHandler(const std::string &name);
  ~AbstractUiHandler() override = default;

  virtual void connectToMessageQueue(bsgalone::core::IMessageQueue &messageQueue);
};

using AbstractUiHandlerPtr = std::unique_ptr<AbstractUiHandler>;

} // namespace pge
