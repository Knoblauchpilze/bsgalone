
#pragma once

#include "IUiCommand.hh"

namespace bsgalone::client {

class UndockCommand : public IUiCommand
{
  public:
  UndockCommand();
  ~UndockCommand() override = default;

  auto clone() const -> IUiCommandPtr override;
};

} // namespace bsgalone::client
