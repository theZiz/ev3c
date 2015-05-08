CFLAGS = -O3
ifndef RANLIB
	RANLIB = ranlib
endif

all: lib/ev3c.a bin/test_sensor bin/test_motor bin/test_lcd

obj/%.o: %.c ev3c.h Makefile
	mkdir -p $(@D)
	$(CC) -c -o $@ $< $(CFLAGS)

lib/ev3c.a: obj/ev3c.o
	mkdir -p $(@D)
	$(AR) rc $@ $^ && $(RANLIB) $@

bin/test_sensor: test_sensor.c lib/ev3c.a
	mkdir -p $(@D)
	$(CC) -o $@ $< lib/ev3c.a $(CFLAGS)

bin/test_motor: test_motor.c lib/ev3c.a
	mkdir -p $(@D)
	$(CC) -o $@ $< lib/ev3c.a $(CFLAGS)

bin/test_lcd: test_lcd.c lib/ev3c.a
	mkdir -p $(@D)
	$(CC) -o $@ $< lib/ev3c.a $(CFLAGS)

documentation: *.h
	mkdir -p /tmp/ev3DocumentationTemp
	naturaldocs -i . -xi ./ev3c_documentation -o HTML ev3c_documentation -p /tmp/ev3DocumentationTemp

.PHONY: all clean

clean:
	rm -rf obj lib bin
	rm -f *~
