#include "copilot/product/error_code.hpp"
#include "drogon_response.hpp"
#include "health_controller.hpp"

#include <drogon/drogon.h>

#include <cstdint>
#include <exception>
#include <string>

namespace {

copilot::product::ErrorCode error_code_for_status(drogon::HttpStatusCode status) {
    using copilot::product::ErrorCode;

    switch (status) {
        case drogon::k400BadRequest:
            return ErrorCode::invalid_request;
        case drogon::k404NotFound:
            return ErrorCode::route_not_found;
        case drogon::k405MethodNotAllowed:
            return ErrorCode::method_not_allowed;
        default:
            return ErrorCode::internal_error;
    }
}

}  // namespace

int main(int argc, char** argv) {
    const std::string config_path = argc > 1 ? argv[1] : "config/product.json";

    auto& app = drogon::app();
    app.loadConfigFile(config_path);

    app.setDefaultHandler(
        [](const drogon::HttpRequestPtr&,
           std::function<void(const drogon::HttpResponsePtr&)>&& callback) {
            callback(copilot::product::error_drogon_response(
                copilot::product::ErrorCode::route_not_found));
        });

    app.setCustomErrorHandler(
        [](drogon::HttpStatusCode status, const drogon::HttpRequestPtr&) {
            return copilot::product::error_drogon_response(
                error_code_for_status(status));
        });

    app.setExceptionHandler(
        [](const std::exception& error,
           const drogon::HttpRequestPtr&,
           std::function<void(const drogon::HttpResponsePtr&)>&& callback) {
            LOG_ERROR << "Unhandled request exception: " << error.what();
            callback(copilot::product::error_drogon_response(
                copilot::product::ErrorCode::internal_error));
        });

    app.registerPreSendingAdvice(
        [](const drogon::HttpRequestPtr& request,
           const drogon::HttpResponsePtr& response) {
            const std::int64_t elapsed_us =
                trantor::Date::now().microSecondsSinceEpoch() -
                request->creationDate().microSecondsSinceEpoch();
            LOG_INFO << request->methodString() << ' ' << request->path()
                     << " -> " << response->statusCode() << " in "
                     << elapsed_us / 1000.0 << "ms";
        });

    app.run();
}
