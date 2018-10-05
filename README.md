# N-gram-Detection-in-texts

This project is similar to the challenge of Sigmod 2017 competition. For further details you can visit 
[this](http://sigmod17contest.athenarc.gr/) page.

The main goal of this project is to detect N-grams in texts. To be more specific, we have a given list of N-grams, which is kept in memory, and a stream of texts
in which we should detect possible occurrences of N-grams. N-gram add / remove commands will be submitted, during the flow of the text, to update the original list.

First of all, the structure, in which the information is saved, is a [trie](https://en.wikipedia.org/wiki/Trie). The available actions that can be performed to this trie are Query (Q), Add (A) and Delete (D).
These actions take place in batches. For example, one batch can have Q, Q, Q, A, A, Q, D and another batch can have A, D, D, A, Q, Q, D, Q.

The main utilities we have implemented are listed below:
1) A [bloom filter](https://en.wikipedia.org/wiki/Bloom_filter) which is used to efficiently avoid inserting duplicate N-grams during a Q.
2) A hash table that points to the root of trie. In this way, we find very fast the first word of an N-gram, which a very frequent query.
3) A [binary heap](https://en.wikipedia.org/wiki/Binary_heap) to find out which are the top-K N-grams in our structure.
4) A static trie, which is a trie that the only allowed action is Q (not A and D). The nodes of the static trie are compressed.
5) Finally, we have introduced multi-threading in order to minimize the execution time of Q actions. We use POSIX threads. 

*In src/ there is a detailed report in Greek, but in the last pages there are measurements of the performance of our application.
