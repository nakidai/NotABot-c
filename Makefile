include include/config.mk

INCLUDE = -Iinclude
RM      = rm -f
SRCDIR  = src
OBJDIR  = obj
SRC     = main.c
OBJ     = $(addprefix $(OBJDIR)/,$(SRC:.c=.o))
DEFLDFLAGS = -L/usr/local/lib -pthread -ldiscord -lcurl

all: notabotc

obj:
	mkdir -p $(OBJDIR)

obj/%.o: $(SRCDIR)/%.c
	$(CC) -c -o $@ $< $(CFLAGS) $(INCLUDE)

notabotc: obj $(OBJ)
	$(CC) -o notabotc $(OBJ) $(DEFLDFLAGS) $(LDFLAGS)

clean:
	$(RM) notabotc obj/*.o

.PHONY: all clean concord
