
#pragma once

#include "IInputHandler.hh"
#include "IMessageQueue.hh"
#include <string>

namespace pge {

class AbstractInputHandler : public IInputHandler
{
  public:
  AbstractInputHandler(const std::string &name, bsgo::IMessageQueue *const messageQueue);
  ~AbstractInputHandler() override = default;

  protected:
  bsgo::IMessageQueue *const m_messageQueue{};
};

} // namespace pge
