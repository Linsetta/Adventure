
APP=adventure
OBJS=adventure.o
SOURCE_FILES= \
  makefile    \
  adventure.c
TEXT_EDITOR=vim -p

default: print-usage

print-usage:
	@echo ""
	@echo "Example Project Makefile"
	@echo ""
	@echo "    make"
	@echo "    make print-usage"
	@echo "    make start-editor"
	@echo "    make $(APP)"
	@echo "    make clean"
	@echo "    make edit"
	@echo ""

start-editor:
	$(TEXT_EDITOR) $(SOURCE_FILES)

$(APP): $(OBJS)
	gcc -o $(APP) $(OBJS)

%.o: %.c 
	gcc -c -o $@ $<
 
clean:
	rm -f $(APP) $(OBJS) .*.swp

edit:
	vim -p makefile *.c
