CC=gcc
CFLAGS=-c -Wall -g
LDFLAGS=-ljpeg -lm -lpthread
SOURCES= mandel.c jpegrw.c mandelmovie.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=mandelmovie.out

all: $(SOURCES) $(EXECUTABLE) 

# pull in dependency info for *existing* .o files
-include $(OBJECTS:.o=.d)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@

.c.o: 
	$(CC) $(CFLAGS) $< -o $@
	$(CC) -MM $< > $*.d

clean:
	rm -rf $(OBJECTS) $(EXECUTABLE) *.d
