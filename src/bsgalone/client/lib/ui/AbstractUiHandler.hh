
#pragma once

#include "CoreObject.hh"
#include "IMessageQueue.hh"
#include "IUiHandler.hh"
#include "Views.hh"
#include <memory>

namespace bsgalone::client {

class AbstractUiHandler : public bsgalone::client::IUiHandler, public ::core::CoreObject
{
  public:
  AbstractUiHandler(const std::string &name);
  ~AbstractUiHandler() override = default;

  virtual void connectToMessageQueue(core::IMessageQueue &messageQueue);
};

using AbstractUiHandlerPtr = std::unique_ptr<AbstractUiHandler>;

} // namespace bsgalone::client
