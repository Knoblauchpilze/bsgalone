
#include "Views.hh"
#include "ViewConsumerProxy.hh"

namespace bsgalone::client {

auto createViews(const ViewsConfig &config, const core::DatabaseEntityMapper &entityMapper) -> Views
{
  Views out{};

  out.gameView = std::make_shared<GameView>(config.gameSession,
                                            config.coordinator,
                                            entityMapper,
                                            config.internalMessageQueue,
                                            config.outputMessageQueue);

  return out;
}

void Views::connectToQueue(core::IMessageQueue *const queue)
{
  queue->addListener(std::make_unique<ViewConsumerProxy>(*gameView));
}

void Views::reset()
{
  gameView->reset();
}

} // namespace bsgalone::client
