OBJ = obj/main.o obj/map.o obj/hash.o obj/map-render.o obj/generator.o
BINDIR = bin/
OBJDIR = obj/

all: bin/out

bin/out: $(OBJ) | $(BINDIR) $(OBJDIR)
	$(CC) $(CFLAGS) -o $@ $(OBJ)

obj/main.o: src/main.c src/map.h src/hash.h | $(OBJDIR)
	$(CC) $(CFLAGS) -c -o $@ src/main.c

obj/map.o: src/map.c src/map.h src/tectonics.h | $(OBJDIR)
	$(CC) $(CFLAGS) -c -o $@ src/map.c

obj/map-render.o: src/map-render.c src/map.h | $(OBJDIR)
	$(CC) $(CFLAGS) -c -o $@ src/map-render.c

obj/hash.o: src/hash.c src/hash.h | $(OBJDIR)
	$(CC) $(CFLAGS) -c -o $@ src/hash.c

obj/generator.o: src/generator.c src/generator.h src/tectonics.h src/hash.h \
src/map.h | $(OBJDIR)
	$(CC) $(CFLAGS) -c -o $@ src/generator.c

$(BINDIR) $(OBJDIR):
	mkdir -p $@