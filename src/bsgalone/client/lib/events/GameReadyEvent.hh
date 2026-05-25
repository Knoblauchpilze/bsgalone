
#pragma once

#include "IUiEvent.hh"
#include <string>

namespace bsgalone::client {

class GameReadyEvent : public IUiEvent
{
  public:
  GameReadyEvent(const std::string &systemName);
  ~GameReadyEvent() override = default;

  auto getSystemName() const -> std::string;

  auto clone() const -> IUiEventPtr override;

  private:
  std::string m_systemName{};
};

} // namespace bsgalone::client
