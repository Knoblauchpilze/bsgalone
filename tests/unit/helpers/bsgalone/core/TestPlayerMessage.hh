
#pragma once

#include "AbstractPlayerMessage.hh"
#include "Uuid.hh"

namespace test {

class TestPlayerMessage : public bsgalone::core::AbstractPlayerMessage
{
  public:
  TestPlayerMessage(const bsgalone::core::Uuid playerDbId);
  ~TestPlayerMessage() override = default;

  auto clone() const -> bsgalone::core::IMessagePtr override;
};

} // namespace test
