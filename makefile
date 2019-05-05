INCLUDE = ./include

BUILD = ./build

CC = gcc

RUNTIME = -lrt -lpthread

CFLAG = -g -Wall 

BIN = $(BUILD)/make_ipc $(BUILD)/proc0 $(BUILD)/proc1 $(BUILD)/proc2 $(BUILD)/proc3 $(BUILD)/proc4

DEPS = $(BUILD)/error.o $(BUILD)/queue.o $(BUILD)/proc_init.o $(BUILD)/buffer.o $(BUILD)/sample.o $(BUILD)/blackboard.o
#BIN = make_ipc proc0 proc1 


$(BUILD)/queue.o : queue.c
	$(CC) $(CFLAG) -c $^ -o $@ $(RUNTIME)

$(BUILD)/error.o : error.c
	$(CC) $(CFLAG) -c $^ -o $@ $(RUNTIME)

$(BUILD)/proc_init.o : proc_init.c
	$(CC) $(CFLAG) -c $^ -o $@ $(RUNTIME)

$(BUILD)/buffer.o : buffer.c
	$(CC) $(CFLAG) -c $^ -o $@ $(RUNTIME)

$(BUILD)/sample.o : sample.c
	$(CC) $(CFLAG) -c $^ -o $@ $(RUNTIME)

$(BUILD)/blackboard.o : blackboard.c
	$(CC) $(CFLAG) -c $^ -o $@ $(RUNTIME)

$(BUILD)/make_ipc : make_ipc.c $(DEPS)
	$(CC) $(CFLAG) $^ -o $@ $(RUNTIME)

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

all : $(BIN)

clean : 
	rm *.o
	rm $(BUILD)/*
