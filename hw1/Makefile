.PHONY: clean
GCC=gcc
CFLAGS+=
FLAG=-g 
#FLAG=-g -DPRINT_DEBUG
LDFLAGS+=-lm     # link to math library

# TARGET=test_hmm
TARGET=train test

TRAIN_OBJS=train.o algo.o
TEST_OBJS=test.o viterbi.o

all: $(TARGET)
# type make/make all to compile test_hmm

%.o: %.c
	@echo "making: " $@
	@$(GCC) -c $< $(FLAG)

train: $(TRAIN_OBJS)
	@echo "making: " $@
	@$(GCC) -o $@ $(TRAIN_OBJS)
	
test: $(TEST_OBJS)
	@echo "making: " $@
	@$(GCC) -o $@ $(TEST_OBJS)
	
clean:
	rm -f *.o
	@for bin in $(TARGET); do \
		echo "rm -f $$bin"; \
		rm -f $$bin; \
	done
