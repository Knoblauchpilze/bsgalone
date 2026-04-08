
#pragma once

#include "IUiCommand.hh"

namespace bsgalone::client {

class LogoutCommand : public IUiCommand
{
  public:
  LogoutCommand();
  ~LogoutCommand() override = default;

  auto clone() const -> IUiCommandPtr override;
};

} // namespace bsgalone::client
