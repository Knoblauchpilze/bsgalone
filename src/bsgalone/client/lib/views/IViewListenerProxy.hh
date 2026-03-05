
#pragma once

#include "IViewListener.hh"
#include <functional>

namespace pge {

class IViewListenerProxy : public IViewListener
{
  public:
  using OnUpdatedConsumer = std::function<void()>;

  IViewListenerProxy(const OnUpdatedConsumer &listener);
  ~IViewListenerProxy() override = default;

  void onViewUpdated() override;

  private:
  OnUpdatedConsumer m_listener{};
};

} // namespace pge
