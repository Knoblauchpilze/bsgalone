
#include "HangarReadyEvent.hh"

namespace bsgalone::client {

HangarReadyEvent::HangarReadyEvent()
  : IUiEvent(UiEventType::HANGAR_READY)
{}

auto HangarReadyEvent::clone() const -> IUiEventPtr
{
  return std::make_unique<HangarReadyEvent>();
}

} // namespace bsgalone::client
