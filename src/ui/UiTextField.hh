
#pragma once

#include "UiTextMenu.hh"

namespace ui {

struct TextFieldConfig
{
  pge::Vec2i pos{};
  pge::Vec2i dims{10, 10};

  bool visible{true};
};

class UiTextField : public UiTextMenu
{
  public:
  UiTextField(const TextFieldConfig &config, const BackgroundConfig &bg, const TextConfig &text);
  ~UiTextField() override = default;

  auto getText() const noexcept -> std::string override;

  bool processUserInput(const UserInputData &inputData) override;

  private:
  int m_cursorPos{0};
  std::string m_fullText{};
  bool m_editing{false};

  void updateInternalText(const pge::controls::State &controls);
  void updateCursorPosition(const pge::controls::State &controls);
  void generateDisplayedText();
};

using UiTextFieldPtr = std::unique_ptr<UiTextField>;

} // namespace ui
