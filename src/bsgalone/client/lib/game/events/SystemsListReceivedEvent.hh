
#pragma once

#include "IUiEvent.hh"
#include "System.hh"
#include <vector>

namespace bsgalone::client {

class SystemsListReceivedEvent : public IUiEvent
{
  public:
  SystemsListReceivedEvent(const std::vector<core::System> &systemsData);
  ~SystemsListReceivedEvent() override = default;

  auto getSystems() const -> const std::vector<core::System> &;

  auto clone() const -> IUiEventPtr override;

  private:
  std::vector<core::System> m_systems{};
};

} // namespace bsgalone::client
