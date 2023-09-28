
#include "GameScreenSystemHandler.hh"
#include "ScreenCommon.hh"

namespace pge {

GameScreenSystemHandler::GameScreenSystemHandler(const bsgo::Views &views)
  : m_systemView(views.systemView)
{
  if (nullptr == m_systemView)
  {
    throw std::invalid_argument("Expected non null system view");
  }
}

void GameScreenSystemHandler::loadResources(int /*width*/,
                                            int /*height*/,
                                            sprites::TexturePack & /*texturesLoader*/)
{}

void GameScreenSystemHandler::render(SpriteRenderer & /*engine*/,
                                     const RenderState & /*state*/,
                                     const RenderingPass pass) const
{
  if (pass != RenderingPass::DECAL)
  {
    return;
  }
}

auto GameScreenSystemHandler::processUserInput(const controls::State & /*c*/,
                                               std::vector<ActionShPtr> & /*actions*/)
  -> menu::InputHandle
{
  return {};
}

void GameScreenSystemHandler::updateUi() {}

} // namespace pge
