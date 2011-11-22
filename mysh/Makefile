CFLAGS += -g -pedantic -Wall -Wextra -std=c99
OBJDIR = obj
BINDIR = bin

$(BINDIR)/mysh: $(OBJDIR)/mysh.o  $(OBJDIR)/cd.o
	mkdir -p bin
	gcc $(CFLAGS) $^ -o $@

$(OBJDIR)/%.o: %.c
	mkdir -p obj
	gcc -c $(CFLAGS) $^ -o $@

.PHONY: clear
	rm -r $(BINDIR)
	rm -r $(OBJDIR)

