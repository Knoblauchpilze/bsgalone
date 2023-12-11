
#pragma once

#include "IUiHandler.hh"
#include "UiTextMenu.hh"
#include <memory>

namespace pge {

class StatusUiHandler : public IUiHandler
{
  public:
  StatusUiHandler(const olc::vi2d &offset, const bsgo::Views &views);
  ~StatusUiHandler() override = default;

  void initializeMenus(const int width, const int height) override;
  bool processUserInput(UserInputData &inputData) override;
  void render(SpriteRenderer &engine) const override;
  void updateUi() override;

  private:
  olc::vi2d m_offset{};
  bsgo::ShipViewShPtr m_shipView{};

  std::vector<UiTextMenuPtr> m_menus{};
};

using StatusUiHandlerPtr = std::unique_ptr<StatusUiHandler>;

} // namespace pge
