
# 20240313 LF @dosconio
# This script is for Dosconio's looking up websites and https://github.com/ARMinARM/stm32flash
# by CH340 port

IDN = main

MCU = /dev/ttyUSB0
FLS = stm32flash
HEX = $(IDN)/build/$(IDN).hex

all: run clean

run:
	@cd $(IDN) && make
	@echo "write into MCU ..."
	@sudo $(FLS) -w $(HEX) $(MCU) # one time may fail, press RESET may better.
	@sudo $(FLS) -g 0x0 $(MCU)

clean:
	@cd $(IDN) && make clean

