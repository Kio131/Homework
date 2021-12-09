TARGET = ./main.out
VALGRIND_LOG = "valgrind.log"

HDRS = \
		include

SRCS = \
	   src/main.c \
	   src/resources.c

all: build run

build: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) -Wall -Wextra -Werror $(addprefix -I,$(HDRS)) -o $(TARGET) $(CFLAGS) $(SRCS)

run: 
	$(TARGET)

clean:
	rm -f $(TARGET) ${VALGRIND_LOG}