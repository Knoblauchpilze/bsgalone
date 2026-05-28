
#pragma once

#include "IDataStore.hh"
#include "IUiEventQueue.hh"
#include "IUiHandler.hh"
#include "UiMenu.hh"

namespace bsgalone::client {

class UiEventListenerShopProxy;

class ShopUiHandler : public IUiHandler
{
  public:
  ShopUiHandler(IDataStoreShPtr dataStore, IUiEventQueueShPtr inputQueue);
  ~ShopUiHandler() override = default;

  void initializeMenus(const pge::Vec2i &dimensions,
                       pge::sprites::ITexturePack &texturesLoader) override;
  bool processUserInput(ui::UserInputData &inputData) override;
  void render(pge::Renderer &engine) const override;
  void updateUi() override;

  private:
  IDataStoreShPtr m_dataStore{};

  ui::UiMenuPtr m_resourcesMenu{};
  ui::UiMenuPtr m_menu{};

  void registerToQueue(IUiEventQueueShPtr inputQueue);

  void onShopReady();

  friend class UiEventListenerShopProxy;
};

} // namespace bsgalone::client
