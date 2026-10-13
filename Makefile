CC ?= cc
CFLAGS ?= -std=gnu99 -O2 -Wall -Wextra -Wpedantic
SOURCES = main.c two_d_tree.c processingstring.c

map2: $(SOURCES) two_d_tree.h processingstring.h
	$(CC) $(CFLAGS) $(SOURCES) -lm -o $@

test: map2 .build/test-nearest
	mkdir -p .build
	./.build/test-nearest
	python3 -B -m unittest discover -s tests -v

.build/test-nearest: two_d_tree.c tests/test_nearest.c two_d_tree.h
	mkdir -p .build
	$(CC) $(CFLAGS) two_d_tree.c tests/test_nearest.c -lm -o $@

sanitize:
	mkdir -p .build
	$(CC) -std=gnu99 -O1 -g -Wall -Wextra -Wpedantic -fsanitize=address,undefined -fno-omit-frame-pointer $(SOURCES) -lm -o .build/map2-sanitize
	$(CC) -std=gnu99 -O1 -g -Wall -Wextra -Wpedantic -fsanitize=address,undefined -fno-omit-frame-pointer two_d_tree.c tests/test_nearest.c -lm -o .build/test-nearest-sanitize
	./.build/test-nearest-sanitize
	PROGRAM=$(CURDIR)/.build/map2-sanitize python3 -B -m unittest discover -s tests -v

clean:
	rm -f map2 *.o *.gch .build/map2-sanitize
.PHONY: test sanitize clean
