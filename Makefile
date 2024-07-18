OBJS:=cap.o

all: $(OBJS)

clean:
	rm -f $(OBJS)

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $<
