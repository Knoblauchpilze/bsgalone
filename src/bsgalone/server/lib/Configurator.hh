
#pragma once

#include "ForManagingClient.hh"
#include "ForPublishingEvent.hh"
#include "IMessageListener.hh"
#include "Repositories.hh"
#include "SystemsManager.hh"

namespace bsgalone::server {

class Configurator
{
  public:
  Configurator()  = default;
  ~Configurator() = default;

  auto createSignupDrivingAdapter(ForPublishingEventShPtr publisher) const
    -> core::IMessageListenerPtr;

  auto createLoginDrivingAdapter(ForManagingClientShPtr clientManager,
                                 ForPublishingEventShPtr publisher) const
    -> core::IMessageListenerPtr;

  auto createLogoutDrivingAdapter(ForManagingClientShPtr clientManager,
                                  ForPublishingEventShPtr publisher) const
    -> core::IMessageListenerPtr;

  auto createUndockDrivingAdapter(SystemsManagerShPtr systemsManager,
                                  ForPublishingEventShPtr publisher) const
    -> core::IMessageListenerPtr;

  private:
  Repositories m_repositories{};
};
} // namespace bsgalone::server
