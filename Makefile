CC  = 	g++
CFLAGS = -O2 -g -Wall -c
SOURCES = BitmapFont.cpp
OBJECTS = $(SOURCES:.cpp=.o)
TARGET = BitmapFont

all: $(SOURCES) $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf $(OBJECTS) $(TARGET)
