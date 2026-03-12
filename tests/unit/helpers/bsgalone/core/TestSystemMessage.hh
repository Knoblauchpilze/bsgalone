
#pragma once

#include "AbstractSystemMessage.hh"

namespace test {

class TestSystemMessage : public bsgalone::core::AbstractSystemMessage
{
  public:
  // The type of message does not matter, it is just defined because it is
  // required to pick one to instantiate an `AbstractSystemMessage`.
  TestSystemMessage(const bsgalone::core::Uuid systemDbId);
  ~TestSystemMessage() override = default;

  auto clone() const -> bsgalone::core::IMessagePtr override;
};

} // namespace test
