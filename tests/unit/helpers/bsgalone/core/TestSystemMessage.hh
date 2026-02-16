
#pragma once

#include "AbstractSystemMessage.hh"

namespace test {

class TestSystemMessage : public bsgalone::core::AbstractSystemMessage
{
  public:
  // The type of message does not matter, it is just defined because it is
  // required to pick one to instantiate an `AbstractSystemMessage`.
  TestSystemMessage(const bsgo::Uuid systemDbId);
  ~TestSystemMessage() override = default;

  auto serialize(std::ostream &out) const -> std::ostream & override;
  bool deserialize(std::istream &in) override;

  auto clone() const -> std::unique_ptr<IMessage> override;
};

} // namespace test
