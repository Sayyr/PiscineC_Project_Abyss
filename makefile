# ===== OS detection =====
ifeq ($(OS),Windows_NT)
  EXE      := abyss.exe
  RM       := del /Q
  NULLDEV  := 2>nul
  SEP      := &
else
  EXE      := abyss
  RM       := rm -f
  NULLDEV  := 2>/dev/null
  SEP      := ;
endif

CC := gcc

SRC_DIRS := . state state/render player gen
SRC := $(foreach D,$(SRC_DIRS),$(wildcard $(D)/*.c))
OBJ := $(SRC:.c=.o)
DEP := $(OBJ:.o=.d)

# SDL2 (Windows: C:/SDL2 ; Unix: utilisez pkg-config si dispo)
SDL2_INCLUDE := C:/SDL2/include
SDL2_LIB     := C:/SDL2/lib

CFLAGS  := -Wall -Wextra -std=c11 -O2 -MMD -MP \
           -I. -Istate -Istate/render -Iplayer -Igen \
           -I$(SDL2_INCLUDE)
# Sous MinGW: pas besoin de -lSDL2main si vous avez int main(...)
LDFLAGS := -L$(SDL2_LIB) -lSDL2

.PHONY: all clean run printsrc

all: $(EXE)

$(EXE): $(OBJ)
	$(CC) $(OBJ) -o $@ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

run: $(EXE)
	./$(EXE)

clean:
	-$(RM) $(EXE) $(NULLDEV)
	-$(RM) $(OBJ) $(NULLDEV)
	-$(RM) $(DEP) $(NULLDEV)

printsrc:
	@echo Sources:
	@for %%f in ($(SRC)) do @echo  - %%f

-include $(DEP)
