
BIN = bin
CFLAGS := -I include

OBJS = ash.o io.o env.o var.o builtin.o

ash: $(OBJS)
	-@mkdir $(BIN)
	$(CC) $(CFLAGS) $(OBJS) -o $(BIN)/$@

%.o:%.c
	$(CC) -c $(CFLAGS) $< -o $@

install: ash
	@cp ash $(INSTALL_DIR)
	-@echo "ash: successfully installed"
	-@echo "type 'ash' to start a new shell session"

uninstall:
	@rm $(INSTALL_DIR)/ash
	-@echo "ash: successfully uninstalled"

clean:
	-@rm -r $(BIN) $(OBJS)
