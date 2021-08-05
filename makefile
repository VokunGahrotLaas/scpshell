CC = gcc
CCFLAGS = -std=gnu2x -O2 -g -Wall -Wextra -Wconversion -Werror
LDFLAGS = 
INCLUDES = -I. -I/usr/include -Iscplib
LIBS = -L/usr/lib
EXEC = scpshell
STRICT_EXEC = strict_$(EXEC)

empty = 
space = $(empty) $(empty)
tab = $(empty)	$(empty)
comma = ,
define newline

$(empty)
endef

all: $(EXEC)

$(EXEC): main.c
	@$(CC) -o $@ $< $(LDFLAGS) $(LIBS) $(CCFLAGS) $(INCLUDES)

$(STRICT_EXEC): main.c
	@$(CC) -o $@ $< $(LDFLAGS) $(LIBS) $(CCFLAGS) $(INCLUDES) -Wpedantic -DSCP_PEDANTIC

clean:

mrproper: clean
	@rm -rf $(EXEC) $(STRICT_EXEC)

run_%: %
	@echo "Running "$<"..."
	@echo "================================"
	@-./$<
	@echo "================================"
	@echo "Finished "$<
	@rm $<
	@echo ""

debug_%: %
	@echo "Debugging "$<"..."
	@echo "================================"
	@-gdb $<
	@echo "================================"
	@echo "Finished "$<
	@rm $<
	@echo ""

validate_%: %
	@echo "Validating "$<"..."
	@echo "================================"
	@-./build-your-own-shell/validate ./$<
	@echo "================================"
	@echo "Finished "$<
	@rm $<
	@echo ""

install: $(EXEC)
	@cp $< /usr/bin/

.PHONY: all clear mrproper run_% debug_% validate_% install