
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

  void addListener(IViewListenerPtr listener) override;

  void onMessageReceived(const bsgo::IMessage &message) override;

  protected:
  /// @brief - Used to notify all currently registered listeners of an update of the
  /// view, using the interface method `onViewUpdated`.
  void notifyListeners();

  private:
  std::vector<IViewListenerPtr> m_listeners{};
};

} // namespace pge
