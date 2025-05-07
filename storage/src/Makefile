CXX = g++
CXXFLAGS = -std=c++17 -O3
INCLUDES = -I/opt/mongocxx/include \
           -I/opt/mongocxx/include/bsoncxx/v_noabi \
           -I/opt/mongocxx/include/mongocxx/v_noabi
LIBS = -L/opt/mongocxx/lib \
       -lmongocxx -lbsoncxx -lssl -lcrypto -lz -lpthread -lbcrypt

SOURCES = $(wildcard *.cpp)
TARGET = app

$(TARGET): $(SOURCES)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(SOURCES) -o $(TARGET) $(LIBS)

.PHONY: clean
clean:
	rm -f $(TARGET)
