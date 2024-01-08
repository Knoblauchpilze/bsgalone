
#pragma once

#include "UiTextMenu.hh"

namespace pge {

struct TextFieldConfig
{
  Vec2i pos{};
  Vec2i dims{10, 10};

  bool visible{true};

  std::optional<GameCallback> gameClickCallback{};
};

class UiTextField : public UiTextMenu
{
  public:
  UiTextField(const TextFieldConfig &config, const BackgroundConfig &bg, const TextConfig &text);
  ~UiTextField() override = default;

  auto getText() const noexcept -> std::string override;

  bool processUserInput(UserInputData &inputData) override;

  private:
  int m_cursorPos{0};
  std::string m_fullText{};
  bool m_editing{false};

  void updateInternalText(const controls::State &controls);
  void updateCursorPosition(const controls::State &controls);
  void generateDisplayedText();
};

using UiTextFieldPtr = std::unique_ptr<UiTextField>;

} // namespace pge
