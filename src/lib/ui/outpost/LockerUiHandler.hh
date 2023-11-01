
#pragma once

#include "IUiHandler.hh"
#include "Views.hh"
#include <memory>

namespace pge {

class LockerUiHandler : public IUiHandler
{
  public:
  LockerUiHandler(const bsgo::Views &views);
  virtual ~LockerUiHandler() = default;

  void initializeMenus(const int width, const int height) override;
  auto processUserInput(const controls::State &c, std::vector<ActionShPtr> &actions)
    -> menu::InputHandle override;
  void render(SpriteRenderer &engine) const override;
  void updateUi() override;

  private:
  MenuShPtr m_menu{};
};

using LockerUiHandlerPtr = std::unique_ptr<LockerUiHandler>;

} // namespace pge
