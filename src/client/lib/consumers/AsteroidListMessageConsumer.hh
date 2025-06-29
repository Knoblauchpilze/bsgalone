
#pragma once

#include "AbstractMessageConsumer.hh"
#include "AsteroidData.hh"
#include "Coordinator.hh"
#include "DatabaseEntityMapper.hh"
#include "Uuid.hh"

namespace pge {

class AsteroidListMessageConsumer : public bsgo::AbstractMessageConsumer
{
  public:
  AsteroidListMessageConsumer(bsgo::DatabaseEntityMapper &entityMapper,
                              bsgo::CoordinatorShPtr coordinator);
  ~AsteroidListMessageConsumer() override = default;

  void onMessageReceived(const bsgo::IMessage &message) override;

  private:
  bsgo::DatabaseEntityMapper &m_entityMapper;
  bsgo::CoordinatorShPtr m_coordinator{};

  void registerAsteroid(const bsgo::AsteroidData &data) const;
};

} // namespace pge
