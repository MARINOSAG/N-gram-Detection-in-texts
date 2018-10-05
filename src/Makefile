OBJS1	= ./Dynamic/main.o ./Dynamic/bloomfilter.o ./Dynamic/func.o  ./Dynamic/trie.o ./Dynamic/bucket.o ./Dynamic/hashTable.o ./Dynamic/heap.o ./Dynamic/bucketHeap.o ./Dynamic/hashTableHeap.o ./Dynamic/job_scheduler.o
OBJS2	= ./Static/static_main.o ./Static/static_bloomfilter.o ./Static/static_func.o  ./Static/static_trie.o ./Static/static_bucket.o ./Static/static_hashTable.o ./Static/static_heap.o ./Static/static_bucketHeap.o ./Static/static_hashTableHeap.o ./Static/job_scheduler.o
OBJS3   = MAIN.o
OUT1	= dynamic
OUT2 	= static
OUT3    = exec
CC	= g++
FLAGS	= -c

#g++ -pthread Static/job_scheduler.cc Static/static_bloomfilter.cc Static/static_bucket.cc Static/static_hashTableHeap.cc Static/static_heap.cc Static/static_main.cc Static/static_trie.cc Static/static_hashTable.cc Static/static_func.cc Static/static_bucketHeap.cc -o static

all:  t1 t2 t3

t1: $(OBJS1)
	$(CC) $(OBJS1)  -o $(OUT1) -lpthread
	rm -f $(OBJS1)

job_scheduler.o: ./Dynamic/job_scheduler.cc
	$(CC)  $(FLAGS) ./Dynamic/job_scheduler.cc
heap.o: ./Dynamic/heap.cc
	$(CC) $(FLAGS) ./Dynamic/heap.cc
bucketHeap.o: ./Dynamic/bucketHeap.cc
	$(CC) $(FLAGS) ./Dynamic/bucketHeap.cc
hashTableHeap.o: ./Dynamic/hashTableHeap.cc
	$(CC) $(FLAGS) ./Dynamic/hashTableHeap.cc
bucket.o: ./Dynamic/bucket.cc
	$(CC) $(FLAGS) ./Dynamic/bucket.cc
hashTable.o: ./Dynamic/hashTable.cc
	$(CC) $(FLAGS) ./Dynamic/hashTable.cc	
bloomfilter.o: ./Dynamic/bloomfilter.cc
	$(CC) $(FLAGS) ./Dynamic/bloomfilter.cc
func.o: ./Dynamic/func.cc
	$(CC) $(FLAGS) ./Dynamic/func.cc
trie.o: ./Dynamic/trie.cc
	$(CC) $(FLAGS) ./Dynamic/trie.cc
main.o: ./Dynamic/main.cc
	$(CC) $(FLAGS) ./Dynamic/main.cc


t2: $(OBJS2)
	$(CC) $(OBJS2) -o $(OUT2) -lpthread
	rm -f $(OBJS2)

job_scheduler.o: ./Static/job_scheduler.cc
	$(CC)  $(FLAGS) ./Static/job_scheduler.cc
static_heap.o: ./Static/static_heap.cc
	$(CC) $(FLAGS) ./Static/static_heap.cc
static_bucketHeap.o: ./Static/static_bucketHeap.cc
	$(CC) $(FLAGS) ./Static/static_bucketHeap.cc
static_hashTableHeap.o: ./Static/static_hashTableHeap.cc
	$(CC) $(FLAGS) ./Static/static_hashTableHeap.cc
static_bucket.o: ./Static/static_bucket.cc
	$(CC) $(FLAGS) ./Static/static_bucket.cc
static_hashTable.o: ./Static/static_hashTable.cc
	$(CC) $(FLAGS) ./Static/static_hashTable.cc	
static_bloomfilter.o: ./Static/static_bloomfilter.cc
	$(CC) $(FLAGS) ./Static/static_bloomfilter.cc
static_func.o: ./Static/static_func.cc
	$(CC) $(FLAGS) ./Static/static_func.cc
static_trie.o: ./Static/static_trie.cc
	$(CC) $(FLAGS) ./Static/static_trie.cc
static_main.o: ./Static/static_main.cc
	$(CC) $(FLAGS) ./Static/static_main.cc

t3: $(OBJS3)
	$(CC) $(OBJS3) -o $(OUT3) -lpthread
	rm -f $(OBJS3)

MAIN.o: ./MAIN.cc
	$(CC) $(FLAGS) ./MAIN.cc

# clean up
clean:
	rm -f $(OBJS1) $(OUT1)
	rm -f $(OBJS2) $(OUT2)
	rm -f $(OBJS3) $(OUT3)
