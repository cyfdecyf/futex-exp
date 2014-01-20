all: wait-wake requeue

wait-wake: wait-wake.o
	$(CC) $^ -lpthread -o $@

requeue: requeue.o
	$(CC) $^ -lpthread -o $@

clean:
	rm -f *.o

