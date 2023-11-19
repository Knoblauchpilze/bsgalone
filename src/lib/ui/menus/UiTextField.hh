
#pragma once

#include "UiTextMenu.hh"

namespace pge {

class UiTextField : public UiTextMenu
{
  public:
  UiTextField(const MenuConfig &config, const BackgroundConfig &bg, const TextConfig &text);
  ~UiTextField() override = default;

  bool processUserInput(UserInputData &inputData) override;

  private:
  int m_cursorPos{0};
  std::string m_fullText{};

  void updateInternalText(const controls::State &controls);
  void updateCursorPosition(const controls::State &controls);
  void generateDisplayedText();
};

using UiTextFieldPtr = std::unique_ptr<UiTextField>;

} // namespace pge
