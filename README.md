# cpp-ai-copilot

C++ 企业级 AI Copilot 后端骨架。当前仓库保留两套后端：

- `cpp-ai-copilot`：手写 Winsock 教学版，用于理解 HTTP 请求解析、路由分发和响应序列化。
- `cpp-ai-copilot-product`：Drogon 产品版，后续 MySQL、Redis、上传、SSE、RAG 和模型调用都在此版本继续开发。

## 当前能力

教学版已经具备：

- `GET /health`
- 统一 JSON 响应
- Router 路由分发
- AppConfig 配置读取
- Logger 控制台日志
- SimpleHttpServer 最小 HTTP 服务
- 单元测试

产品版已经具备：

- Drogon 异步 HTTP 服务骨架
- `GET /health`
- 统一 `code/message/data` JSON 响应
- 统一 404、405 和 500 错误响应
- 请求方法、路径、状态码和耗时日志
- 独立配置文件 `config/product.json`

## 目录结构

```text
include/copilot
  application.hpp
  config.hpp
  http.hpp
  logger.hpp
  router.hpp
  simple_server.hpp
  product/
    api_response.hpp
    error_code.hpp

src
  application.cpp
  config.cpp
  http.cpp
  logger.cpp
  main.cpp
  router.cpp
  simple_server.cpp
  product/
    api_response.cpp
    drogon_response.hpp
    error_code.cpp
    health_controller.cpp
    health_controller.hpp
    main.cpp

tests
  test_core.cpp
  test_api_response.cpp

config
  app.env
  product.json
```

## 构建和测试教学版

```bash
mingw32-make test
mingw32-make all
mingw32-make run
```

教学版默认监听：

```text
http://127.0.0.1:18080
```

健康检查：

```bash
curl http://127.0.0.1:18080/health
```

预期返回：

```json
{"status":"ok","service":"cpp-ai-copilot","version":"0.1.0"}
```

## 构建和测试产品版

产品版依赖 `third_party/drogon-src/drogon-1.9.13` 和其 Trantor 子目录，顶层 CMake 通过 `COPILOT_BUILD_DROGON=ON` 接入。

```bash
mingw32-make product-build
mingw32-make product-test
mingw32-make product-run
```

也可以直接使用 CMake：

```bash
cmake -S . -B build-drogon -G Ninja -DCMAKE_BUILD_TYPE=Debug -DCOPILOT_BUILD_DROGON=ON
cmake --build build-drogon --target cpp-ai-copilot-product test_core test_api_response
ctest --test-dir build-drogon --output-on-failure
```

产品版默认监听：

```text
http://127.0.0.1:18081
```

健康检查：

```bash
curl http://127.0.0.1:18081/health
```

预期返回：

```json
{"code":"OK","message":"success","data":{"status":"ok","service":"cpp-ai-copilot","version":"0.2.0"}}
```

## 第五阶段落地记录

- Drogon 版本：v1.9.13 源码包
- Drogon 路径：`third_party/drogon-src/drogon-1.9.13`
- Trantor 路径：`third_party/drogon-src/drogon-1.9.13/trantor`
- 产品版构建目录：`build-drogon`
- 产品版程序：`build-drogon/cpp-ai-copilot-product.exe`
- 教学版测试：`mingw32-make test`
- 产品版测试：`mingw32-make product-test`

第五阶段只完成产品后端基础骨架。MySQL、Redis、Docker Compose、SSE、模型 API、RAG、文档上传和用户鉴权不在本阶段范围内。
