
#pragma once

#include "AbstractUiHandler.hh"
#include "UiTextMenu.hh"
#include <memory>

namespace bsgalone::client {

using EntityFetcher = std::function<std::optional<core::Entity>(const GameView &)>;
struct EntityUiConfig
{
  pge::Vec2i offset{};
  bool displayDistance{false};
  EntityFetcher getEntity{};
};

class EntityUiHandler : public AbstractUiHandler
{
  public:
  EntityUiHandler(const EntityUiConfig &config, const Views &views);
  ~EntityUiHandler() override = default;

  void initializeMenus(const int width,
                       const int height,
                       pge::sprites::TexturePack &texturesLoader) override;
  bool processUserInput(ui::UserInputData &inputData) override;
  void render(pge::Renderer &engine) const override;
  void updateUi() override;

  private:
  EntityUiConfig m_config{};
  GameViewShPtr m_gameView{};

  enum MenuItem
  {
    NAME     = 0,
    HEALTH   = 1,
    POWER    = 2,
    DISTANCE = 3,

    DOCK = 4,

    COUNT = 5,
  };
  std::vector<ui::UiTextMenuPtr> m_menus{};

  void subscribeToViews();
  void updateNameComponent(const core::Entity &entity);
  void updateHealthComponent(const core::Entity &entity);
  void updatePowerComponent(const core::Entity &entity);
  void updateDistanceComponent();
  void updateDockComponent(const core::Entity &entity);
};

using EntityUiHandlerPtr = std::unique_ptr<EntityUiHandler>;

} // namespace bsgalone::client
