.PHONY: all clean

TAG := adxl362
EXE := $(TAG)
LIB := lib$(TAG).a

DEBUG := 1

CSRC := $(shell find * -name '*.c')
COBJ := $(CSRC:.c=.o)
CHDR := $(shell find * -name '*.h')

OBJ := $(COBJ)

CPPFLAGS :=
CFLAGS :=
LDFLAGS :=

CPPFLAGS += -I.
CPPFLAGS += -Iinclude

CFLAGS += -fPIC
CFLAGS += -Wall -Werror -Wextra
CFLAGS += -Wno-unused-function
CFLAGS += -Wno-unused-const-variable
CFLAGS += -Wno-unused-parameter
CFLAGS += -Wno-unused-variable
CFLAGS += -Wno-sign-compare
ifeq ($(DEBUG),)
CFLAGS += -O3 -fomit-frame-pointer -pipe
else
CFLAGS += -O0 -g -DDEBUG
endif
CFLAGS += -std=c11

LDFLAGS +=

all: $(EXE)

clean:
	rm -f $(OBJ) $(LIB) $(EXE) $(shell find * -name '*.o' -o -name '*.a')

$(LIB): $(OBJ)
	ar cr $@ $^

$(EXE): $(LIB)
	gcc $(LDFLAGS) $(CFLAGS) -o $@ $<

%.o: %.c $(CHDR)
	gcc $(CPPFLAGS) $(CFLAGS) -o $@ -c $<
