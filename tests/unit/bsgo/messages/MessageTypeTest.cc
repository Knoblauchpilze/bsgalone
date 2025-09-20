
#include "MessageType.hh"
#include "SerializationUtils.hh"
#include <gtest/gtest.h>
#include <sstream>

using namespace ::testing;

namespace bsgo {

struct TestCaseMessageType
{
  MessageType type{};
};

using TestCaseSerialization = TestWithParam<TestCaseMessageType>;

TEST_P(TestCaseSerialization, Serialization)
{
  const auto &param = GetParam();

  std::ostringstream out;
  core::serialize(out, param.type);
  std::istringstream in(out.str());

  MessageType expected;
  core::deserialize(in, expected);

  EXPECT_EQ(param.type, expected);
}

INSTANTIATE_TEST_SUITE_P(Unit_Bsgo_Serialization_MessageType,
                         TestCaseSerialization,
                         Values(TestCaseMessageType{MessageType::DOCK},
                                TestCaseMessageType{MessageType::EQUIP},
                                TestCaseMessageType{MessageType::HANGAR},
                                TestCaseMessageType{MessageType::JUMP},
                                TestCaseMessageType{MessageType::LOGIN},
                                TestCaseMessageType{MessageType::LOOT},
                                TestCaseMessageType{MessageType::PURCHASE},
                                TestCaseMessageType{MessageType::SCANNED},
                                TestCaseMessageType{MessageType::SLOT},
                                TestCaseMessageType{MessageType::VELOCITY},
                                TestCaseMessageType{MessageType::TARGET}),
                         [](const TestParamInfo<TestCaseMessageType> &info) -> std::string {
                           return str(info.param.type);
                         });

} // namespace bsgo
