PROG=simple-tcp-proxy
#PROG=myproxy
CFLAGS=-Wall
CC=g++

all: ${PROG}

$(PROG): $(PROG).o
	$(CC) -o $(PROG) $(PROG).o

$(PROG).static: $(PROG).o
	$(CC) -static -o ${@} $(PROG).o
	strip $@

clean:
	@rm -f $(PROG).o
	@rm -f $(PROG).core

fclean: clean
	@rm -f $(PROG)

re: fclean all

.PHONY: all clean fclean re
