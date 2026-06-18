#pragma once

#include <drogon/HttpSimpleController.h>

#include <functional>

namespace copilot::product {

class HealthController : public drogon::HttpSimpleController<HealthController> {
public:
    void asyncHandleHttpRequest(
        const drogon::HttpRequestPtr& request,
        std::function<void(const drogon::HttpResponsePtr&)>&& callback) override;

    PATH_LIST_BEGIN
    PATH_ADD("/health", drogon::Get);
    PATH_LIST_END
};

}  // namespace copilot::product
