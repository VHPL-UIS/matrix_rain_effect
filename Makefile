CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -O2

TARGET = matrix_rain

SOURCES = main.cpp MatrixRain.cpp MenuInterface.cpp

HEADERS = MatrixRain.hpp MenuInterface.hpp

OBJECTS = $(SOURCES:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJECTS)
	@echo "Build complete! Run with: ./$(TARGET)"

%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)
	@echo "Clean complete!"

install: $(TARGET)
	sudo cp $(TARGET) /usr/local/bin/
	@echo "Installed to /usr/local/bin/$(TARGET)"

uninstall:
	sudo rm -f /usr/local/bin/$(TARGET)
	@echo "Uninstalled from /usr/local/bin/"

debug: CXXFLAGS += -g -DDEBUG
debug: $(TARGET)

release: CXXFLAGS += -O3 -DNDEBUG
release: clean $(TARGET)

run: $(TARGET)
	./$(TARGET)

format:
	clang-format -i *.cpp *.h

help:
	@echo "Available targets:"
	@echo "  all      - Build the project (default)"
	@echo "  clean    - Remove build artifacts"
	@echo "  debug    - Build with debug symbols"
	@echo "  release  - Build optimized release version"
	@echo "  run      - Build and run the program"
	@echo "  install  - Install to system (requires sudo)"
	@echo "  uninstall- Remove from system (requires sudo)"
	@echo "  format   - Format code (requires clang-format)"
	@echo "  help     - Show this help message"

.PHONY: all clean install uninstall debug release run format help