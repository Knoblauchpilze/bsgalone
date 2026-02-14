
#pragma once

#include "AbstractGameMessageConsumer.hh"
#include "AsteroidData.hh"
#include "Coordinator.hh"
#include "DatabaseEntityMapper.hh"
#include "Uuid.hh"

namespace pge {

class AsteroidListMessageConsumer : public AbstractGameMessageConsumer
{
  public:
  AsteroidListMessageConsumer(bsgo::DatabaseEntityMapper &entityMapper,
                              bsgo::CoordinatorShPtr coordinator);
  ~AsteroidListMessageConsumer() override = default;

  protected:
  void onMessageReceivedInternal(const bsgalone::core::IMessage &message) override;

  private:
  bsgo::DatabaseEntityMapper &m_entityMapper;
  bsgo::CoordinatorShPtr m_coordinator{};

  /// @brief -
  bool m_relevantLoadingTransitionDetected{false};

  void registerAsteroid(const bsgo::AsteroidData &data) const;
};

} // namespace pge
