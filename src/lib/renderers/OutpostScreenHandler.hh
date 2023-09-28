
#pragma once

#include "IScreenHandler.hh"
#include "Menu.hh"
#include "Views.hh"

namespace pge {

class OutpostScreenHandler : public IScreenHandler
{
  public:
  OutpostScreenHandler();
  ~OutpostScreenHandler() override = default;

  void loadResources(int width, int height, sprites::TexturePack &texturesLoader) override;
  void render(SpriteRenderer &engine,
              const RenderState &state,
              const RenderingPass pass) const override;
  auto processUserInput(const controls::State &c, std::vector<ActionShPtr> &actions)
    -> menu::InputHandle override;
  void updateUi() override;

  private:
  MenuShPtr m_menu{nullptr};
};

} // namespace pge