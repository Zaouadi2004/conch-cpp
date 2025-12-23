# Multi-stage Docker build for Conch Cross-Platform Project
# Stage 1: Build environment
FROM ubuntu:22.04 AS builder

# Install build dependencies and Qt system requirements
RUN apt-get update && apt-get install -y \
    build-essential \
    ninja-build \
    git \
    python3 \
    python3-pip \
    wget \
    curl \
    libgl1-mesa-dev \
    pkg-config \
    xkb-data \
    libxkbcommon-x11-0 \
    libxkbcommon-dev \
    libxcb-xkb1 \
    libxcb-icccm4 \
    libxcb-image0 \
    libxcb-keysyms1 \
    libxcb-randr0 \
    libxcb-render-util0 \
    libxcb-shape0 \
    libxcb-sync1 \
    libxcb-xfixes0 \
    libxcb-xinerama0 \
    libxcb1 \
    libfontconfig1 \
    libdbus-1-dev \
    libssl-dev \
    && rm -rf /var/lib/apt/lists/*

# Install CMake 3.28 (required version >= 3.25)
RUN wget https://github.com/Kitware/CMake/releases/download/v3.28.0/cmake-3.28.0-linux-x86_64.sh -O /tmp/cmake.sh \
    && chmod +x /tmp/cmake.sh \
    && /tmp/cmake.sh --skip-license --prefix=/usr/local \
    && rm /tmp/cmake.sh

# Install Conan
RUN pip3 install conan

# Set working directory
WORKDIR /app

# Copy project files
COPY . .

# Create build directory and navigate into it
RUN mkdir -p build

# Create Conan default profile
RUN conan profile detect --force

# Install Conan dependencies (no sudo needed in Docker as root user)
WORKDIR /app/build
RUN conan install .. \
    --build=missing \
    -c tools.system.package_manager:mode=install \
    -c tools.system.package_manager:sudo=False

# Configure CMake with Conan toolchain (matching build.sh)
RUN cmake .. \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_TOOLCHAIN_FILE=Release/generators/conan_toolchain.cmake

# Build the project
RUN cmake --build . --config Release -j$(nproc)

# Stage 2: Runtime environment (minimal)
FROM ubuntu:22.04 AS runtime

# Install only runtime dependencies
RUN apt-get update && apt-get install -y \
    libstdc++6 \
    libgl1 \
    libglib2.0-0 \
    libdbus-1-3 \
    && rm -rf /var/lib/apt/lists/*

# Create non-root user
RUN useradd -m -u 1000 conchuser

# Copy binaries from builder
COPY --from=builder /app/build/bin/* /usr/local/bin/

# Switch to non-root user
USER conchuser

# Default command
CMD ["trading_engine"]
