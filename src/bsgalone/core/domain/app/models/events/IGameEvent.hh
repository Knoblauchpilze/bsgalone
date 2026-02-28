
#pragma once

#include "GameEventType.hh"
#include <memory>

namespace bsgalone::core {

class IGameEvent
{
  public:
  virtual ~IGameEvent() = default;

  auto type() const -> GameEventType;

  virtual auto clone() const -> std::unique_ptr<IGameEvent> = 0;

  template<typename Event>
  auto as() -> Event &;

  template<typename Event>
  auto as() const -> const Event &;

  template<typename Event>
  bool isA() const;

  protected:
  IGameEvent(const GameEventType type);

  private:
  GameEventType m_type{};
};

using IGameEventPtr = std::unique_ptr<IGameEvent>;

} // namespace bsgalone::core

#include "IGameEvent.hxx"
