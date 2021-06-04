CC = $(CROSS_COMPILE)gcc -g


TARGET_FUNKEY ?= 1


ifeq ($(TARGET_FUNKEY),1)
	SDL_INCLUDES  = $(shell /opt/FunKey-sdk-2.0.0/arm-funkey-linux-musleabihf/sysroot/usr/bin/sdl-config --cflags)
	SDL_LIBS = $(shell /opt/FunKey-sdk-2.0.0/arm-funkey-linux-musleabihf/sysroot/usr/bin/sdl-config --libs) -Wl,--as-needed -Wl,--gc-sections -Wl,-O1,--sort-common -flto -s
else
	SDL_INCLUDES = `sdl-config --cflags`
	SDL_LIBS = `sdl-config --libs`
endif

DEPFLAGS = $(SDL_INCLUDES)
CFLAGS = -Wall -O -std=c99 $(SDL_INCLUDES) 
LDFLAGS = $(SDL_LIBS) -lSDL_image -lSDL_mixer

SRC  = $(wildcard src/*.c)
OBJ  = $(SRC:.c=.o) 
EXEC = bomber


.PHONY: all dep clean mrproper

all : dep $(EXEC)
dep: .depend
.depend: $(SRC)
	@touch .depend
	$(CC) -MM $(DEPFLAGS) $(SRC) > $@

$(EXEC): $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS) 

%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	-rm -f *~ $(OBJ)

mrproper: clean
	-rm -f *~ $(EXEC) .depend


ifneq ($(wildcard .depend),)
include .depend
endif
