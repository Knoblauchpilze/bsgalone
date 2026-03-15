
#pragma once

#include "IUiEvent.hh"
#include "Uuid.hh"

namespace bsgalone::client {

class SystemSelectedCommand : public IUiEvent
{
  public:
  SystemSelectedCommand(const core::Uuid systemDbId);
  ~SystemSelectedCommand() override = default;

  auto getSystemDbId() const -> core::Uuid;

  auto clone() const -> IUiEventPtr override;

  private:
  core::Uuid m_systemDbId{};
};

} // namespace bsgalone::client
