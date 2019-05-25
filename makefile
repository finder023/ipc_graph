INCLUDE = ./include

BUILD = ./build

CC = gcc

RUNTIME = -lrt -lpthread

CFLAG = -g -Wall 

BIN = $(BUILD)/make_ipc $(BUILD)/proc0 $(BUILD)/proc1 $(BUILD)/proc2 \
	  $(BUILD)/proc3 $(BUILD)/proc4

DEPS = $(BUILD)/error.o  $(BUILD)/ipc_init.o $(BUILD)/buffer.o $(BUILD)/opr_impl.o \
	   $(BUILD)/sample.o $(BUILD)/blackboard.o $(BUILD)/graph.o 
#BIN = make_ipc proc0 proc1 


$(BUILD)/graph.o : graph.c
	$(CC) $(CFLAG) -c $^ -o $@ 

$(BUILD)/opr_impl.o : opr_impl.c
	$(CC) $(CFLAG) -c $^ -o $@ 
	
$(BUILD)/error.o : error.c
	$(CC) $(CFLAG) -c $^ -o $@ 

$(BUILD)/ipc_init.o : ipc_init.c
	$(CC) $(CFLAG) -c $^ -o $@ 

$(BUILD)/buffer.o : buffer.c
	$(CC) $(CFLAG) -c $^ -o $@ 

$(BUILD)/sample.o : sample.c
	$(CC) $(CFLAG) -c $^ -o $@ 

$(BUILD)/blackboard.o : blackboard.c
	$(CC) $(CFLAG) -c $^ -o $@ 

$(BUILD)/make_ipc.o : make_ipc.c 
	$(CC) $(CFLAG) -c $^ -o $@ 

$(BUILD)/proc0 : proc0.c	$(DEPS)
	$(CC) $(CFLAG) $^ -o $@ $(RUNTIME)

$(BUILD)/proc1 : proc1.c	$(DEPS)
	$(CC) $(CFLAG) $^ -o $@ $(RUNTIME)

$(BUILD)/proc2 : proc2.c	$(DEPS)
	$(CC) $(CFLAG) $^ -o $@ $(RUNTIME)

$(BUILD)/proc3 : proc3.c	$(DEPS)
	$(CC) $(CFLAG) $^ -o $@ $(RUNTIME)

$(BUILD)/proc4 : proc4.c	$(DEPS)
	$(CC) $(CFLAG) $^ -o $@ $(RUNTIME)

$(BUILD)/make_ipc : $(BUILD)/make_ipc.o $(DEPS)
	$(CC) $(CFLAG) $^ -o $@ $(RUNTIME)

all : MKBUILD $(BIN)

.PHONY: MKBUILD all

MKBUILD:
	if [ ! -d $(BUILD) ]; then mkdir $(BUILD); mkdir $(BUILD)/tmp; elif \
		[ ! -d $(BUILD)/tmp ]; then mkdir $(BUILD)/tmp; fi

clean : 
	rm -rf $(BUILD)/*
