
#pragma once

#include "AbstractMessageListener.hh"
#include <core_utils/CoreObject.hh>

namespace bsgo {

class AbstractMessageConsumer : public AbstractMessageListener, public utils::CoreObject
{
  public:
  AbstractMessageConsumer(const std::string &name,
                          const std::unordered_set<MessageType> &relevantMessageTypes);
  ~AbstractMessageConsumer() override = default;
};

} // namespace bsgo
