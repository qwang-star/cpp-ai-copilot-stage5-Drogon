#include "health_controller.hpp"

#include "copilot/product/api_response.hpp"
#include "drogon_response.hpp"

#include <drogon/drogon.h>

#include <string>

namespace copilot::product {

void HealthController::asyncHandleHttpRequest(
    const drogon::HttpRequestPtr&,
    std::function<void(const drogon::HttpResponsePtr&)>&& callback) {
    const Json::Value& config = drogon::app().getCustomConfig();
    const std::string service_name =
        config.get("service_name", "cpp-ai-copilot").asString();
    const std::string version = config.get("version", "0.2.0").asString();

    callback(success_drogon_response(health_data(service_name, version)));
}

}  // namespace copilot::product
