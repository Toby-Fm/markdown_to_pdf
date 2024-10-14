all:
	mkdir -p build && cd build/ && cmake .. && make

linux_install:
	sudo apt install libtinyxml2-dev


clear:
	rm -rf build/*