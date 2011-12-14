GTK_VER = 3.0
#GTK_VER = 2.0

CFLAGS = `pkg-config --cflags gtk+-$(GTK_VER)`
LIBS = `pkg-config --libs gtk+-$(GTK_VER)`

gclip:
	$(CC) $(CFLAGS) -o $@ $(LIBS) gclip.c

clean:
	rm -rf gclip

