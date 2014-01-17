all: requeue

requeue: requeue.o
	$(CC) $^ -lpthread -o $@

clean:
	rm -f *.o

