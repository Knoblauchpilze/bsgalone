
#pragma once

#include "AbstractMessageConsumer.hh"
#include "IView.hh"
#include <string>

namespace pge {

class AbstractView : public bsgo::AbstractMessageConsumer, public IView
{
  public:
  AbstractView(const std::string &name);
  AbstractView(const std::string &name,
               const std::unordered_set<bsgo::MessageType> &relevantMessageTypes);
  ~AbstractView() override = default;

  bool isReady() const noexcept override;

  void onMessageReceived(const bsgo::IMessage &message) override;
};

} // namespace pge
