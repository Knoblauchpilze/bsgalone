
#pragma once

#include "AbstractGameMessageConsumer.hh"
#include "AsteroidData.hh"
#include "Coordinator.hh"
#include "DatabaseEntityMapper.hh"
#include "Uuid.hh"

namespace bsgalone::client {

class AsteroidListMessageConsumer : public AbstractGameMessageConsumer
{
  public:
  AsteroidListMessageConsumer(core::DatabaseEntityMapper &entityMapper,
                              core::CoordinatorShPtr coordinator);
  ~AsteroidListMessageConsumer() override = default;

  protected:
  void onMessageReceivedInternal(const core::IMessage &message) override;

  private:
  core::DatabaseEntityMapper &m_entityMapper;
  core::CoordinatorShPtr m_coordinator{};

  /// @brief -
  bool m_relevantLoadingTransitionDetected{false};

  void registerAsteroid(const core::AsteroidData &data) const;
};

} // namespace bsgalone::client
