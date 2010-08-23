include $(TOPDIR)/config.mk

CFLAGS += -Wno-unused

LIB := libdrv.a

OBJS := hisilicon-net.o watchdog.o

all:	$(LIB)

$(LIB):	$(OBJS)
	$(AR) crv $@ $(OBJS)

#########################################################################

.depend:	Makefile $(OBJS:.o=.c)
		$(CC) -M $(CFLAGS) $(OBJS:.o=.c) > $@

sinclude .depend

