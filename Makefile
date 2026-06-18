CXX ?= g++
CXXFLAGS ?= -std=c++20 -Wall -Wextra -pedantic -O2 -finput-charset=UTF-8 -fexec-charset=UTF-8
CMAKE ?= cmake
CTEST ?= ctest
INCLUDES := -Iinclude -Ithird_party
PRODUCT_BUILD_DIR := build-drogon

SRC := src/api_response.cpp src/application.cpp src/chat_service.cpp src/config.cpp src/error_code.cpp src/http.cpp src/router.cpp src/logger.cpp
SERVER_SRC := src/main.cpp src/simple_server.cpp $(SRC)
TEST_SRC := tests/test_core.cpp $(SRC)
PRODUCT_API_TEST_SRC := tests/test_api_response.cpp src/product/api_response.cpp src/product/error_code.cpp

ifeq ($(OS),Windows_NT)
  SERVER_LIBS := -lws2_32
  EXE := .exe
else
  SERVER_LIBS :=
  EXE :=
endif

.PHONY: all test product-api-test run clean product-configure product-build product-test product-run

all: build/cpp-ai-copilot$(EXE)

build:
	@if not exist build mkdir build

build/cpp-ai-copilot$(EXE): build $(SERVER_SRC)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(SERVER_SRC) -o $@ $(SERVER_LIBS)

build/test_core$(EXE): build $(TEST_SRC)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(TEST_SRC) -o $@

build/test_api_response$(EXE): build $(PRODUCT_API_TEST_SRC)
	$(CXX) $(CXXFLAGS) -Iinclude -Isrc/product $(PRODUCT_API_TEST_SRC) -o $@

test: build/test_core$(EXE)
	./build/test_core$(EXE)

product-api-test: build/test_api_response$(EXE)
	./build/test_api_response$(EXE)

product-configure:
	$(CMAKE) -S . -B $(PRODUCT_BUILD_DIR) -G Ninja -DCMAKE_BUILD_TYPE=Debug -DCOPILOT_BUILD_DROGON=ON

product-build: product-configure
	$(CMAKE) --build $(PRODUCT_BUILD_DIR) --target cpp-ai-copilot-product

product-test: product-configure
	$(CMAKE) --build $(PRODUCT_BUILD_DIR) --target cpp-ai-copilot-product test_core test_api_response
	$(CTEST) --test-dir $(PRODUCT_BUILD_DIR) --output-on-failure

product-run: product-build
	./$(PRODUCT_BUILD_DIR)/cpp-ai-copilot-product$(EXE) config/product.json

run: build/cpp-ai-copilot$(EXE)
	./build/cpp-ai-copilot$(EXE) config/app.env

clean:
	@if exist build rmdir /s /q build
