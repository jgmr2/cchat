FROM gcc:latest

# Agregar lsb-release para obtener la versión de Debian
RUN apt-get update && apt-get install -y lsb-release wget gnupg ca-certificates

# Añadir la clave pública de MongoDB
RUN wget -qO - https://www.mongodb.org/static/pgp/server-6.0.asc | apt-key add -

# Configurar el repositorio MongoDB para Debian 11 (Bullseye)
RUN echo "deb [ arch=amd64 ] https://repo.mongodb.org/apt/debian bullseye/mongodb-org/6.0 main" | tee /etc/apt/sources.list.d/mongodb-org-6.0.list

# Actualizar apt y luego instalar las dependencias necesarias
RUN 	apt-get update && apt-get install -y \
    	git \
  	wget \
    	curl \
    	libssl-dev \
    	pkg-config \
    	make \
    	libblas-dev \
	gfortran- \
	libboost-all-dev \
    	libmongoc-dev \
    	libboost-all-dev \
		libcpp-httplib-dev \
    	&& rm -rf /var/lib/apt/lists/*  # Limpiar la caché de apt

# Establecer el directorio de trabajo
WORKDIR /workspace

