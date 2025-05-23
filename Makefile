OBJ = obj/main.o obj/map.o obj/hash.o
BINDIR = bin/
OBJDIR = obj/

all: bin/out
	mkdir -p bin
	mkdir -p obj

bin/out: $(OBJ) | $(BINDIR) $(OBJDIR)
	$(CC) $(CFLAGS) -o bin/out $(OBJ)

obj/main.o: src/main.c src/map.h src/hash.h | $(OBJDIR)
	$(CC) $(CFLAGS) -c -o obj/main.o src/main.c

obj/map.o: src/map.c src/map.h | $(OBJDIR)
	$(CC) $(CFLAGS) -c -o obj/map.o src/map.c

obj/hash.o: src/hash.c src/hash.h | $(OBJDIR)
	$(CC) $(CFLAGS) -c -o obj/hash.o src/hash.c

$(BINDIR) $(OBJDIR):
	mkdir -p $@