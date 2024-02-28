NAME=test
DEVICE=atmega2560
BUILDDIR=build

CC=avr-gcc
CXX=avr-g++
OBJCOPY=avr-objcopy
CFLAGS=-std=c99 -Wall -Os -fdata-sections -ffunction-sections
CXXFLAGS=-std=c++11 -Wall -Os -fdata-sections -ffunction-sections
CPPFLAGS=-DF_CPU=16000000 -mmcu=$(DEVICE)
LDFLAGS=-lc -Wl,-gc-sections

CFILES = $(wildcard *.c)
COBJS := $(patsubst %.c,$(BUILDDIR)/%.o,$(CFILES))

CPPFILES = $(wildcard *.cpp)
CPPOBJS := $(patsubst %.cpp,$(BUILDDIR)/%.o,$(CPPFILES))

all: dir $(BUILDDIR)/$(NAME).hex

dir:
	mkdir -p $(BUILDDIR)

$(BUILDDIR)/%.o: %.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -c -o $@ $<

$(BUILDDIR)/%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c -o $@ $<

$(BUILDDIR)/ws2812b.o: ws2812b.S
	$(CC) $(CFLAGS) $(CPPFLAGS) -E -o $(BUILDDIR)/ws2812b.s ws2812b.S
	$(CC) $(CFLAGS) $(CPPFLAGS) -c -o $(BUILDDIR)/ws2812b.o $(BUILDDIR)/ws2812b.s

$(BUILDDIR)/$(NAME).elf: $(COBJS) $(CPPOBJS) $(BUILDDIR)/ws2812b.o
	$(CC) $(CFLAGS) $(CPPFLAGS) -o $(BUILDDIR)/$(NAME).elf $(COBJS) $(CPPOBJS) $(BUILDDIR)/ws2812b.o $(LDFLAGS)

$(BUILDDIR)/$(NAME).hex: $(BUILDDIR)/$(NAME).elf
	avr-size --mcu=$(DEVICE) -C --format=avr $(BUILDDIR)/$(NAME).elf
	$(OBJCOPY) -j .text -j .data -O ihex $(BUILDDIR)/$(NAME).elf $(BUILDDIR)/$(NAME).hex
	cp $(BUILDDIR)/$(NAME).hex /sdcard/ArduinoDroid/precompiled

clean:
	rm -r $(BUILDDIR)
