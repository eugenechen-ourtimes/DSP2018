# The following two variable will be commandline determined by TA
# For testing, you could uncomment them.
SRIPATH ?= /data/DSP_HW3/103_2/srilm-1.5.10
MACHINE_TYPE ?= i686-m64
LM ?= bigram.lm

CXX = g++
CXXFLAGS = -O3 -I$(SRIPATH)/include -w
vpath lib%.a $(SRIPATH)/lib/$(MACHINE_TYPE)

TARGET = mapping mydisambig
MAPPING_OBJ = mapping.o CNChar.o
MYDISAMBIG_OBJ = mydisambig.o NgramUtils.o Viterbi.o CNChar.o
OBJ = $(MAPPING_OBJ) $(MYDISAMBIG_OBJ)
LIB = -loolm -ldstruct -lmisc
TO = ZhuYin-Big5.map
FROM = Big5-ZhuYin.map
.PHONY: clean map run disambig

all: $(TARGET)

mapping: $(MAPPING_OBJ) $(LIB)
	@echo "making: " $@
	@$(CXX) -o $@ $(LDFLAGS) $^ -Wall
mydisambig: $(MYDISAMBIG_OBJ) $(LIB)
	@echo "making: " $@
	@$(CXX) -o $@ $(LDFLAGS) $^ -Wall

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -Wall

map:
	@echo "Mapping!"
	@./mapping $(FROM) > $(TO)
	@#matlab mapping.m $(FROM) $(TO)
	@#python3 mapping.py $(FROM) $(TO)
	@#sh mapping.sh $(FROM) $(TO)

disambig:
	@for i in $(shell seq 1 10) ; do \
	    echo "Running $$i.txt"; \
	    disambig -text testdata/$$i.txt -map $(TO) -lm $(LM) -order 2 > result1/$$i.txt; \
	done;

run:
	@if [ ! -d result2 ]; then \
		mkdir result2; \
		echo "mkdir result2"; \
		break; \
	fi; \
 
	@for i in $(shell seq 1 10) ; do \
	    echo "Running $$i.txt"; \
	    ./mydisambig -text testdata/$$i.txt -map $(TO) -lm $(LM) -order 2 -consider-rare > result2/$$i.txt; \
	done;

clean:
	$(RM) $(OBJ) $(TARGET)

