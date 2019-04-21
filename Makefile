all: install

install: build/jwalk

build/jwalk: src/* lib/irrklang
	mkdir -p build
	cd build && cmake ..
	cd build && make

lib/irrklang:
	./download_irrklang.sh
