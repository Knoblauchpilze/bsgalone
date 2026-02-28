
#pragma once

#include "IGameEvent.hh"
#include "IGameEventListener.hh"
#include <memory>

namespace bsgalone::core {

class ForProcessingEvents
{
  public:
  ForProcessingEvents()          = default;
  virtual ~ForProcessingEvents() = default;

  virtual void addListener(IGameEventListenerPtr listener) = 0;
  virtual bool empty()                                     = 0;
  virtual void processEvents()                             = 0;
};

using ForProcessingEventsShPtr = std::shared_ptr<ForProcessingEvents>;

} // namespace bsgalone::core
