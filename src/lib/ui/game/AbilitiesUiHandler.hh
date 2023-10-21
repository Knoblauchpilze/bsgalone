
#pragma once

#include "ComputerSlotComponent.hh"
#include "IUiHandler.hh"
#include "Views.hh"
#include <memory>

namespace pge {

class AbilitiesUiHandler : public IUiHandler
{
  public:
  AbilitiesUiHandler(const bsgo::Views &views);
  virtual ~AbilitiesUiHandler() = default;

  void initializeMenus(const int width, const int height) override;
  auto processUserInput(const controls::State &c, std::vector<ActionShPtr> &actions)
    -> menu::InputHandle override;
  void render(SpriteRenderer &engine) const override;
  void updateUi() override;

  private:
  bsgo::ShipViewShPtr m_shipView;

  std::vector<MenuShPtr> m_computers{};

  void generateCompmutersMenus(int width, int height);
  void updateComputerMenu(const bsgo::ComputerSlotComponent &computer,
                          const int id,
                          const bsgo::Entity &ship);
};

using AbilitiesUiHandlerPtr = std::unique_ptr<AbilitiesUiHandler>;

} // namespace pge
