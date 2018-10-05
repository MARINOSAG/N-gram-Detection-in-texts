OBJS1	= main.o bloomfilter.o func.o  trie.o bucket.o hashTable.o heap.o bucketHeap.o hashTableHeap.o
OUT1	= exec
CC		= g++
FLAGS	= -c 


t1: $(OBJS1)
	$(CC) $(OBJS1) -o $(OUT1)
	rm -f $(OBJS1)

heap.o: heap.cc
	$(CC) $(FLAGS) heap.cc
bucketHeap.o: bucketHeap.cc
	$(CC) $(FLAGS) bucketHeap.cc
hashTableHeap.o: hashTableHeap.cc
	$(CC) $(FLAGS) hashTableHeap.cc
bucket.o: bucket.cc
	$(CC) $(FLAGS) bucket.cc
hashTable.o: hashTable.cc
	$(CC) $(FLAGS) hashTable.cc	
bloomfilter.o: bloomfilter.cc
	$(CC) $(FLAGS) bloomfilter.cc
func.o: func.cc
	$(CC) $(FLAGS) func.cc
trie.o: trie.cc
	$(CC) $(FLAGS) trie.cc
main.o: main.cc
	$(CC) $(FLAGS) main.cc


# clean up
clean:
	rm -f $(OBJS1) $(OUT1)
