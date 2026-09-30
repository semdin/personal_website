# Personal Website (Powered by twf)

A modern, high-performance personal engineering portfolio built with **C++23** on the backend and **WebAssembly (Wasm)** on the frontend, powered by the **twf** (The Web Framework) engine.

---

## Features

- **100% C++23 Native Backend**: Zero-copy HTTP server, compile-time JSON reflection with Glaze.
- **Embedded C++ WebAssembly Engine**: Interactive terminal and compute benchmark running inside the browser's Wasm VM.
- **Microsecond Response Times**: Sub-millisecond latency and ultra-low RAM usage (< 15MB).
- **Docker-Ready**: Multi-stage Docker build producing a minimal 30MB production container image.

---

## 1. Run Locally

Ensure you have your modern C++23 compiler and CMake ready:

```bash
# Configure and build
cmake -B build -G Ninja -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_C_COMPILER=clang
cmake --build build

# Start the website server
./build/my_website.exe
```

Open your browser at **`http://localhost:8081/`**.

---

## 2. Recompiling the WebAssembly Frontend

If you modify `src/client/main.cpp`:

```bash
em++ -std=c++23 -O2 --bind -I ../twf/include \
  -sEXPORTED_RUNTIME_METHODS="['ccall','cwrap','stringToUTF8','lengthBytesUTF8']" \
  -sEXPORTED_FUNCTIONS="['_main','_run_cpp_benchmark','_execute_terminal_cmd','_malloc','_free']" \
  -sNO_EXIT_RUNTIME=1 \
  src/client/main.cpp -o public/wasm/website_client.js
```

---

## 3. Deployment to a Server (Production)

### Option A: Using Docker & Docker Compose (Recommended)

From the `personal_website` directory:

```bash
docker compose up -d --build
```

Your website will be live on port `8080` inside a secure, lightweight container.

---

### Option B: Deploying with Automatic HTTPS (Caddy)

On your production server (Ubuntu/Debian VPS):

1. Install **Caddy** (automatic SSL provider):
   ```bash
   sudo apt install -y caddy
   ```

2. Point your domain (e.g. `yourname.dev`) to your server's IP address.

3. Edit `/etc/caddy/Caddyfile`:
   ```caddy
   yourname.dev {
       reverse_proxy localhost:8080
   }
   ```

4. Restart Caddy:
   ```bash
   sudo systemctl restart caddy
   ```

Caddy will automatically provision a free Let's Encrypt SSL/TLS certificate and route all traffic to your C++ `twf` server!
