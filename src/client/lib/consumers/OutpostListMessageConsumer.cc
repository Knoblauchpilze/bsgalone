
#include "OutpostListMessageConsumer.hh"
#include "OutpostDataSource.hh"
#include "OutpostListMessage.hh"

namespace pge {

OutpostListMessageConsumer::OutpostListMessageConsumer(bsgo::DatabaseEntityMapper &entityMapper,
                                                       bsgo::CoordinatorShPtr coordinator)
  : AbstractGameMessageConsumer("outpost", bsgo::MessageType::OUTPOST_LIST)
  , m_entityMapper(entityMapper)
  , m_coordinator(std::move(coordinator))
{}

void OutpostListMessageConsumer::onMessageReceivedInternal(const bsgalone::core::IMessage &message)
{
  const auto outpostsList = message.as<bsgo::OutpostListMessage>();

  for (const auto &outpostData : outpostsList.getOutpostsData())
  {
    registerOutpost(outpostData);
  }
}

void OutpostListMessageConsumer::registerOutpost(const bsgo::OutpostData &data) const
{
  bsgo::OutpostDataSource source;
  source.registerOutpost(*m_coordinator, data, m_entityMapper);
}

} // namespace pge
