
#pragma once

#include "AbstractGameMessageConsumer.hh"
#include "Coordinator.hh"
#include "DatabaseEntityMapper.hh"
#include "OutpostData.hh"

namespace bsgalone::client {

class OutpostListMessageConsumer : public AbstractGameMessageConsumer
{
  public:
  OutpostListMessageConsumer(core::DatabaseEntityMapper &entityMapper,
                             core::CoordinatorShPtr coordinator);
  ~OutpostListMessageConsumer() override = default;

  protected:
  void onMessageReceivedInternal(const core::IMessage &message) override;

  private:
  core::DatabaseEntityMapper &m_entityMapper;
  core::CoordinatorShPtr m_coordinator{};

  void registerOutpost(const core::OutpostData &data) const;
};

} // namespace bsgalone::client
