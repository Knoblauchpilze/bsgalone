
#pragma once

#include "AbstractEventProcessor.hh"
#include "IGameEvent.hh"

namespace bsgalone::core {

class GameEventProcessor : public messaging::AbstractEventProcessor<IGameEvent, GameEventType>
{
  public:
  GameEventProcessor(const std::string &onBehalfOfName,
                     std::deque<IGameEventPtr> &events,
                     std::mutex &locker,
                     EventHandler handler);

  protected:
  auto unimportantEventTypes() const -> std::unordered_set<GameEventType> override;
};

} // namespace bsgalone::core
