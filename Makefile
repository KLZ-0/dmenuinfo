TARGET			= systatus
BUILD_DIR		= cmake-build-release
INSTALL_DIR		= ~/.local/bin

.PHONY: all install clean $(TARGET)
all: $(TARGET)

$(TARGET): $(BUILD_DIR)
	$(MAKE) -C $<
	cp $</$@ ./

$(BUILD_DIR): CMakeLists.txt
	mkdir -p $@
	cd $@ && cmake -D CMAKE_BUILD_TYPE=Release ..

install: $(TARGET)
	mkdir -p $(INSTALL_DIR)
	cp $< $(INSTALL_DIR)/$<

clean:
	rm -rf $(BUILD_DIR)
	rm -rf $(TARGET)
