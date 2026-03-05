
#pragma once

#include "TickDuration.hh"
#include <eigen3/Eigen/Eigen>
#include <string>

namespace bsgo {

auto str(const Eigen::Vector3f &v) -> std::string;

/// @brief - Multiplies the vector by the duration. In this expression, the vector is
/// representing a value with a dimension of Tick^-1. This could be for example the
/// speed in m/Tick, a rate of change in m/Tick^2, or anything that has at least one
/// dimension with Tick^-1.
/// The result is a component-wise multiplication of the values of the vector by the
/// duration.
/// @param lhs - the value to multiply
/// @param rhs - the duration it is applied for
/// @return - the result of lhs * rhs
auto operator*(const Eigen::Vector3f &lhs, const chrono::TickDuration &rhs) -> Eigen::Vector3f;

} // namespace bsgo
