.SILENT:

include C:/AndroXStudio/Tools/make_comm

all: $(OBJECTS)
	$(CC) $(CFLAGS) $(EXEC_LDFLAGS) -save-temps -O2 -o CServer $(OBJECTS) \
		$(LLIB)/libgcc.a

	echo 
	echo "**** Install:" /data/local/tmp/CServer "****"
	$(ADB) push CServer /data/local/tmp
	$(ADB) shell chmod 755 /data/local/tmp/CServer

clean:
	rm -f *.o
	rm -f *.i
	rm -f *.s
	rm -f CServer
