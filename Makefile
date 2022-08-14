
OBJDIR= objs
SRCS=$(shell find -L . -name '*.c')
OBJS=$(patsubst %.c,$(OBJDIR)/%.o,$(SRCS))

TARGET=		myBeeMonitor

LIB=		-lpthread -lmosquitto -ljansson -lpigpio
SAMPLE_EXTRA_LIBS = -lm  
LDADD =  $(SAMPLE_EXTRA_LIBS)

CFLAGS+=     -Iinclude/ $(INCLUDE_DBUS) -Wall -Wextra 
	
	
$(OBJDIR)/%.o: %.c
	$(CC)  $(LDADD) $(CFLAGS) -c $< -o $@
	
all: $(TARGET) 
			
# Normale			
$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(LIB) ${LDADD} $(CFLAGS)
			
clean:
	rm -f $(OBJDIR)/source/*.o $(TARGET) 
	
copy:
	scp  $(TARGET) root@192.168.6.71:/home/root/myBeeMonitor

