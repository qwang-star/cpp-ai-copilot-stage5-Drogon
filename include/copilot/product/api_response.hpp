#pragma once

#include "copilot/product/error_code.hpp"

#include <string>

namespace copilot::product {

std::string success_body(const std::string& data_json);
std::string error_body(ErrorCode code);
std::string health_data(const std::string& service_name, const std::string& version);

}  // namespace copilot::product
