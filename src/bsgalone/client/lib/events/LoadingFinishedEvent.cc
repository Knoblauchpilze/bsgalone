
#include "LoadingFinishedEvent.hh"

namespace bsgalone::client {

LoadingFinishedEvent::LoadingFinishedEvent()
  : IUiEvent(UiEventType::LOADING_FINISHED)
{}

auto LoadingFinishedEvent::clone() const -> IUiEventPtr
{
  return std::make_unique<LoadingFinishedEvent>();
}

} // namespace bsgalone::client
