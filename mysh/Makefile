CFLAGS += -g -pedantic -Wall -Wextra -std=c99
OBJDIR = obj
BINDIR = bin
SRCDIR = .

MYSH_DEP = cd resizable mysh input command
MYSH_DEP_H = $(addsuffix .h,  $(addprefix $(SRCDIR)/, $(MYSH_DEP)) )
MYSH_DEP_OBJ = $(addsuffix .o,  $(addprefix $(OBJDIR)/, $(MYSH_DEP)) )
$(BINDIR)/mysh: $(MYSH_DEP_H) $(MYSH_DEP_OBJ)
	mkdir -p bin
	gcc $(CFLAGS) $^ -o $@

$(OBJDIR)/%.o: %.c
	mkdir -p obj
	gcc -c $(CFLAGS) $^ -o $@

.PHONY: clear
	rm -r $(BINDIR)
	rm -r $(OBJDIR)

