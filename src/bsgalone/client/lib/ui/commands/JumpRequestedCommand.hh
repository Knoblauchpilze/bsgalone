
#pragma once

#include "IUiEvent.hh"

namespace bsgalone::client {

class JumpRequestedCommand : public IUiEvent
{
  public:
  JumpRequestedCommand();
  ~JumpRequestedCommand() override = default;

  auto clone() const -> IUiEventPtr override;
};

} // namespace bsgalone::client
