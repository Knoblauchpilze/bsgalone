

#pragma once

#include "IComponent.hh"

namespace bsgo {

class NetworkComponent : public IComponent
{
  public:
  NetworkComponent();
  ~NetworkComponent() override = default;

  void update(const float elapsedSeconds) override;
};

using NetworkComponentShPtr = std::shared_ptr<NetworkComponent>;

} // namespace bsgo
