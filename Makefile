picsorter: tmp/main.o tmp/ps_fs.o tmp/ps_smb.o build
	gcc -o build/picsorter tmp/main.o tmp/ps_fs.o tmp/ps_smb.o -lsmbclient

tmp/ps_fs.o: src/ps_fs.c tmp 
	gcc -c src/ps_fs.c -o tmp/ps_fs.o

tmp/ps_smb.o: src/ps_smb.c tmp
	gcc -c src/ps_smb.c -o tmp/ps_smb.o

tmp/main.o: src/main.c tmp
	gcc -c src/main.c -o tmp/main.o

build:
	mkdir -p build

restore:
	git restore .

tmp:
	mkdir -p tmp

tempclean: tmp
	rm -rf tmp

.PHONY: clean
clean:
	rm -rf build tmp
