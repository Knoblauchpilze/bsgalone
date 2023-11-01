
#pragma once

#include "IUiHandler.hh"
#include "Views.hh"
#include <memory>

namespace pge {

class HangarUiHandler : public IUiHandler
{
  public:
  HangarUiHandler(const bsgo::Views &views);
  virtual ~HangarUiHandler() = default;

  void initializeMenus(const int width, const int height) override;
  auto processUserInput(const controls::State &c, std::vector<ActionShPtr> &actions)
    -> menu::InputHandle override;
  void render(SpriteRenderer &engine) const override;
  void updateUi() override;

  private:
  MenuShPtr m_menu{};
};

using HangarUiHandlerPtr = std::unique_ptr<HangarUiHandler>;

} // namespace pge
