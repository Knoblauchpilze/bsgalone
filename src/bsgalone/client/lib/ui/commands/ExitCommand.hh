
#pragma once

#include "IUiEvent.hh"
#include <string>

namespace bsgalone::client {

class ExitCommand : public IUiEvent
{
  public:
  ExitCommand();
  ~ExitCommand() override = default;

  auto clone() const -> IUiEventPtr override;
};

} // namespace bsgalone::client
