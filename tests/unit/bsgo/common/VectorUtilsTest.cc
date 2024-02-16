
#include "VectorUtils.hh"
#include <eigen3/Eigen/Eigen>
#include <gtest/gtest.h>

using namespace ::testing;

namespace bsgo {
namespace {
auto serializeAndDeserialize(const Eigen::Vector3f &expected, Eigen::Vector3f &actual)
{
  std::ostringstream out{};
  serialize(out, expected);
  std::istringstream in(out.str());

  deserialize(in, actual);
}
} // namespace

struct TestCaseVector
{
  Eigen::Vector3f expected{};
};

using TestCaseVector3fSerialization = TestWithParam<TestCaseVector>;

TEST_P(TestCaseVector3fSerialization, Serialization)
{
  const auto &param = GetParam();

  Eigen::Vector3f actual{};
  serializeAndDeserialize(param.expected, actual);
  EXPECT_EQ(actual, param.expected);
}

INSTANTIATE_TEST_CASE_P(Unit_Bsgo_Serialization_Vector3f,
                        TestCaseVector3fSerialization,
                        Values(TestCaseVector{Eigen::Vector3f{0.0f, 0.0f, 0.0f}},
                               TestCaseVector{Eigen::Vector3f{1.0f, 0.0f, 0.0f}},
                               TestCaseVector{Eigen::Vector3f{0.0f, 1.0f, 0.0f}},
                               TestCaseVector{Eigen::Vector3f{0.0f, 0.0f, 1.0f}},
                               TestCaseVector{Eigen::Vector3f{0.1f, 5.0f, -5.0f}},
                               TestCaseVector{Eigen::Vector3f{-2.0f, 2.0f, 1.0f}},
                               TestCaseVector{Eigen::Vector3f{17.0f, 23.0f, 26.0f}},
                               TestCaseVector{Eigen::Vector3f{-0.25f, -18.0f, -52.0f}},
                               TestCaseVector{Eigen::Vector3f{0.01871f, -0.9817f, 0.987f}}),
                        [](const TestParamInfo<TestCaseVector> &info) -> std::string {
                          auto out = str(info.param.expected);
                          std::replace(out.begin(), out.end(), '.', '_');
                          std::replace(out.begin(), out.end(), '-', 'm');
                          return out;
                        });

TEST(TestCaseVector3fSerialization, EmptyOptional)
{
  std::optional<Eigen::Vector3f> expected{};

  std::ostringstream out{};
  serialize(out, expected);
  std::istringstream in(out.str());

  std::optional<Eigen::Vector3f> actual{Eigen::Vector3f::Zero()};
  deserialize(in, actual);
  EXPECT_EQ(actual, expected);

  in.str(out.str());
  actual.reset();
  deserialize(in, actual);
  EXPECT_EQ(actual, expected);
}

TEST(TestCaseVector3fSerialization, OptionalWithValue)
{
  std::optional<Eigen::Vector3f> expected{Eigen::Vector3f(1.0f, 2.5f, -3.7f)};

  std::ostringstream out{};
  serialize(out, expected);
  std::istringstream in(out.str());

  std::optional<Eigen::Vector3f> actual{Eigen::Vector3f(-4.0f, 3.1f, -0.8f)};
  deserialize(in, actual);
  EXPECT_EQ(actual, expected);

  in.str(out.str());
  actual.reset();
  deserialize(in, actual);
  EXPECT_EQ(actual, expected);
}

} // namespace bsgo
