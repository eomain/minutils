
CC = cc

CFLAGS := -std=c99 -Wall

INSTALL_DIR = /usr/local/bin

OBJS = cp rm cat touch ash wc

default:

cp: cp.c
rm: rm.c
cat: cat.c
touch: touch.c
ash: ash.c
wc: wc.c

all: $(OBJS)

install-ash: ash cp
	@cp ash $(INSTALL_DIR)
	-@echo "ash: successfully installed"
	-@echo "type 'ash' to start a new shell session"

uninstall-ash:
	@rm $(INSTALL_DIR)/ash
	-@echo "ash: successfully uninstalled"

clean:
	-@rm $(OBJS)
