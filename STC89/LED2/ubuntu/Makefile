# ASCII C-SDCC TAB4 LF
# Attribute: CPU(STC89)
# LastCheck: 20240303
# AllAuthor: @dosconio
# ModuTitle: LCD1602
# Copyright: 

AS   = sdas8051
CC   = sdcc -I/mnt/hgfs/unisym/inc/c
ULIB = /mnt/hgfs/unisym/lib
PREP = sdcpp

MCU = /dev/ttyUSB0
FLS = stcgal -P stc89 -p $(MCU)

DIR = ~/_obj
IDN = stc89led2
OUT = $(DIR)/$(IDN).ihx
HEX = $(DIR)/$(IDN).hex

.PHONY: defa all main delay port lp clean

defa: main delay port
	@$(CC) -o $(OUT) $(DIR)/$(IDN).rel $(DIR)/delay.rel $(DIR)/port.rel
	@packihx $(OUT) > $(HEX)

all: clean defa
	@sudo $(FLS) $(HEX)

main: led2ubt.c
	@$(CC) -o $(DIR)/$(IDN).rel -c $(<)

delay: $(ULIB)/asm/8051/delay11M.asm
	@$(AS) -o $(DIR)/delay.rel $(<)
	@$(AS) -l $(DIR)/delay.lst $(<)

port: $(ULIB)/c/8051/port.c
	@$(CC) -o $(DIR)/port.rel -c $(<)
	

lp: # list ports
	ls /dev/tty*

clean:
	-rm -f $(DIR)/*.rel
	-rm -f $(DIR)/*.ihx
	-rm -f $(DIR)/*.hex
	-rm -f $(DIR)/*.lk
	-rm -f $(DIR)/*.lst
	-rm -f $(DIR)/*.asm
	-rm -f $(DIR)/*.mem
	-rm -f $(DIR)/*.map
	-rm -f $(DIR)/*.rst
	-rm -f $(DIR)/*.sym
