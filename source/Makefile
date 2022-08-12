
OBJDIR= ../objs
SRCS=$(shell find -L . -name '*.c')
OBJS=$(patsubst %.c,$(OBJDIR)/%.o,$(SRCS))

TARGET=		myBeeMonitor

LIB=		-lpthread -lmosquitto -ljansson
SAMPLE_EXTRA_LIBS = -lm  
LDADD =  $(SAMPLE_EXTRA_LIBS)

CFLAGS+=     -I./../include/ $(INCLUDE_DBUS) -Wall -Wextra 
	
	
$(OBJDIR)/%.o: %.c
	$(CC)  $(LDADD) $(CFLAGS) -c $< -o $@
	
all: clean $(TARGET) copy
			
# Normale			
$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(LIB) ${LDADD} $(CFLAGS)
			
clean:
	rm -f $(OBJDIR)/*.o $(TARGET) 
	
copy:
	scp  $(TARGET) root@192.168.6.71:/home/root/myBeeMonitor

