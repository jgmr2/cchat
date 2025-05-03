CXX = g++
CXXFLAGS = -Wall -std=c++11

# Rutas a las bibliotecas
MYSQL_LIB_PATH = /usr/include/cppconn
MYSQL_LIB = -lmysqlcppconn

# Ruta de vcpkg (ajústala si tu instalación es distinta)
VCPKG_INCLUDE = /vcpkg/installed/x64-linux/include

# Archivos fuente
SRC = endpoint.cpp

# Archivo de salida
OUT = endpoint

all:
	$(CXX) $(CXXFLAGS) -I$(MYSQL_LIB_PATH) -I$(VCPKG_INCLUDE) $(SRC) -o $(OUT) $(MYSQL_LIB)
clean:
	rm -f $(OUT)