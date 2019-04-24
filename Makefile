all: install

install: build/jwalk

build/jwalk: src/* lib/irrklang.zip
	mkdir -p build
	cd build && cmake ..
	cd build && make

lib/irrklang.zip:
	./download_irrklang.sh
