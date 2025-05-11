CXX = g++
CXXFLAGS = -std=c++17 -O3

INCLUDES = -I/opt/mongocxx/include \
           -I/opt/mongocxx/include/bsoncxx/v_noabi \
           -I/opt/mongocxx/include/mongocxx/v_noabi \
           -I/app/Crow/include \
           -I/opt/jwt-cpp/include  

LIBS = -L/opt/mongocxx/lib \
       -lmongocxx -lbsoncxx \
       -lmongoc2 -lbson2 \
       -lssl -lcrypto -lz -lpthread -lbcrypt \
       -Wl,-rpath,/opt/mongocxx/lib

SOURCES = $(wildcard *.cpp)
TARGET = app

$(TARGET): $(SOURCES)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(SOURCES) -o $(TARGET) $(LIBS)

.PHONY: clean
clean:
	rm -f $(TARGET)
