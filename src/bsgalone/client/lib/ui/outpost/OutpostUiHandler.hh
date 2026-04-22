
#pragma once

#include "IDataStore.hh"
#include "IUiCommandQueue.hh"
#include "IUiEventQueue.hh"
#include "IUiHandler.hh"
#include "UiMenu.hh"

namespace bsgalone::client {

/// @brief - Forward declaration of the proxy class allowing the handler
/// to listen to UI events. This allows to make this class a friend class
/// of the handler.
class UiEventListenerOutpostProxy;

class OutpostUiHandler : public IUiHandler
{
  public:
  explicit OutpostUiHandler(IDataStoreShPtr dataStore,
                            IUiEventQueueShPtr inputQueue,
                            IUiCommandQueueShPtr outputQueue);
  ~OutpostUiHandler() override = default;

  void initializeMenus(const pge::Vec2i &dimensions,
                       pge::sprites::TexturePack &texturesLoader) override;
  bool processUserInput(ui::UserInputData &inputData) override;
  void render(pge::Renderer &engine) const override;
  void updateUi() override;

  private:
  IDataStoreShPtr m_dataStore{};

  enum class ActiveScreen
  {
    HANGAR,
    LOCKER,
    SHOP
  };
  ActiveScreen m_activeScreen{ActiveScreen::LOCKER};

  ui::UiMenuPtr m_tabsMenu{};

  IUiCommandQueueShPtr m_queue{};

  void registerToQueue(IUiEventQueueShPtr inputQueue);
  void generateTabsMenu(const pge::Vec2i &dimensions);

  void initializeTabsMenuOptions();

  void setActiveScreen(const ActiveScreen screen);

  void onLoadingFinished();

  friend class UiEventListenerOutpostProxy;
};

} // namespace bsgalone::client
