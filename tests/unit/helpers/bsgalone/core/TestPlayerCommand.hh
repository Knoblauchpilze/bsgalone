
#pragma once

#include "AbstractPlayerCommand.hh"
#include "Uuid.hh"

namespace test {

class TestPlayerCommand : public bsgalone::core::AbstractPlayerCommand
{
  public:
  TestPlayerCommand(const bsgalone::core::Uuid playerDbId, const bsgalone::core::Uuid systemDbId);
  ~TestPlayerCommand() override = default;

  auto clone() const -> bsgalone::core::IMessagePtr override;
};

} // namespace test
