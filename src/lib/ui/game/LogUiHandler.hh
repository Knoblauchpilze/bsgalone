
#pragma once

#include "IUiHandler.hh"
#include "Views.hh"
#include <memory>

namespace pge {

class LogUiHandler : public IUiHandler
{
  public:
  LogUiHandler(const bsgo::Views &views);
  ~LogUiHandler() override = default;

  void initializeMenus(const int width, const int height) override;
  bool processUserInput(UserInputData &inputData) override;
  void render(SpriteRenderer &engine) const override;
  void updateUi() override;
  void reset() override;

  private:
};

using LogUiHandlerPtr = std::unique_ptr<LogUiHandler>;

} // namespace pge
