CC = clang
CFLAGS = -std=c99 -ggdb3 -Wall -I../
SRCS = $(wildcard ./*.c)

run_all: $(SRCS)
	@for example in $^ ; do \
		echo "Running $${example}" ; \
		$(CC) $(CFLAGS) $${example} ; \
		./a.out ; \
		echo " " ; \
	done ; \
	rm -rf a.out ; \
