
#pragma once

#include <eigen3/Eigen/Eigen>
#include <optional>
#include <string>

namespace bsgo {

auto str(const Eigen::Vector3f &v) -> std::string;

void serialize(std::ostream &out, const Eigen::Vector3f &value);
void serialize(std::ostream &out, const std::optional<Eigen::Vector3f> &value);

bool deserialize(std::istream &in, Eigen::Vector3f &value);
bool deserialize(std::istream &in, std::optional<Eigen::Vector3f> &value);

} // namespace bsgo
