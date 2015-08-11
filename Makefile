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
LDFLAGS=-t $(CC65_TARGET) -m $(PROGRAM).map
DISK=posix.dsk
PGMS=cat chartest date delkey.hndlr vi
BASIC_AUX_TYPE=0x0801
READ_TIME_LOAD_ADDR=0x0260
AC=java -jar ../lib/AppleCommander-1.3.5.14-ac.jar
SYS_LOAD_ADDR=0x2000
BIN_LOAD_ADDR=0x0803
MKDISK=$(DISK_MGR) -pro140 $(DISK) $(DISK_VOLUME_NAME)

########################################

all: $(DISK)

$(DISK): $(PGMS) 
	if [! -f $(DISK) ]; then $(MKDISK); fi
	for PGM in $(PGMS); \
	do \
		$(AC) -d $(DISK) $PGM; \
		$(AC) -cc65 $(DISK) $PGM bin $(BIN_LOAD_ADDR) < $PGM; \
	done

cat: cat.c

chartest: chartest.c

date: date.c

vi: vi.c

delkey.hndlr: delkey.hndlr.c

%: %.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	$(RM) $(PGMS)

