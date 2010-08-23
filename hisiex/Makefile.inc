
HISIEX_DIR = hisiex

DRAMER_TARGET := hisiex-dramer
DRAMER_DIR := $(HISIEX_DIR)/dramer
DRAMER_OBJS := $(DRAMER_DIR)/start.o $(DRAMER_DIR)/main.o 

hisi: $(DRAMER_TARGET)

$(DRAMER_TARGET): u-boot $(DRAMER_OBJS)
	$(LD) $(DRAMER_OBJS) --start-group $(LIBS) --end-group -Bstatic -Ttext 0 -o $(DRAMER_TARGET)

