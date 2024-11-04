all:
	cp -r src/test.md build/
	cd build/ && cmake .. && make

linux_install:
	sudo apt install libtinyxml2-dev \
	mkdir -p ~/Downloads/ \
	git clone https://github.com/commonmark/cmark.git ~/Downloads/ \
    cd cmark \
    make \
    sudo make install



clear:
	rm -rf build/*