OBJS = main.o GravityHandler.o Planet.o
CC = g++
CFLAGS = -Wall -c
XFLAGS = -Wall
LFLAGS = -lSDL -lSDL_image

gravity: $(OBJS)
	$(CC) $(XFLAGS) $(OBJS) -o gravity $(LFLAGS)
sdl_test.o: main.cpp GravityHandler.h Earth.h Moon.h Vector2d.h
	$(CC) $(CFLAGS) sdl_test.cpp $(LFLAGS)
GravityHandler.o: GravityHandler.cpp GravityHandler.h Vector2d.h
	$(CC) $(CFLAGS) GravityHandler.cpp
Planet.o: Planet.cpp Planet.h Vector2d.h
	$(CC) $(CFLAGS) Planet.cpp $(LFLAGS)
clean:
	\rm *.o *~ gravity