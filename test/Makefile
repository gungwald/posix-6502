# Builds POSIX.2-like utils for 6502 machines

DISK_VOL=posix
DISK=../src/$(DISK_VOL).dsk
BASIC_AUX_TYPE=0x0801
READ_TIME_LOAD_ADDR=0x0260
AC=java -jar ../lib/AppleCommander-1.3.5.14.jar
SYS_LOAD_ADDR=0x2000
BIN_LOAD_ADDR=0x0803
MKDISK=$(AC) -pro140 $(DISK) $(DISK_VOL)
HD=testhd.hdv
ROM=ROM.1
ROM_ZIP=gsrom01.zip
PRODOS_DSK=prodos-2.4.1.dsk
ROM_URL=https://mirrors.apple2.org.za/ftp.apple.asimov.net/emulators/rom_images/gsrom01.zip
PRODOS_URL=https://mirrors.apple2.org.za/ftp.apple.asimov.net/images/masters/prodos/ProDOS_2_4_1.dsk
HD_URL=https://mirrors.apple2.org.za/ftp.apple.asimov.net/images/masters/prodos/PRODOS_111_1.HDV

########################################

all: test

test: $(ROM) $(HD) $(DISK)
	gsportx $(DISK)

$(ROM): xgs.rom
	mv xgs.rom $(ROM)

xgs.rom: $(ROM_ZIP)
	@if [ ! -f xgs.rom ]; then unzip $(ROM_ZIP); fi
	@# The ROM is old so force make only to run the rule if it is absent.
	
$(ROM_ZIP):
	wget --output-document=$(ROM_ZIP) $(ROM_URL)

$(PRODOS_DSK):
	wget --output-document=$(PRODOS_DSK) $(PRODOS_URL)

$(HD): PRODOS QUIT.SYSTEM BITSY.BOOT BASIC.SYSTEM COPYIIPLUS.7.2
	$(AC) -d $(HD) PRODOS
	#$(AC) -d $(HD) QUIT.SYSTEM
	#$(AC) -d $(HD) BITSY.BOOT
	$(AC) -d $(HD) BASIC.SYSTEM
	$(AC) -d $(HD) COPYIIPLUS.7.2
	$(AC) -p $(HD) PRODOS SYS < PRODOS
	#$(AC) -p $(HD) QUIT.SYSTEM SYS < QUIT.SYSTEM
	#$(AC) -p $(HD) BITSY.BOOT SYS < BITSY.BOOT
	$(AC) -p $(HD) BASIC.SYSTEM SYS < BASIC.SYSTEM
	$(AC) -p $(HD) COPYIIPLUS.7.2 SYS < COPYIIPLUS.7.2

PRODOS: $(PRODOS_DSK)
	$(AC) -g $(PRODOS_DSK) PRODOS PRODOS

QUIT.SYSTEM: $(PRODOS_DSK)
	$(AC) -g $(PRODOS_DSK) QUIT.SYSTEM QUIT.SYSTEM

BITSY.BOOT: $(PRODOS_DSK)
	$(AC) -g $(PRODOS_DSK) BITSY.BOOT BITSY.BOOT

BASIC.SYSTEM: $(PRODOS_DSK)
	$(AC) -g $(PRODOS_DSK) BASIC.SYSTEM BASIC.SYSTEM

COPYIIPLUS.7.2: $(PRODOS_DSK)
	$(AC) -g $(PRODOS_DSK) COPYIIPLUS.7.2 COPYIIPLUS.7.2

clean:
	$(RM) $(ROM) $(ROM_ZIP) $(PRODOS_DSK) PRODOS QUIT.SYSTEM BASIC.SYSTEM BITSY.BOOT COPYIIPLUS.7.2 xgs.rom
