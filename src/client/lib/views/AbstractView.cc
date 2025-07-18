
#include "AbstractView.hh"

namespace pge {

AbstractView::AbstractView(const std::string &name)
  : AbstractView(name, {})
{}

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

void AbstractView::onMessageReceived(const bsgo::IMessage & /*message*/)
{
  // Default implementation should never be called. It is provided to conform
  // to the AbstractMessageConsumer interface but if an inheriting class does
  // receive some messages, it should also overload this one.
  // Therefore we raise an error here.
  error("Please define an overload for this method if you want to receive messages");
}

} // namespace pge
