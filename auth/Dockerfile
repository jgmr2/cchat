# Etapa 1: Compilación
FROM ubuntu:22.04 AS builder

# Evita prompts de configuración
ENV DEBIAN_FRONTEND=noninteractive

# Actualizar e instalar dependencias esenciales
RUN apt-get update && apt-get install -y \
    build-essential \
    curl \
    zip \
    pkg-config \
    git \
    python3-pip \
    && apt-get clean

# Clonar el repositorio de vcpkg y compilarlo
RUN git clone https://github.com/microsoft/vcpkg.git /vcpkg \
    && /vcpkg/bootstrap-vcpkg.sh

# Configurar el contenedor para usar vcpkg
ENV VCPKG_ROOT=/vcpkg
ENV PATH=$VCPKG_ROOT:$PATH

# Instala herramientas necesarias para compilar C++ y conectarse a MySQL
RUN apt-get update && apt-get install -y \
    g++ \
    make \
    libmysqlcppconn-dev \
    && vcpkg install nlohmann-json \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app/src

# Copiamos el código fuente
COPY src/ .

# Compilamos el binario
RUN make

# Etapa 2: Imagen final
FROM ubuntu:22.04

ENV DEBIAN_FRONTEND=noninteractive

# Solo instalamos las bibliotecas necesarias para ejecutar
RUN apt-get update && apt-get install -y \
    libmysqlcppconn-dev \
    libstdc++6 \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

# Copiamos el binario desde la imagen de compilación
COPY --from=builder /app/src/endpoint .

EXPOSE 8080

CMD ["sh", "-c", "sleep 10 && ./endpoint"]
