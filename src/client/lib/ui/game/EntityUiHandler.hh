
#pragma once

#include "IUiHandler.hh"
#include "UiTextMenu.hh"
#include <memory>

namespace pge {

using EntityFetcher = std::function<std::optional<bsgo::Entity>(const ShipView &)>;
struct EntityUiConfig
{
  Vec2i offset{};
  bool displayDistance{false};
  EntityFetcher getEntity{};
};

class EntityUiHandler : public IUiHandler
{
  public:
  EntityUiHandler(const EntityUiConfig &config, const Views &views);
  ~EntityUiHandler() override = default;

  void initializeMenus(const int width,
                       const int height,
                       sprites::TexturePack &texturesLoader) override;
  bool processUserInput(UserInputData &inputData) override;
  void render(Renderer &engine) const override;
  void updateUi() override;

  private:
  EntityUiConfig m_config{};
  ShipViewShPtr m_shipView{};
  ShipDbViewShPtr m_shipDbView{};

  enum MenuItem
  {
    NAME     = 0,
    HEALTH   = 1,
    POWER    = 2,
    DISTANCE = 3,

    DOCK = 4,

    COUNT = 5,
  };
  std::vector<UiTextMenuPtr> m_menus{};

  void subscribeToViews();
  void updateNameComponent(const bsgo::Entity &entity);
  void updateHealthComponent(const bsgo::Entity &entity);
  void updatePowerComponent(const bsgo::Entity &entity);
  void updateDistanceComponent();
  void updateDockComponent(const bsgo::Entity &entity);
};

using EntityUiHandlerPtr = std::unique_ptr<EntityUiHandler>;

} // namespace pge
