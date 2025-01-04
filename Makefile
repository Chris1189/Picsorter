SRCDIR := .
PATTERN := *.JPEG

picsorter: tmp/main.o tmp/ps_fs.o build
	gcc -o build/picsorter tmp/main.o tmp/ps_fs.o

tmp/ps_fs.o: src/ps_fs.c tmp 
	gcc -c src/ps_fs.c -o tmp/ps_fs.o

tmp/main.o: src/main.c tmp
	gcc -c src/main.c -o tmp/main.o

build:
	mkdir -p build

tmp:
	mkdir -p tmp

rename:
	find $(SRCDIR) -type f -iname "$(PATTERN)" | while read -r file; do \
	  dir=$$(dirname "$$file"); \
	  mv "$$file" "$$dir/untitled.jpeg"; \
	done

tempclean: tmp
	rm -rf tmp

.PHONY: clean
clean:
	rm -rf build tmp
