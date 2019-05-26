INCLUDE = ./include

BUILD = ./build

CC = gcc

RUNTIME = -lrt -lpthread

CFLAG = -g -Wall 

BIN = $(BUILD)/make_ipc $(BUILD)/tmain $(BUILD)/tmain1 $(BUILD)/tmain2 
	  

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

$(BUILD)/tmain.o : tmain.c
	$(CC) $(CFLAG) -c $^ -o $@

$(BUILD)/make_ipc : $(BUILD)/make_ipc.o $(DEPS)
	$(CC) $(CFLAG) $^ -o $@ $(RUNTIME)

$(BUILD)/tmain : $(BUILD)/tmain.o $(DEPS)
	$(CC) $(CFLAG) $^ -o $@ $(RUNTIME)

$(BUILD)/tmain1.o : tmain1.c
	$(CC) $(CFLAG) -c $^ -o $@

$(BUILD)/tmain1 : $(BUILD)/tmain1.o $(DEPS)
	$(CC) $(CFLAG) $^ -o $@ $(RUNTIME)

$(BUILD)/tmain2.o : tmain2.c
	$(CC) $(CFLAG) -c $^ -o $@

$(BUILD)/tmain2 : $(BUILD)/tmain2.o $(DEPS)
	$(CC) $(CFLAG) $^ -o $@ $(RUNTIME)



all : MKBUILD $(BIN)

.PHONY: MKBUILD all

MKBUILD:
	if [ ! -d $(BUILD) ]; then mkdir $(BUILD); mkdir $(BUILD)/tmp; elif \
		[ ! -d $(BUILD)/tmp ]; then mkdir $(BUILD)/tmp; fi

clean : 
	rm -rf $(BUILD)/*
