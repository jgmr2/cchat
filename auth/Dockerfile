# Stage 1: Compilation
FROM alpine:latest AS builder

ARG MONGOCRYPT_VERSION=1.10.0
ARG MONGOC_VERSION=1.27.1
ARG MONGOCXX_VERSION=3.10.1

RUN apk update && apk add --no-cache \
    build-base \
    cmake \
    ninja \
    git \
    wget \
    tar \
    boost-dev \
    python3 \
    py3-pip \
    py3-packaging \
    py3-setuptools \
    boost-dev \
    openssl-dev \
    asio-dev

WORKDIR /build

RUN mkdir -p /opt/mongocxx

# === Build libmongocrypt ===
RUN wget https://github.com/mongodb/libmongocrypt/archive/refs/tags/${MONGOCRYPT_VERSION}/libmongocrypt-${MONGOCRYPT_VERSION}.tar.gz && \
    tar xf libmongocrypt-${MONGOCRYPT_VERSION}.tar.gz && \
    cmake -S libmongocrypt-${MONGOCRYPT_VERSION} -B build-crypt -G Ninja \
        -DCMAKE_BUILD_TYPE=RelWithDebInfo \
        -DCMAKE_INSTALL_PREFIX=/opt/mongocxx \
        -DBUILD_VERSION=${MONGOCRYPT_VERSION} && \
    cmake --build build-crypt && \
    cmake --install build-crypt

# === Build mongo-c-driver ===
RUN wget https://github.com/mongodb/mongo-c-driver/archive/refs/tags/${MONGOC_VERSION}/mongo-c-driver-${MONGOC_VERSION}.tar.gz && \
    tar xf mongo-c-driver-${MONGOC_VERSION}.tar.gz && \
    cmake -S mongo-c-driver-${MONGOC_VERSION} -B build-c -G Ninja \
        -DCMAKE_BUILD_TYPE=RelWithDebInfo \
        -DCMAKE_INSTALL_PREFIX=/opt/mongocxx \
        -DENABLE_AUTOMATIC_INIT_AND_CLEANUP=OFF \
        -DMONGOC_ENABLE_CLIENT_SIDE_ENCRYPTION=ON && \
    cmake --build build-c && \
    cmake --install build-c

# === Build mongo-cxx-driver ===
RUN wget https://github.com/mongodb/mongo-cxx-driver/archive/refs/tags/r${MONGOCXX_VERSION}/mongo-cxx-driver-${MONGOCXX_VERSION}.tar.gz && \
    tar xf mongo-cxx-driver-${MONGOCXX_VERSION}.tar.gz && \
    cmake -S mongo-cxx-driver-r${MONGOCXX_VERSION} -B build-cxx -G Ninja \
    -DCMAKE_BUILD_TYPE=RelWithDebInfo \
    -DCMAKE_CXX_STANDARD=17 \
    -DCMAKE_INSTALL_PREFIX=/opt/mongocxx \
    -DCMAKE_PREFIX_PATH=/opt/mongocxx \
    -DBUILD_VERSION=${MONGOCXX_VERSION} && \
    cmake --build build-cxx --verbose && \
    cmake --install build-cxx

# === Build your app ===

# Instalar libbcrypt desde GitHub
RUN git clone https://github.com/trusch/libbcrypt.git /libbcrypt && \
    cd /libbcrypt && \
    mkdir build && cd build && \
    cmake .. && \
    make && \
    make install

WORKDIR /app
COPY ./src /app
# Clone httlib (headers only)
RUN wget -O /app/httplib.h https://raw.githubusercontent.com/yhirose/cpp-httplib/master/httplib.h

# === Add nlohmann/json (single include version) ===
RUN wget -O /app/json.hpp https://raw.githubusercontent.com/nlohmann/json/develop/single_include/nlohmann/json.hpp


RUN g++ -std=c++17 -O3 *.cpp -o app \
    -I/opt/mongocxx/include \
    -I/opt/mongocxx/include/bsoncxx/v_noabi \
    -I/opt/mongocxx/include/mongocxx/v_noabi \
    -L/opt/mongocxx/lib \
    -lmongocxx -lbsoncxx -lssl -lcrypto -lz -lpthread -lbcrypt





# Stage 2: Minimal image for execution
FROM alpine:latest

RUN apk add --no-cache \
    libstdc++ \
    openssl \
    boost-system \
    zlib \
    libgcc \
    zstd

COPY --from=builder /app/app /app/app
COPY --from=builder /opt/mongocxx /usr/local

# For dynamic libraries to be found
ENV LD_LIBRARY_PATH="/usr/local/lib:/usr/local/lib64"

WORKDIR /app
EXPOSE 80

CMD ["./app"]