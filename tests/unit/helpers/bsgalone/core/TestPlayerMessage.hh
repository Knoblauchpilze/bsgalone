
#pragma once

#include "AbstractPlayerMessage.hh"
#include "Uuid.hh"

namespace test {

class TestPlayerMessage : public bsgalone::core::AbstractPlayerMessage
{
  public:
  // The type of message does not matter, it is just defined because it is
  // required to pick one to instantiate an `AbstractPlayerMessage`.
  TestPlayerMessage(const bsgo::Uuid playerDbId, const bsgo::Uuid systemDbId);
  ~TestPlayerMessage() override = default;

  auto serialize(std::ostream &out) const -> std::ostream & override;
  bool deserialize(std::istream &in) override;

  auto clone() const -> std::unique_ptr<IMessage> override;
};

} // namespace test
