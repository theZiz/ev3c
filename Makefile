CFLAGS += -O3
ifndef RANLIB
	RANLIB = ranlib
endif

all: lib/ev3c.a bin/test_sensor bin/test_motor bin/test_lcd bin/test_button bin/test_led bin/test_battery

obj/%.o: src/%.c include/%.h include/ev3c.h Makefile
	mkdir -p $(@D)
	$(CC) -c -o $@ $< $(CFLAGS) -Iinclude

lib/ev3c.a: obj/ev3c_core.o obj/ev3c_lcd.o obj/ev3c_sensor.o obj/ev3c_motor.o obj/ev3c_button.o obj/ev3c_led.o obj/ev3c_battery.o
	mkdir -p $(@D)
	$(AR) rc $@ $^ && $(RANLIB) $@

bin/test_sensor: src/test_sensor.c lib/ev3c.a
	mkdir -p $(@D)
	$(CC) -o $@ $< lib/ev3c.a $(CFLAGS) -Iinclude

bin/test_motor: src/test_motor.c lib/ev3c.a
	mkdir -p $(@D)
	$(CC) -o $@ $< lib/ev3c.a $(CFLAGS) -Iinclude

bin/test_lcd: src/test_lcd.c lib/ev3c.a
	mkdir -p $(@D)
	$(CC) -o $@ $< lib/ev3c.a $(CFLAGS) -Iinclude

bin/test_button: src/test_button.c lib/ev3c.a
	mkdir -p $(@D)
	$(CC) -o $@ $< lib/ev3c.a $(CFLAGS) -Iinclude

bin/test_led: src/test_led.c lib/ev3c.a
	mkdir -p $(@D)
	$(CC) -o $@ $< lib/ev3c.a $(CFLAGS) -Iinclude

bin/test_battery: src/test_battery.c lib/ev3c.a
	mkdir -p $(@D)
	$(CC) -o $@ $< lib/ev3c.a $(CFLAGS) -Iinclude

documentation: include/*.h
	mkdir -p /tmp/ev3DocumentationTemp
	mkdir -p doc
	naturaldocs -i . -xi ./doc -o HTML doc -p /tmp/ev3DocumentationTemp

.PHONY: all clean

clean:
	rm -rf obj lib bin
	rm -f *~
