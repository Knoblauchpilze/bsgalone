
#include "OutpostListMessageConsumer.hh"
#include "OutpostDataSource.hh"
#include "OutpostListMessage.hh"

namespace bsgalone::client {

OutpostListMessageConsumer::OutpostListMessageConsumer(core::DatabaseEntityMapper &entityMapper,
                                                       core::CoordinatorShPtr coordinator)
  : AbstractGameMessageConsumer("outpost", core::MessageType::OUTPOST_LIST)
  , m_entityMapper(entityMapper)
  , m_coordinator(std::move(coordinator))
{}

void OutpostListMessageConsumer::onMessageReceivedInternal(const core::IMessage &message)
{
  const auto outpostsList = message.as<core::OutpostListMessage>();

  for (const auto &outpostData : outpostsList.getOutpostsData())
  {
    registerOutpost(outpostData);
  }
}

void OutpostListMessageConsumer::registerOutpost(const core::OutpostData &data) const
{
  core::OutpostDataSource source;
  source.registerOutpost(*m_coordinator, data, m_entityMapper);
}

} // namespace bsgalone::client
