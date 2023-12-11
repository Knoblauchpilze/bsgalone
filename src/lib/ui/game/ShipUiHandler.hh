
#pragma once

#include "IUiHandler.hh"
#include "UiTextMenu.hh"
#include <memory>

namespace pge {

using EntityFetcher = std::function<std::optional<bsgo::Entity>(const bsgo::ShipView &)>;
struct ShipUiConfig
{
  olc::vi2d offset{};
  bool displayDistance{false};
  EntityFetcher getEntity{};
};

class ShipUiHandler : public IUiHandler
{
  public:
  ShipUiHandler(const ShipUiConfig &config, const bsgo::Views &views);
  ~ShipUiHandler() override = default;

  void initializeMenus(const int width, const int height) override;
  bool processUserInput(UserInputData &inputData) override;
  void render(SpriteRenderer &engine) const override;
  void updateUi() override;

  private:
  ShipUiConfig m_config{};
  bsgo::ShipViewShPtr m_shipView{};

  enum MenuItem
  {
    NAME     = 0,
    HEALTH   = 1,
    POWER    = 2,
    DISTANCE = 3,

    COUNT = 4,
  };
  std::vector<UiTextMenuPtr> m_menus{};

  void updateNameComponent(const bsgo::Entity &ship);
  void updateHealthComponent(const bsgo::Entity &ship);
  void updatePowerComponent(const bsgo::Entity &ship);
  void updateDistanceComponent();
};

using ShipUiHandlerPtr = std::unique_ptr<ShipUiHandler>;

} // namespace pge
