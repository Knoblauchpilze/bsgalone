
#include "TransformComponent.hh"
#include "AxisAlignedBox.hh"
#include "CircleBox.hh"
#include <gtest/gtest.h>

namespace bsgalone::core {

TEST(Unit_Bsgalone_Core_Domain_App_Models_Components_TransformComponent,
     SizeIsEqualToRadiusForCircleBox)
{
  TransformComponent component{.bbox = std::make_shared<CircleBox>(Eigen::Vector3f::Ones(), 1.237f)};

  EXPECT_EQ(1.237f, component.size());
}

TEST(Unit_Bsgalone_Core_Domain_App_Models_Components_TransformComponent,
     SizeIsEqualToLargestAxesForAxisAlignedBoundingBox)
{
  TransformComponent component{
    .bbox = std::make_shared<AxisAlignedBox>(Eigen::Vector3f::Ones(),
                                             Eigen::Vector3f(1.0f, 2.0f, 3.0f))};

  EXPECT_EQ(3.0f, component.size());
}

TEST(Unit_Bsgalone_Core_Domain_App_Models_Components_TransformComponent,
     TransformToGlobalForCircleBox)
{
  TransformComponent component{.bbox = std::make_shared<CircleBox>(Eigen::Vector3f(0.5f, 2.5f, 0.0f),
                                                                   1.237f),
                               .heading = 0.707f};

  Eigen::Vector3f pos(1.0f, 0.0f, 0.0f);
  Eigen::Vector3f actual = component.transformToGlobal(pos);
  EXPECT_EQ(Eigen::Vector3f(1.26031399f, 1.85044432f, 0.0f), actual);

  pos    = Eigen::Vector3f(0.0f, 1.0f, 0.0f);
  actual = component.transformToGlobal(pos);
  EXPECT_EQ(Eigen::Vector3f(1.14955568f, 3.26031399f, 0.0f), actual);

  pos    = Eigen::Vector3f(-3.0f, 6.8f, 0.0f);
  actual = component.transformToGlobal(pos);
  EXPECT_EQ(Eigen::Vector3f(2.63603759f, 9.61880207f, 0.0f), actual);

  pos    = Eigen::Vector3f(-8.57f, -17.45f, 0.0f);
  actual = component.transformToGlobal(pos);
  EXPECT_EQ(Eigen::Vector3f(-17.3506393f, -5.20078659f, 0.0f), actual);
}

TEST(Unit_Bsgalone_Core_Domain_App_Models_Components_TransformComponent,
     TransformToGlobalForAxisAlignedBox)
{
  TransformComponent
    component{.bbox    = std::make_shared<AxisAlignedBox>(Eigen::Vector3f(0.5f, 2.5f, 0.0f),
                                                       Eigen::Vector3f(0.8f, 0.9f, 1.2f)),
              .heading = 0.707f};

  Eigen::Vector3f pos(1.0f, 0.0f, 0.0f);
  Eigen::Vector3f actual = component.transformToGlobal(pos);
  EXPECT_EQ(Eigen::Vector3f(1.26031399f, 1.85044432f, 0.0f), actual);

  pos    = Eigen::Vector3f(0.0f, 1.0f, 0.0f);
  actual = component.transformToGlobal(pos);
  EXPECT_EQ(Eigen::Vector3f(1.14955568f, 3.26031399f, 0.0f), actual);

  pos    = Eigen::Vector3f(-3.0f, 6.8f, 0.0f);
  actual = component.transformToGlobal(pos);
  EXPECT_EQ(Eigen::Vector3f(2.63603759f, 9.61880207f, 0.0f), actual);

  pos    = Eigen::Vector3f(-8.57f, -17.45f, 0.0f);
  actual = component.transformToGlobal(pos);
  EXPECT_EQ(Eigen::Vector3f(-17.3506393f, -5.20078659f, 0.0f), actual);
}

} // namespace bsgalone::core
