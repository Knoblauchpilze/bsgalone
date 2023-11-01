
#pragma once

#include "IUiHandler.hh"
#include "Views.hh"
#include <memory>

namespace pge {

class ShopUiHandler : public IUiHandler
{
  public:
  ShopUiHandler(const bsgo::Views &views);
  virtual ~ShopUiHandler() = default;

  void initializeMenus(const int width, const int height) override;
  auto processUserInput(const controls::State &c, std::vector<ActionShPtr> &actions)
    -> menu::InputHandle override;
  void render(SpriteRenderer &engine) const override;
  void updateUi() override;

  private:
  MenuShPtr m_menu{};
};

using ShopUiHandlerPtr = std::unique_ptr<ShopUiHandler>;

} // namespace pge
