

#pragma once

#include "AbstractComponent.hh"

namespace bsgo {

class NetworkComponent : public AbstractComponent
{
  public:
  NetworkComponent();
  ~NetworkComponent() override = default;

  void update(const float elapsedSeconds) override;
};

using NetworkComponentShPtr = std::shared_ptr<NetworkComponent>;

} // namespace bsgo
