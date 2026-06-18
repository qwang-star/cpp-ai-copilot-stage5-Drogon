#pragma once

#include "copilot/product/api_response.hpp"
#include "copilot/product/error_code.hpp"

#include <drogon/HttpResponse.h>
#include <drogon/HttpTypes.h>

#include <string>

namespace copilot::product {

inline drogon::HttpResponsePtr make_drogon_json_response(
    int status,
    const std::string& body) {
    auto response = drogon::HttpResponse::newHttpResponse();
    response->setStatusCode(static_cast<drogon::HttpStatusCode>(status));
    response->setContentTypeCode(drogon::CT_APPLICATION_JSON);
    response->setBody(body);
    return response;
}

inline drogon::HttpResponsePtr success_drogon_response(const std::string& data_json) {
    return make_drogon_json_response(http_status(ErrorCode::ok), success_body(data_json));
}

inline drogon::HttpResponsePtr error_drogon_response(ErrorCode code) {
    return make_drogon_json_response(http_status(code), error_body(code));
}

}  // namespace copilot::product
