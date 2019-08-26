.PHONY: all
all:
	gcc -o post-transition -g -Wall -Werror -Wextra -Wpedantic -fsanitize=address post-transition.c
