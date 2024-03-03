include include/config.mk

INCLUDE = -Iinclude
RM      = rm -f
SRCDIR  = src
OBJDIR  = obj
SRC     = main.c other.c commands/getmsg.c
OBJ     = $(addprefix $(OBJDIR)/,$(SRC:.c=.o))
DEFLDFLAGS = -L/usr/local/lib -pthread -ldiscord -lcurl -lpcre2-8

all: notabotc

obj:
	mkdir -p $(OBJDIR)

obj/commands: obj
	mkdir -p obj/commands

obj/%.o: $(SRCDIR)/%.c
	$(CC) -c -std=c11 -o $@ $< $(CFLAGS) $(INCLUDE)

notabotc: obj obj/commands $(OBJ)
	$(CC) -o notabotc $(OBJ) $(DEFLDFLAGS) $(LDFLAGS)

clean:
	$(RM) notabotc obj/*.o obj/commands/*.o

.PHONY: all clean concord
