# ===============================================================
# Stage 1: Build the C++23 Native Server & WebAssembly Frontend
# ===============================================================
FROM ubuntu:24.04 AS builder

ENV DEBIAN_FRONTEND=noninteractive

# Install build dependencies: Clang, CMake, Ninja, Git, Python, curl
RUN apt-get update && apt-get install -y \
    clang \
    clang++ \
    cmake \
    ninja-build \
    git \
    python3 \
    curl \
    && rm -rf /var/lib/apt/lists/*

# Install Emscripten SDK for WebAssembly
WORKDIR /opt
RUN git clone --depth 1 https://github.com/emscripten-core/emsdk.git && \
    cd emsdk && \
    ./emsdk install latest && \
    ./emsdk activate latest

ENV PATH="/opt/emsdk:/opt/emsdk/upstream/emscripten:${PATH}"
ENV EMSDK="/opt/emsdk"

WORKDIR /app
COPY . .

# Configure and build native server (FetchContent pulls twf from GitHub)
RUN cmake -B build -G Ninja -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_C_COMPILER=clang -DCMAKE_BUILD_TYPE=Release
RUN cmake --build build --target my_website

# Build WebAssembly Frontend using fetched twf include directory
RUN em++ -std=c++23 -O2 --bind \
    -I build/_deps/twf-src/include \
    -sEXPORTED_RUNTIME_METHODS="['ccall','cwrap','stringToUTF8','lengthBytesUTF8']" \
    -sEXPORTED_FUNCTIONS="['_main','_run_cpp_benchmark','_execute_terminal_cmd','_malloc','_free']" \
    -sNO_EXIT_RUNTIME=1 \
    src/client/main.cpp -o public/wasm/website_client.js

# ===============================================================
# Stage 2: Ultra-Minimal Production Runtime Image (~30MB)
# ===============================================================
FROM ubuntu:24.04 AS runner

RUN apt-get update && apt-get install -y --no-install-recommends \
    ca-certificates \
    libstdc++6 \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

# Copy binary and web assets from builder stage
COPY --from=builder /app/build/my_website /app/my_website
COPY --from=builder /app/public /app/public

ENV PORT=8080
EXPOSE 8080

CMD ["./my_website"]
