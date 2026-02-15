
#include "MessageType.hh"
#include "SerializationUtils.hh"
#include <gtest/gtest.h>
#include <sstream>

using namespace ::testing;

namespace bsgo {

struct TestCaseMessageType
{
  bsgalone::core::MessageType type{};
};

using TestCaseSerialization = TestWithParam<TestCaseMessageType>;

TEST_P(TestCaseSerialization, Serialization)
{
  const auto &param = GetParam();

  std::ostringstream out;
  core::serialize(out, param.type);
  std::istringstream in(out.str());

  bsgalone::core::MessageType expected;
  core::deserialize(in, expected);

  EXPECT_EQ(param.type, expected);
}

INSTANTIATE_TEST_SUITE_P(Unit_Bsgo_Serialization_MessageType,
                         TestCaseSerialization,
                         Values(TestCaseMessageType{bsgalone::core::MessageType::DOCK},
                                TestCaseMessageType{bsgalone::core::MessageType::EQUIP},
                                TestCaseMessageType{bsgalone::core::MessageType::HANGAR},
                                TestCaseMessageType{bsgalone::core::MessageType::JUMP},
                                TestCaseMessageType{bsgalone::core::MessageType::LOGIN},
                                TestCaseMessageType{bsgalone::core::MessageType::LOOT},
                                TestCaseMessageType{bsgalone::core::MessageType::PURCHASE},
                                TestCaseMessageType{bsgalone::core::MessageType::SCANNED},
                                TestCaseMessageType{bsgalone::core::MessageType::SLOT},
                                TestCaseMessageType{bsgalone::core::MessageType::VELOCITY},
                                TestCaseMessageType{bsgalone::core::MessageType::TARGET}),
                         [](const TestParamInfo<TestCaseMessageType> &info) -> std::string {
                           return str(info.param.type);
                         });

} // namespace bsgo
