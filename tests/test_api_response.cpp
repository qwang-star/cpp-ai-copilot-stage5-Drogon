#include "copilot/product/api_response.hpp"
#include "copilot/product/error_code.hpp"

#include <cassert>
#include <string>

using copilot::product::ErrorCode;

void test_error_metadata() {
    using copilot::product::error_message;
    using copilot::product::http_status;
    using copilot::product::to_string;

    assert(std::string(to_string(ErrorCode::route_not_found)) == "ROUTE_NOT_FOUND");
    assert(std::string(error_message(ErrorCode::route_not_found)) == "route not found");
    assert(http_status(ErrorCode::route_not_found) == 404);
    assert(http_status(ErrorCode::method_not_allowed) == 405);
    assert(http_status(ErrorCode::internal_error) == 500);
}

void test_success_body_wraps_data_json() {
    const std::string body = copilot::product::success_body(R"({"status":"ok"})");

    assert(body == R"({"code":"OK","message":"success","data":{"status":"ok"}})");
}

void test_error_body_uses_default_message_and_null_data() {
    const std::string body = copilot::product::error_body(ErrorCode::route_not_found);

    assert(body == R"({"code":"ROUTE_NOT_FOUND","message":"route not found","data":null})");
}

void test_health_data_builds_product_health_payload() {
    const std::string data = copilot::product::health_data("cpp-ai-copilot", "0.2.0");

    assert(data == R"({"status":"ok","service":"cpp-ai-copilot","version":"0.2.0"})");
}

int main() {
    test_error_metadata();
    test_success_body_wraps_data_json();
    test_error_body_uses_default_message_and_null_data();
    test_health_data_builds_product_health_payload();
}
