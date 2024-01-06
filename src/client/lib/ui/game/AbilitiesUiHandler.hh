
#pragma once

#include "ComputerSlotComponent.hh"
#include "IUiHandler.hh"
#include "UiMenu.hh"
#include "UiTextMenu.hh"
#include "Views.hh"
#include <memory>

namespace pge {

class AbilitiesUiHandler : public IUiHandler
{
  public:
  AbilitiesUiHandler(const bsgo::Views &views);
  ~AbilitiesUiHandler() override = default;

  void initializeMenus(const int width, const int height) override;
  bool processUserInput(UserInputData &inputData) override;
  void render(SpriteRenderer &engine) const override;
  void updateUi() override;
  void reset() override;

  private:
  bsgo::ShipViewShPtr m_shipView;
  bool m_initialized{false};

  std::vector<UiMenuPtr> m_computers{};
  std::vector<UiTextMenu *> m_ranges{};
  std::vector<UiTextMenu *> m_damages{};
  std::vector<UiTextMenu *> m_statuses{};

  void generateComputersMenus(int width, int height);
  void initializeAbilities();
  void updateComputerMenu(const bsgo::ComputerSlotComponent &computer, const int id);
};

using AbilitiesUiHandlerPtr = std::unique_ptr<AbilitiesUiHandler>;

} // namespace pge
