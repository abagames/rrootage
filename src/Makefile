# rRootage makefile
# gcc version 7.2.0 (Rev1, Built by MSYS2 project)
#  Target: i686-w64-mingw32

NAME   = rr
O      = o
RM     = rm -f
CC     = gcc
CXX    = g++

PROG   = $(NAME).exe

DEFAULT_CFLAGS = `sdl-config --cflags`
LDFLAGS        = -static-libgcc -static-libstdc++ -Wl,-Bstatic,--whole-archive -lwinpthread -Wl,--no-whole-archive -lglu32 -lopengl32 -lmingw32 -L/usr/lib -lSDLmain -L. -lbulletml -Wl,-Bdynamic -L.. -lSDL -lSDL_mixer -mwindows

MORE_CFLAGS = -O2 -Wall

CFLAGS   = $(DEFAULT_CFLAGS) $(MORE_CFLAGS)
CXXFLAGS = $(DEFAULT_CFLAGS) $(MORE_CFLAGS) -I./bulletml/

OBJS =	$(NAME).$(O) \
	foe.$(O) foecommand.$(O) barragemanager.$(O) boss.$(O) ship.$(O) laser.$(O) \
	frag.$(O) background.$(O) letterrender.$(O) shot.$(O) \
	screen.$(O) vector.$(O) degutil.$(O) rand.$(O) mt19937int.$(O) \
	soundmanager.$(O) attractmanager.$(O) \
	$(NAME)_res.$(O)

$(PROG): $(OBJS) 
	$(CXX) -o $(PROG) $(OBJS) $(LDFLAGS)
	mv $(PROG) ..

$(NAME)_res.o: $(NAME).rc
	windres -i $(NAME).rc -o $(NAME)_res.o

clean:
	$(RM) ../$(PROG) *.$(O)
