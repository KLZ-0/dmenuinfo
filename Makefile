TARGET			= systatus
SRC				= main.c
BUILD_DIR		= cmake-build-release
INSTALL_DIR		= ~/.local/bin

.PHONY: all install clean
all: $(TARGET)

$(TARGET): $(BUILD_DIR)
	make -C $<
	cp $</$@ ./

$(BUILD_DIR): $(SRC)
	mkdir $@
	cd $@ && cmake -D CMAKE_BUILD_TYPE=Release ..

install: $(TARGET)
	mkdir -p $(INSTALL_DIR)
	cp $< $(INSTALL_DIR)/$<

clean:
	rm -r $(BUILD_DIR)
	rm -r $(TARGET)
