
#pragma once

#include "IUiCommand.hh"
#include <string>

namespace bsgalone::client {

class ExitCommand : public IUiCommand
{
  public:
  ExitCommand();
  ~ExitCommand() override = default;

  auto clone() const -> IUiCommandPtr override;
};

} // namespace bsgalone::client
