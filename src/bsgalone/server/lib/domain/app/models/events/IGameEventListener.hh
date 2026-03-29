

#pragma once

#include "IGameEvent.hh"
#include <memory>

namespace bsgalone::core {

class IGameEventListener
{
  public:
  virtual ~IGameEventListener() = default;

  virtual bool isEventRelevant(const GameEventType &type) const = 0;
  virtual void onEventReceived(const IGameEvent &event)         = 0;
};

using IGameEventListenerPtr = std::unique_ptr<IGameEventListener>;

} // namespace bsgalone::core
