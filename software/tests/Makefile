.PHONY: all clean

all: test_wiegand_reader_callback test_wiegand_reader_getter

test_wiegand_reader_callback: test_wiegand_reader_callback.c tinkerforge/ip_connection.c tinkerforge/bricklet_industrial_digital_in_4_v2.c
	gcc -O2 -Wall -Wextra -pthread $^ -o $@

test_wiegand_reader_getter: test_wiegand_reader_getter.c tinkerforge/ip_connection.c tinkerforge/bricklet_industrial_digital_in_4_v2.c
	gcc -O2 -Wall -Wextra -pthread $^ -o $@

clean:
	rm test_wiegand_reader_callback test_wiegand_reader_getter
