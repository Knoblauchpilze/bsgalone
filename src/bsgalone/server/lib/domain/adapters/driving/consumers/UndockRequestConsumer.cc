
#include "UndockRequestConsumer.hh"
#include "UndockRequest.hh"

namespace bsgalone::server {

UndockRequestConsumer::UndockRequestConsumer(ForExecutingUndockPtr undockUseCase)
  : ::core::CoreObject("undock")
  , m_useCase(std::move(undockUseCase))
{
  setService("consumer");

  if (nullptr == m_useCase)
  {
    throw std::invalid_argument("Expected non null undock use case");
  }
}

bool UndockRequestConsumer::isEventRelevant(const core::MessageType &type) const
{
  return type == core::MessageType::UNDOCK_REQUEST;
}

void UndockRequestConsumer::onEventReceived(const core::IMessage &message)
{
  const auto &request = message.as<core::UndockRequest>();

  UndockData data{
    .playerDbId = request.getPlayerDbId(),
  };

  auto success = withSafetyNet([this, &data]() { m_useCase->performUndock(data); }, "performUndock");
  if (!success)
  {
    warn("Failed to process undock request for player " + data.playerDbId.str());
  }
}

} // namespace bsgalone::server
