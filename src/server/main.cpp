#include "twf/server/server.hpp"
#include "shared/site_models.hpp"

#include <iostream>
#include <format>
#include <cstdlib>
#include <vector>
#include <mutex>

int main() {
    twf::Server app;

    // Portfolio data
    site::SiteInfo info;
    std::vector<site::Project> projects = {
        {1, "twf (The Web Framework)", "Full-stack C++23 & WebAssembly web framework with zero-copy I/O and compile-time reflection.", "C++23, Wasm, Glaze, Ninja", "https://github.com", ""},
        {2, "Low-Latency Matching Engine", "High-frequency limit order book with lock-free queues and kernel-bypass UDP feeds.", "C++20, Lock-Free, SIMD", "https://github.com", ""},
        {3, "Distributed Key-Value Store", "Raft consensus distributed storage engine with LSM-tree storage and async RPC.", "C++23, Raft, Distributed", "https://github.com", ""}
    };

    std::mutex messages_mutex;
    std::vector<site::ContactMessage> messages;

    // ---------------------------------------------------------
    // Static Asset Routes
    // ---------------------------------------------------------
    app.get("/", [](const twf::Request&) {
        return twf::Response::file("public/index.html", "text/html; charset=utf-8");
    });

    app.get("/styles.css", [](const twf::Request&) {
        return twf::Response::file("public/styles.css", "text/css; charset=utf-8");
    });

    app.get("/wasm/website_client.js", [](const twf::Request&) {
        return twf::Response::file("public/wasm/website_client.js", "application/javascript");
    });

    app.get("/wasm/website_client.wasm", [](const twf::Request&) {
        return twf::Response::file("public/wasm/website_client.wasm", "application/wasm");
    });

    // ---------------------------------------------------------
    // API Endpoints (C++23 Reflection)
    // ---------------------------------------------------------
    app.get("/api/info", [&info](const twf::Request&) {
        return twf::Response::json(info);
    });

    app.get("/api/projects", [&projects](const twf::Request&) {
        return twf::Response::json(projects);
    });

    app.post("/api/contact", [&messages, &messages_mutex](const twf::Request& req) {
        auto msg = req.json<site::ContactMessage>();
        if (!msg) {
            return twf::Response::json(
                site::ContactResponse{false, "Invalid contact payload: " + msg.error().message}, 
                twf::StatusCode::BadRequest
            );
        }

        std::cout << std::format("[Contact Received] From: {} <{}>\n  Message: {}\n", 
            msg->name, msg->email, msg->message);

        {
            std::lock_guard<std::mutex> lock(messages_mutex);
            messages.push_back(*msg);
        }

        return twf::Response::json(
            site::ContactResponse{true, "Thank you, " + msg->name + "! Your message was received by the C++ backend."}
        );
    });

    // Determine port from environment (standard for Cloud/Docker) or fallback to 8081
    int port = 8081;
    if (const char* env_port = std::getenv("PORT")) {
        port = std::atoi(env_port);
    }

    std::cout << std::format("\n==================================================\n");
    std::cout << std::format("   Personal Website (Powered by twf C++23)        \n");
    std::cout << std::format("   Listening at: http://localhost:{}/             \n", port);
    std::cout << std::format("==================================================\n\n");

    auto result = app.listen("0.0.0.0", port);
    if (!result) {
        std::cerr << std::format("[Server Error] {}\n", result.error().message);
        return 1;
    }

    return 0;
}
