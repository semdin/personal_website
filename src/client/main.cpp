#include "twf/client/dom.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <cmath>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <emscripten/val.h>
#include <emscripten/bind.h>

static twf::dom::Element* g_benchmark_result = nullptr;
static twf::dom::Element* g_terminal_output = nullptr;

// C++ Benchmark running inside WebAssembly in the browser
extern "C" {
EMSCRIPTEN_KEEPALIVE
void run_cpp_benchmark() {
    if (!g_benchmark_result) return;
    g_benchmark_result->text("Running C++ compute loop...");

    auto start = std::chrono::high_resolution_clock::now();

    // Compute prime numbers up to 100,000 in pure C++
    int count = 0;
    for (int n = 2; n <= 100000; ++n) {
        bool is_prime = true;
        int limit = static_cast<int>(std::sqrt(n));
        for (int d = 2; d <= limit; ++d) {
            if (n % d == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime) count++;
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto elapsed_us = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    double ms = elapsed_us / 1000.0;

    g_benchmark_result->html(
        "&#x2714; Found <b>" + std::to_string(count) + " primes</b> in <b>" + 
        std::to_string(ms) + " ms</b> running inside your browser's WebAssembly CPU!"
    );
}

EMSCRIPTEN_KEEPALIVE
void execute_terminal_cmd(const char* cmd_str) {
    if (!g_terminal_output || !cmd_str) return;
    std::string cmd(cmd_str);

    std::string response;
    if (cmd == "help") {
        response = "<span style='color:#38bdf8'>Available commands:</span><br>"
                   "  <b>skills</b>   - View technical competencies<br>"
                   "  <b>projects</b> - View highlighted repositories<br>"
                   "  <b>stack</b>    - View website architecture<br>"
                   "  <b>clear</b>    - Clear terminal screen";
    } else if (cmd == "skills") {
        response = "<span style='color:#10b981'>[Languages & Systems]:</span> C++23, WebAssembly, Rust, Python, Linux<br>"
                   "<span style='color:#10b981'>[Architecture]:</span> Low-Latency Sockets, Epoll, SIMD, Distributed Systems";
    } else if (cmd == "projects") {
        response = "<span style='color:#f59e0b'>1. twf (The Web Framework):</span> Full-stack C++23 & Wasm framework<br>"
                   "<span style='color:#f59e0b'>2. Low-Latency Matching Engine:</span> Microsecond order book in C++<br>"
                   "<span style='color:#f59e0b'>3. Personal Cloud & Server:</span> Self-hosted C++ microservices";
    } else if (cmd == "stack") {
        response = "<span style='color:#a855f7'>Stack:</span> 100% C++23 (Backend + WebAssembly Frontend) | twf framework";
    } else if (cmd == "clear") {
        g_terminal_output->html("");
        return;
    } else {
        response = "<span style='color:#ef4444'>Command not recognized: '" + cmd + "'. Type 'help' for options.</span>";
    }

    twf::dom::Element entry = twf::dom::div();
    entry.attr("style", "margin-bottom: 0.6rem;");
    entry.html("<span style='color:#64748b;'>$ " + cmd + "</span><br>" + response);
    g_terminal_output->append(entry);
}
}

int main() {
    using namespace twf::dom;

    // Build the interactive terminal widget
    Element terminal = div();
    terminal.id("cpp-terminal-widget");
    terminal.attr("style", 
        "background: #05070d; "
        "border: 1px solid rgba(56, 189, 248, 0.25); "
        "border-radius: 12px; "
        "padding: 1.25rem; "
        "font-family: 'JetBrains Mono', monospace, Consolas, sans-serif; "
        "box-shadow: 0 15px 30px rgba(0,0,0,0.5); "
        "margin-top: 1rem;");

    // Header bar with macOS-like colored dots
    Element header = div();
    header.attr("style", "display: flex; align-items: center; justify-content: space-between; margin-bottom: 1rem; border-bottom: 1px solid rgba(255,255,255,0.06); padding-bottom: 0.75rem;");
    header.html(
        "<div style='display:flex; gap:6px;'>"
        "  <span style='width:10px; height:10px; border-radius:50%; background:#ef4444; display:inline-block;'></span>"
        "  <span style='width:10px; height:10px; border-radius:50%; background:#f59e0b; display:inline-block;'></span>"
        "  <span style='width:10px; height:10px; border-radius:50%; background:#10b981; display:inline-block;'></span>"
        "</div>"
        "<span style='font-size:0.75rem; color:#64748b; font-weight:bold;'>C++23 WebAssembly Terminal Engine</span>"
    );

    // Terminal output area
    static Element output = div();
    output.attr("style", "font-size: 0.85rem; line-height: 1.5; min-height: 90px; color: #cbd5e1;");
    output.html("<span style='color:#94a3b8;'>twf Wasm interactive terminal ready. Type a command below or click a quick action.</span><br>");
    g_terminal_output = &output;

    // Quick Action Chips
    Element chips = div();
    chips.attr("style", "display: flex; gap: 8px; margin-top: 0.75rem; flex-wrap: wrap;");
    chips.html(
        "<button onclick=\"runQuickCmd('skills')\" class='chip'>skills</button>"
        "<button onclick=\"runQuickCmd('projects')\" class='chip'>projects</button>"
        "<button onclick=\"runQuickCmd('stack')\" class='chip'>stack</button>"
        "<button onclick=\"Module._run_cpp_benchmark()\" class='chip benchmark'>run C++ benchmark</button>"
    );

    // Benchmark Result Display
    static Element bench = div();
    bench.attr("style", "margin-top: 0.75rem; font-size: 0.8rem; color: #38bdf8; min-height: 20px;");
    g_benchmark_result = &bench;

    terminal.append(header);
    terminal.append(output);
    terminal.append(chips);
    terminal.append(bench);

    terminal.mount("#terminal-mount-point");
    return 0;
}

#else

int main() {
    return 0;
}

#endif
