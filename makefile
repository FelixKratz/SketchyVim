VERSION = "1.0.7"
CC = clang
DEFINES = -DHAVE_CONFIG_H -DMACOS_X -DMACOS_X_DARWIN #-DGUI_MOVES
LIBS = lib/libvim.a -lm -lncurses -liconv -framework Carbon -framework Cocoa
WARN_FLAGS = -Wall -Wno-array-bounds \
	     -Wno-unknown-warning-option \
	     -Wno-cpp -Wno-pointer-sign \
	     -Wno-unused-parameter \
	     -Wno-strict-overflow \
	     -Wno-return-type -Werror
CFLAGS = $(WARN_FLAGS) $(DEFINES) -g -Ilib -Ilib/libvim/proto -std=c99 -O2 #-fsanitize=address -fsanitize=undefined
ODIR = bin
SRC = src

_OBJ = helpers.om workspace.om event_tap.o ax.o buffer.o line.o env_vars.o
OBJ = $(patsubst %, $(ODIR)/%, $(_OBJ))

.PHONY: all x86 arm64 universal sign lib clean

all: $(ODIR)/svim

x86: CFLAGS = $(WARN_FLAGS) $(DEFINES) -g -Ilib -Ilib/libvim/proto -std=c99 -O2 -target x86_64-apple-macos12.0
x86: $(ODIR)/svim
	mv $(ODIR)/svim $(ODIR)/svim_x86
	rm -rf $(ODIR)/*.o
	rm -rf $(ODIR)/*.om

arm64: CFLAGS = $(WARN_FLAGS) $(DEFINES) -g -Ilib -Ilib/libvim/proto -std=c99 -O2 -target arm64-apple-macos12.0
arm64: $(ODIR)/svim
	mv $(ODIR)/svim $(ODIR)/svim_arm64
	rm -rf $(ODIR)/*.o
	rm -rf $(ODIR)/*.om

universal:
	$(MAKE) x86
	$(MAKE) arm64
	lipo -create -output $(ODIR)/svim $(ODIR)/svim_x86 $(ODIR)/svim_arm64

sign:
	$(MAKE) universal
	codesign -fs 'svim-cert' $(ODIR)/svim

bundle: clean
	$(MAKE) sign
	@mkdir bundle
	cp $(ODIR)/svim bundle/
	cp -r examples/ bundle/
	tar -czf bundle_$(VERSION).tgz bundle/
	rm -rf bundle/

lib:
	cd libvim/src/ && make
	cp libvim/src/libvim.a lib/libvim.a

bin/svim: $(SRC)/main.m $(OBJ) | $(ODIR)
	$(CC) $(CFLAGS) $^ -o $(ODIR)/svim $(LIBS)

$(ODIR)/%.o: $(SRC)/%.c $(SRC)/%.h | $(ODIR)
	$(CC) -c -o $@ $< $(CFLAGS)

$(ODIR)/%.om: $(SRC)/%.m $(SRC)/%.h | $(ODIR)
	$(CC) -c -o $@ $< $(CFLAGS)

$(ODIR):
	mkdir $(ODIR)

.PHONY: clean

clean:
	rm -rf $(ODIR)
