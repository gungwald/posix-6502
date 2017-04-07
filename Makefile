# Builds POSIX.2-like utils for 6502 machines

# Requirements:
# 1. Gmake must be used.
# 2. The cc65 compiler must be properly setup. See setup-cc65.

ifndef CC65_TARGET
	CC65_TARGET:=apple2enh
endif

CC=cl65
AS=ca65
LD=ld65

CFLAGS=-t $(CC65_TARGET) -O
ASFLAGS=-t $(CC65_TARGET)
LDFLAGS=-t $(CC65_TARGET)

DISK_VOL=posix
DISK=test/$(DISK_VOL).dsk
PGMS=cat chartest date delkeyhndlr vi more uname od hex2dec mapdel2bkspc
BASIC_AUX_TYPE=0x0801
READ_TIME_LOAD_ADDR=0x0260
AC=java -jar lib/AppleCommander-1.3.5.14.jar
SYS_LOAD_ADDR=0x2000
BIN_LOAD_ADDR=0x0803
MKDISK=$(AC) -pro140 $(DISK) $(DISK_VOL)
HD=test/prodos.hdv
ROM=test/ROM.1
ROM_ZIP=test/gsrom01.zip

	#$(AC) -d $(DISK) $@
	#$(AC) -cc65 $(DISK) $@ BIN < $@
#	if [ ! -f $(DISK) ]; \
#	then \
#	    $(MKDISK); \
#	else \
#	    for PGM in $(PGMS); \
#	    do \
#		    $(AC) -d $(DISK) $$PGM; \
#	    done \
#	fi
#	for PGM in $(PGMS); \
#	do \
#	    $(AC) -cc65 $(DISK) $$PGM BIN < $$PGM; \
#	done

########################################

all: $(DISK)

runtest: $(ROM) $(HD) $(DISK)
	(cd test; gsportx $(DISK))

$(ROM): $(ROM_ZIP)
	unzip $(ROM_ZIP)
	mv xgs.rom $(ROM)

$(ROM_ZIP):
	wget --output-document=$(ROM_ZIP) https://mirrors.apple2.org.za/ftp.apple.asimov.net/emulators/rom_images/gsrom01.zip

$(HD):
	wget --output-document=$(HD) https://mirrors.apple2.org.za/ftp.apple.asimov.net/images/masters/prodos/PRODOS_111_1.HDV

$(DISK): $(PGMS)
	cp test/blank-prodos.dsk $(DISK)
	for PGM in $(PGMS); \
	do \
	    $(AC) -cc65 $(DISK) $$PGM BIN < $$PGM; \
	done
	

cat: cat.o
	$(CC) $(LDFLAGS) -o $@ $^

chartest: chartest.o
	$(CC) $(LDFLAGS) -o $@ $^

date: date.o
	$(CC) $(LDFLAGS) -o $@ $^

vi: vi.o
	$(CC) $(LDFLAGS) -o $@ $^

delkeyhndlr: delkeyhndlr.o
	$(CC) $(LDFLAGS) -o $@ $^

more: more.o
	$(CC) $(LDFLAGS) -o $@ $^

uname: uname.o
	$(CC) $(LDFLAGS) -o $@ $^

od: od.o
	$(CC) $(LDFLAGS) -o $@ $^

hex2dec: hex2dec.o
	$(CC) $(LDFLAGS) -o $@ $^

mapdel2bkspc: mapdel2bkspc.o
	$(CC) $(LDFLAGS) -o $@ $^

clean:
	$(RM) *.o $(PGMS) $(DISK) $(HD) $(ROM) $(ROM_ZIP)

