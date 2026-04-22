
#pragma once

#include "IDataStore.hh"
#include "IUiCommandQueue.hh"
#include "IUiHandler.hh"
#include "UiMenu.hh"

namespace bsgalone::client {

class OutpostUiHandler : public IUiHandler
{
  public:
  explicit OutpostUiHandler(IDataStoreShPtr dataStore, IUiCommandQueueShPtr outputQueue);
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

  void generateTabsMenu(const pge::Vec2i &dimensions);

  void initializeTabsMenuOptions();

  void setActiveScreen(const ActiveScreen screen);
};

} // namespace bsgalone::client
