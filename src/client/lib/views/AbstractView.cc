
#include "AbstractView.hh"

namespace pge {

AbstractView::AbstractView(const std::string &name,
                           const std::unordered_set<bsgo::MessageType> &relevantMessageTypes)
  : bsgo::AbstractMessageConsumer(name, relevantMessageTypes)
  , IView()
{
  setService("view");
}

bool AbstractView::isReady() const noexcept
{
  return true;
}

void AbstractView::addListener(IViewListenerPtr listener)
{
  if (nullptr == listener)
  {
    throw std::invalid_argument("Expected a non null listener");
  }

  m_listeners.emplace_back(std::move(listener));
}

void AbstractView::onMessageReceived(const bsgalone::core::IMessage &message)
{
  withSafetyNet([this, &message]() { handleMessageInternal(message); }, "onMessageReceived");
  notifyListeners();
}

void AbstractView::notifyListeners()
{
  for (const auto &listener : m_listeners)
  {
    withSafetyNet([&listener]() { listener->onViewUpdated(); }, "onViewUpdated");
  }
}

void AbstractView::handleMessageInternal(const bsgalone::core::IMessage & /*message*/)
{
  // Default implementation should never be called. It is provided to conform
  // to the AbstractMessageConsumer interface and allow views which are not
  // listening to any messages to not have to reimplement this method.
  // In case a derived class DOES receive message and DOES NOT override this
  // method, an error will be raised, prompting to provide an override.
  error("Please define an overload for this method if you want to receive messages");
}

} // namespace pge
