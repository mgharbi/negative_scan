ARCHIVE = halide-mac-64-trunk-46d8e9e0cdae456489f1eddfd6d829956fc3c843.tgz

unzip_halide: $(ARCHIVE)
	tar zvxf $(ARCHIVE)

$(ARCHIVE):
	@echo Downloading Halide
	wget https://github.com/halide/Halide/releases/download/release_2018_02_15/$(ARCHIVE)
