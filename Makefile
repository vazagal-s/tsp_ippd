FLAGS=-O2

CC=gcc

RM=rm -f

EXEC=tsp

all: $(EXEC)

$(EXEC):
	$(CC) -fopenmp $(FLAGS) $(EXEC).c -c -o $(EXEC).o
	$(CC) $(FLAGS) $(EXEC).o -o $(EXEC)

run:
	./$(EXEC)

clean:
	$(RM) $(EXEC).o $(EXEC)
