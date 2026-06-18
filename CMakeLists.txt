cmake_minimum_required(VERSION 3.20)

project(cpp_ai_copilot LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(COPILOT_BUILD_DROGON "Build Drogon-based product server" OFF)

if(COPILOT_BUILD_DROGON)
    set(BUILD_CTL OFF CACHE BOOL "" FORCE)
    set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    set(BUILD_ORM OFF CACHE BOOL "" FORCE)
    set(BUILD_BROTLI OFF CACHE BOOL "" FORCE)
    set(BUILD_YAML_CONFIG OFF CACHE BOOL "" FORCE)
    set(USE_SUBMODULE ON CACHE BOOL "" FORCE)
    if(WIN32 AND CMAKE_CXX_COMPILER_ID MATCHES "GNU")
        # MSYS2 UCRT64 links std::atomic<int64_t> without libatomic. Pre-set this
        # to avoid Trantor's try_compile rerun leaving locked temp files on Windows.
        set(ATOMIC_WITHOUT_LINKING TRUE CACHE INTERNAL "" FORCE)
    endif()

    add_subdirectory(third_party/drogon-src/drogon-1.9.13)
endif()

add_library(copilot_core
    src/application.cpp
    src/api_response.cpp
    src/chat_service.cpp
    src/config.cpp
    src/error_code.cpp
    src/http.cpp
    src/logger.cpp
    src/router.cpp
)

target_include_directories(copilot_core PUBLIC include third_party)

add_executable(cpp-ai-copilot
    src/main.cpp
    src/simple_server.cpp
)

target_link_libraries(cpp-ai-copilot PRIVATE copilot_core)

add_custom_target(cpp-ai-copilot-legacy DEPENDS cpp-ai-copilot)

if(WIN32)
    target_link_libraries(cpp-ai-copilot PRIVATE ws2_32)
endif()

add_executable(test_core
    tests/test_core.cpp
)

target_link_libraries(test_core PRIVATE copilot_core)

add_library(copilot_product_api
    src/product/api_response.cpp
    src/product/error_code.cpp
)

target_include_directories(copilot_product_api PUBLIC include src/product)

add_executable(test_api_response
    tests/test_api_response.cpp
)

target_link_libraries(test_api_response PRIVATE copilot_product_api)

if(COPILOT_BUILD_DROGON)
    add_executable(cpp-ai-copilot-product
        src/product/main.cpp
        src/product/health_controller.cpp
    )

    target_include_directories(cpp-ai-copilot-product PRIVATE include src/product)
    target_link_libraries(
        cpp-ai-copilot-product
        PRIVATE copilot_product_api Drogon::Drogon
    )
endif()

enable_testing()
add_test(NAME core COMMAND test_core)
add_test(NAME api_response COMMAND test_api_response)
