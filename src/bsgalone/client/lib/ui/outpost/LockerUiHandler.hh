
#pragma once

#include "IDataStore.hh"
#include "IUiEventQueue.hh"
#include "IUiHandler.hh"
#include "UiMenu.hh"

namespace bsgalone::client {

class UiEventListenerLockerProxy;

class LockerUiHandler : public IUiHandler
{
  public:
  LockerUiHandler(IDataStoreShPtr dataStore, IUiEventQueueShPtr inputQueue);
  ~LockerUiHandler() override = default;

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

  void onLockerReady();

  friend class UiEventListenerLockerProxy;
};

} // namespace bsgalone::client
