# Builds POSIX.2-like utils for 6502 machines
#

# Requirements:
# 1. Gmake must be used.
# 2. The cc65 compiler must be properly setup. See setup-cc65.
#

ifndef CC65_TARGET
	CC65_TARGET:=apple2enh
endif

CC=cl65
CFLAGS=-O -t $(CC65_TARGET)
LDFLAGS=-t $(CC65_TARGET)
DISK_VOL=posix
DISK=$(DISK_VOL).dsk
PGMS=cat chartest date delkeyhndlr vi more uname od hex2dec
BASIC_AUX_TYPE=0x0801
READ_TIME_LOAD_ADDR=0x0260
AC=java -jar lib/AppleCommander-1.3.5.14.jar
SYS_LOAD_ADDR=0x2000
BIN_LOAD_ADDR=0x0803
MKDISK=$(AC) -pro140 $(DISK) $(DISK_VOL)

########################################

all: $(PGMS)

$(DISK): $(PGMS) 
	if [ ! -f $(DISK) ]; \
	then \
	    $(MKDISK); \
	else \
	    for PGM in $(PGMS); \
	    do \
		    $(AC) -d $(DISK) $$PGM; \
	    done \
	fi
	for PGM in $(PGMS); \
	do \
		$(AC) -cc65 $(DISK) $$PGM BIN < $$PGM; \
	done

cat: cat.o
	$(CC) $(LDFLAGS) -o $@ $^
	$(AC) -d $(DISK) $@
	$(AC) -cc65 $(DISK) $@ BIN < $@

chartest: chartest.o
	$(CC) $(LDFLAGS) -o $@ $^
	$(AC) -d $(DISK) $@
	$(AC) -cc65 $(DISK) $@ BIN < $@

date: date.o
	$(CC) $(LDFLAGS) -o $@ $^
	$(AC) -d $(DISK) $@
	$(AC) -cc65 $(DISK) $@ BIN < $@

vi: vi.o
	$(CC) $(LDFLAGS) -o $@ $^
	$(AC) -d $(DISK) $@
	$(AC) -cc65 $(DISK) $@ BIN < $@

delkeyhndlr: delkeyhndlr.o
	$(CC) $(LDFLAGS) -o $@ $^
	$(AC) -d $(DISK) $@
	$(AC) -cc65 $(DISK) $@ BIN < $@

more: more.o
	$(CC) $(LDFLAGS) -o $@ $^
	$(AC) -d $(DISK) $@
	$(AC) -cc65 $(DISK) $@ BIN < $@

uname: uname.o
	$(CC) $(LDFLAGS) -o $@ $^
	$(AC) -d $(DISK) $@
	$(AC) -cc65 $(DISK) $@ BIN < $@

od: od.o
	$(CC) $(LDFLAGS) -o $@ $^
	$(AC) -d $(DISK) $@
	$(AC) -cc65 $(DISK) $@ BIN < $@

hex2dec: hex2dec.o
	$(CC) $(LDFLAGS) -o $@ $^
	$(AC) -d $(DISK) $@
	$(AC) -cc65 $(DISK) $@ BIN < $@

clean:
	$(RM) *.o $(PGMS)

