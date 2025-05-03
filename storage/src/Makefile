# Makefile

CC = g++
CFLAGS = -static -Wall
TARGET = endpoint
SRC = endpoint.cpp

# Regla por defecto para compilar el ejecutable
$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

# Limpieza de archivos generados
clean:
	rm -f $(TARGET)
