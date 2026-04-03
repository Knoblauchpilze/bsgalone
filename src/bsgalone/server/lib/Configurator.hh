
#pragma once

#include "ForPublishingEvent.hh"
#include "IMessageListener.hh"
#include "Repositories.hh"

namespace bsgalone::server {

class Configurator
{
  public:
  Configurator()  = default;
  ~Configurator() = default;

  auto createSignupDrivingAdapter(core::ForPublishingEventShPtr publisher) const
    -> core::IMessageListenerPtr;

  auto createLoginDrivingAdapter(core::ForPublishingEventShPtr publisher) const
    -> core::IMessageListenerPtr;

  private:
  core::Repositories m_repositories{};
};
} // namespace bsgalone::server
