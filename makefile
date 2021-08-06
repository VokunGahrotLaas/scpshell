CC = gcc
CCFLAGS = -std=gnu2x -O2 -g -Wall -Wextra -Wconversion -Werror
LDFLAGS = 
INCLUDES = -I. -I/usr/include -Iscplib
LIBS = -L/usr/lib
DEFINES = -DSCPSHELL_INPUT_BUFFER_SIZE=4096

SRC_MAIN = main.c
SRC_SHELL = $(wildcard shell/*.c)
SRC = $(SRC_MAIN) $(SRC_SHELL)
OBJ = $(SRC:.c=.o)
STRICT_OBJ = strict_$(SRC_MAIN:.c=.o) $(addprefix shell/strict_,$(notdir $(SRC_SHELL:.c=.o)))
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

$(EXEC): $(OBJ)
	@$(CC) -o $@ $^ $(LDFLAGS) $(LIBS)

$(STRICT_EXEC): $(STRICT_OBJ)
	@$(CC) -o $@ $^ $(LDFLAGS) $(LIBS)

main.o: main.c
	@$(CC) -o $@ -c $< $(CCFLAGS) $(INCLUDES) $(DEFINES) -DSCP_IMPLEMENTATION

strict_main.o: main.c
	@$(CC) -o $@ -c $< $(CCFLAGS) $(INCLUDES) $(DEFINES) -DSCP_IMPLEMENTATION -Wpedantic -DSCP_PEDANTIC

%.o: %.c
	@$(CC) -o $@ -c $< $(CCFLAGS) $(INCLUDES) $(DEFINES)

strict_%.o: %.c
	@$(CC) -o $@ -c $< $(CCFLAGS) $(INCLUDES) $(DEFINES) -Wpedantic -DSCP_PEDANTIC

shell/strict_%.o: shell/%.c
	@$(CC) -o $@ -c $< $(CCFLAGS) $(INCLUDES) $(DEFINES) -Wpedantic -DSCP_PEDANTIC

clean:
	@rm -rf *.o shell/*.o

mrproper: clean
	@rm -rf $(EXEC) $(STRICT_EXEC)

run_%: %
	@echo "Running "$<"..."
	@echo "================================"
	@-./$<
	@echo "================================"
	@echo "Finished "$<
	@echo ""
	@make mrproper >> /dev/null

debug_%: %
	@echo "Debugging "$<"..."
	@echo "================================"
	@-gdb $<
	@echo "================================"
	@echo "Finished "$<
	@echo ""
	@make mrproper >> /dev/null

validate_%: %
	@echo "Validating "$<"..."
	@echo "================================"
	@-./build-your-own-shell/validate ./$<
	@echo "================================"
	@echo "Finished "$<
	@echo ""
	@make mrproper >> /dev/null

install: $(EXEC)
	@cp $< /usr/bin/

.PHONY: all clean mrproper run_% debug_% validate_% install