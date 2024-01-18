
#pragma once

#include <eigen3/Eigen/Eigen>
#include <string>

namespace bsgo {

auto str(const Eigen::Vector3f &v) -> std::string;

auto serialize(std::ostream &out, const Eigen::Vector3f &v) -> std::ostream &;
bool deserialize(std::istream &in, Eigen::Vector3f &v);

} // namespace bsgo
