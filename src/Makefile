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
DISK=$(DISK_VOL).dsk
#DISK=../test/testhd.hdv
PGMS=textfileexample cat chartest date delkeyhndlr vi more uname od hex2dec mapdel2bkspc helloworld
BASIC_AUX_TYPE=0x0801
READ_TIME_LOAD_ADDR=0x0260
AC=java -jar ../lib/AppleCommander-1.3.5.14.jar
SYS_LOAD_ADDR=0x2000
BIN_LOAD_ADDR=0x0803

########################################

all: $(PGMS)

test: $(PGMS)
	(cd ../test; make)

textfileexample: textfileexample.o $(DISK)
	$(CC) $(LDFLAGS) -o $@ $<
	$(AC) -d $(DISK) $@
	$(AC) -cc65 $(DISK) $@ BIN < $@

cat: cat.o $(DISK)
	$(CC) $(LDFLAGS) -o $@ $<
	$(AC) -d $(DISK) $@
	$(AC) -cc65 $(DISK) $@ BIN < $@

chartest: chartest.o $(DISK)
	$(CC) $(LDFLAGS) -o $@ $<
	$(AC) -d $(DISK) $@
	$(AC) -cc65 $(DISK) $@ BIN < $@

date: date.o $(DISK)
	$(CC) $(LDFLAGS) -o $@ $<
	$(AC) -d $(DISK) $@
	$(AC) -cc65 $(DISK) $@ BIN < $@

vi: vi.o $(DISK)
	$(CC) $(LDFLAGS) -o $@ $<
	$(AC) -d $(DISK) $@
	$(AC) -cc65 $(DISK) $@ BIN < $@

delkeyhndlr: delkeyhndlr.o $(DISK)
	$(CC) $(LDFLAGS) -o $@ $<
	$(AC) -d $(DISK) $@
	$(AC) -cc65 $(DISK) $@ BIN < $@

more: more.o $(DISK)
	$(CC) $(LDFLAGS) -o $@ $<
	$(AC) -d $(DISK) $@
	$(AC) -cc65 $(DISK) $@ BIN < $@

uname: uname.o $(DISK)
	$(CC) $(LDFLAGS) -o $@ $<
	$(AC) -d $(DISK) $@
	$(AC) -cc65 $(DISK) $@ BIN < $@

od: od.o $(DISK)
	$(CC) $(LDFLAGS) -o $@ $<
	$(AC) -d $(DISK) $@
	$(AC) -cc65 $(DISK) $@ BIN < $@

hex2dec: hex2dec.o $(DISK)
	$(CC) $(LDFLAGS) -o $@ $<
	$(AC) -d $(DISK) $@
	$(AC) -cc65 $(DISK) $@ BIN < $@

mapdel2bkspc: mapdel2bkspc.o $(DISK)
	cl65 $< --target none --start-addr 0x0803 -o $@
	$(AC) -d $(DISK) $@
	$(AC) -p $(DISK) $@ BIN 0x0803 < $@

helloworld: helloworld.o $(DISK)
	cl65 $< --target none --start-addr 0x0803 -o $@
	$(AC) -d $(DISK) $@
	$(AC) -p $(DISK) $@ BIN 0x0803 < $@

helloworld.o: helloworld.s
	ca65 -l $<.asmlist -D BASE=0X0803 $<

helloworld2: helloworld2.o $(DISK)
	ld65 -t $(CC65_TARGET) -o $@ $<
	$(AC) -d $(DISK) $@
	$(AC) -cc65 $(DISK) $@ BIN < $@

helloworld2.o: helloworld2.s
	ca65 -t $(CC65_TARGET) $<

$(DISK):
	$(AC) -pro140 $(DISK) $(DISK_VOL)

clean:
	$(RM) *.o $(PGMS)
	for PGM in $(PGMS); \
	do \
	    $(AC) -d $(DISK) $$PGM; \
	done

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

