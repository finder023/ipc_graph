INCLUDE = ./include

BUILD = ./build

CC = gcc

RUNTIME = -lrt -lpthread

CFLAG = -g -Wall 

BIN = $(BUILD)/make_ipc $(BUILD)/proc0 $(BUILD)/proc1 $(BUILD)/proc2 \
			  $(BUILD)/proc4 $(BUILD)/proc3

DEPS = $(BUILD)/error.o  $(BUILD)/buffer.o $(BUILD)/opr_impl.o \
	   $(BUILD)/blackboard.o $(BUILD)/graph.o 
#BIN = make_ipc proc0 proc1 


$(BUILD)/graph.o : graph.c
	$(CC) $(CFLAG) -c $^ -o $@ 

$(BUILD)/opr_impl.o : opr_impl.c
	$(CC) $(CFLAG) -c $^ -o $@ 
	
$(BUILD)/error.o : error.c
	$(CC) $(CFLAG) -c $^ -o $@ 

$(BUILD)/buffer.o : buffer.c
	$(CC) $(CFLAG) -c $^ -o $@ 

$(BUILD)/blackboard.o : blackboard.c
	$(CC) $(CFLAG) -c $^ -o $@ 

$(BUILD)/make_ipc.o : make_ipc.c 
	$(CC) $(CFLAG) -c $^ -o $@ 

$(BUILD)/proc0.o : proc0.c
	$(CC) $(CFLAG) -c $^ -o $@

$(BUILD)/make_ipc : $(BUILD)/make_ipc.o $(DEPS)
	$(CC) $(CFLAG) $^ -o $@ $(RUNTIME)

$(BUILD)/proc0 : $(BUILD)/proc0.o $(DEPS)
	$(CC) $(CFLAG) $^ -o $@ $(RUNTIME)

$(BUILD)/proc1.o : proc1.c
	$(CC) $(CFLAG) -c $^ -o $@

$(BUILD)/proc1 : $(BUILD)/proc1.o $(DEPS)
	$(CC) $(CFLAG) $^ -o $@ $(RUNTIME)

$(BUILD)/proc2.o : proc2.c
	$(CC) $(CFLAG) -c $^ -o $@

$(BUILD)/proc2 : $(BUILD)/proc2.o $(DEPS)
	$(CC) $(CFLAG) $^ -o $@ $(RUNTIME)

$(BUILD)/proc3.o : proc3.c
	$(CC) $(CFLAG) -c $^ -o $@

$(BUILD)/proc3 : $(BUILD)/proc3.o $(DEPS)
	$(CC) $(CFLAG) $^ -o $@ $(RUNTIME)

$(BUILD)/proc4.o : proc4.c
	$(CC) $(CFLAG) -c $^ -o $@

$(BUILD)/proc4 : $(BUILD)/proc4.o $(DEPS)
	$(CC) $(CFLAG) $^ -o $@ $(RUNTIME)




all : MKBUILD MKTMP $(BIN)

.PHONY: MKBUILD all MKTMP

MKBUILD:
	if [ ! -d $(BUILD) ]; then mkdir $(BUILD); mkdir $(BUILD)/tmp; elif \
		[ ! -d $(BUILD)/tmp ]; then mkdir $(BUILD)/tmp; fi

MKTMP:
	if [ ! -d ./tmp ]; then mkdir ./tmp; fi

clean : 
	rm -rf $(BUILD)/*
