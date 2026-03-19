
#include "LoginUiHandler.hh"

namespace bsgalone::client {

LoginUiHandler::LoginUiHandler()
  : IUiHandler()
{}

void LoginUiHandler::initializeMenus(const int /*width*/,
                                     const int /*height*/,
                                     pge::sprites::TexturePack & /*texturesLoader*/)
{}

bool LoginUiHandler::processUserInput(ui::UserInputData & /*inputData*/)
{
  return false;
}

void LoginUiHandler::render(pge::Renderer & /*engine*/) const {}

void LoginUiHandler::updateUi() {}

} // namespace bsgalone::client
