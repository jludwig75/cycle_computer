debug:
	mkdir -p Debug
	cd Debug && cmake -DCMAKE_BUILD_TYPE=Debug .. && make

release:
	mkdir -p Release
	cd Release && cmake -DCMAKE_BUILD_TYPE=Release .. && make

coverage: debug
	cd Debug && make uart_unittest_coverage
	cd Debug && make uart_circular_buffer_integration_test
	cd Debug && make circular_buffer_unittest

clean:
	rm -rf Debug
	rm -rf Release

all: debug release