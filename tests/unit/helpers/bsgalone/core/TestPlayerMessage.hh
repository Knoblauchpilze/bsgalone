
#pragma once

#include "AbstractPlayerMessage.hh"
#include "Uuid.hh"

namespace test {

class TestPlayerMessage : public bsgalone::core::AbstractPlayerMessage
{
  public:
  // The type of message does not matter, it is just defined because it is
  // required to pick one to instantiate an `AbstractPlayerMessage`.
  TestPlayerMessage(const bsgalone::core::Uuid playerDbId, const bsgalone::core::Uuid systemDbId);
  ~TestPlayerMessage() override = default;

  auto clone() const -> bsgalone::core::IMessagePtr override;
};

} // namespace test
