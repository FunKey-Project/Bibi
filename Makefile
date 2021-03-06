CC = $(CROSS_COMPILE)gcc -g

# By default, this will compile the standard executable 
# (close to the original source code, with the original levels...)
#
# You can specify these specific platforms though:
# -"funkey": Specific levels, key mappings, and cross compilation params for the FunKey S
# -"funkey_simulated": Same as "funkey" but without the cross compilation so that you can try it on the host computer
# -"funkey_simulated_editor": Same as "funkey_simulated" but with game menu and map editor

ifeq ($(platform), funkey)
	SDL_INCLUDES  = $(shell /opt/FunKey-sdk-2.0.0/arm-funkey-linux-musleabihf/sysroot/usr/bin/sdl-config --cflags)
	SDL_LIBS = $(shell /opt/FunKey-sdk-2.0.0/arm-funkey-linux-musleabihf/sysroot/usr/bin/sdl-config --libs)
	SDL_INCLUDES  += $(shell /opt/FunKey-sdk-2.0.0/arm-funkey-linux-musleabihf/sysroot/usr/bin/libmikmod-config --cflags)
	SDL_LIBS += $(shell /opt/FunKey-sdk-2.0.0/arm-funkey-linux-musleabihf/sysroot/usr/bin/libmikmod-config --libs)
	SDL_LIBS += -Wl,--as-needed -Wl,--gc-sections -Wl,-O1,--sort-common -flto -s
	CFLAGS_EXTRA = -DFUNKEY -DHW_SCREEN_RESIZE -DSOUND_SDL_ACTIVATED -DBYPASS_MENU -DFUNKEY_MENU
	LDFLAGS_EXTRA =  -lSDL_mixer -lSDL_image -lSDL_ttf
else
	SDL_INCLUDES = `sdl-config --cflags`
	SDL_LIBS = `sdl-config --libs`
endif

ifeq ($(platform), funkey_simulated)
	CFLAGS_EXTRA = -DFUNKEY -DHW_SCREEN_RESIZE -DSOUND_SDL_ACTIVATED -DBYPASS_MENU -DFUNKEY_MENU
	LDFLAGS_EXTRA =  -lSDL_mixer -lSDL_image -lSDL_ttf
	SDL_INCLUDES += `libmikmod-config --cflags`
	SDL_LIBS += `libmikmod-config --libs`
endif

ifeq ($(platform), funkey_simulated_editor)
	CFLAGS_EXTRA = -DFUNKEY -DHW_SCREEN_RESIZE -DSOUND_SDL_ACTIVATED
	LDFLAGS_EXTRA =  -lSDL_mixer
	SDL_INCLUDES += `libmikmod-config --cflags`
	SDL_LIBS += `libmikmod-config --libs`
endif

DEPFLAGS = $(SDL_INCLUDES)
CFLAGS = -Wall -O -std=c99 $(SDL_INCLUDES) $(CFLAGS_EXTRA)
LDFLAGS = $(SDL_LIBS) -lSDL_image $(LDFLAGS_EXTRA)

SRC  = $(wildcard src/*.c)
OBJ  = $(SRC:.c=.o) 
EXEC = bibi


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
