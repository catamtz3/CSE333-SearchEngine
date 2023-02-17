# CSE333-SearchEngine
Creates a query Search Engine in C

In this assignment, you will use the LinkedList and HashTable modules that you built in Homework 1 in order to finish our implementation of a file system crawler, indexer, and search engine. Be sure to follow good C practices and maintain good code quality as you complete the assignment.

In Part A, you will build a module that reads the content of a file into memory, parses it into a series of words, and builds a linked list of (word, position) information.
In Part B, you will build modules that convert a series of these linked lists into an in-memory, .
In Part C, you will use this in-memory, inverted index to build a query processor that has a console-based interface.

Part A:
You're going to write a module that reads the contents of a text file into memory and then parses the text file to look for words within it. As it finds words, it will build up a HashTable that contains one record for each word. Each record will contain a lowercase copy of the word, 
and also a sorted linked list. Each record of the linked list contains an offset within the file that the word appeared in (the first character in the file has offset zero). However, our word parser won't be very smart. It will treat as a word any non-zero sequence of alphabetic characters separated by non-alphabetic characters.
Each key in the hash table is the result of calling the HashTable module's FNVHash64() function, passing the string as the first argument, and the strlen(string) as the second argument.
Each element in the hash table is a pointer to a heap-allocated structure that contains two fields; a string and a linked list. Note the string is lower-cased, and that our parser is not very smart: because it treats any sequence of alphabetic characters surrounded by non-alphabetic characters as words, the word I'll will be misparsed as the two words i and ll.
Each element in the linked list is an integer representing the position in the text file at which the word starts; this is both its byte offset and, since we are only handling ASCII files, the number of characters from the start of the file (each ASCII character is exactly 1 byte). So, the word "my" starts at offset 0 in the text file, the word "i" appears twice, once at offset 14 and once at offset 40, and the word "course" appears twice, once at offset 25 and once at offset 62.
Each list is sorted in ascending order.

Part B:
At a high-level, a search engine has three major components: a crawler, an indexer, and a query processor. A crawler explores the world, looking for documents to index. The indexer takes a set of documents found by the crawler, and produces something called an inverted index out of them. A query processor asks a user for a query, and processes it using the inverted index to figure out a list of documents that match the query.

File System Crawler
Your file system crawler will be provided with the name of a directory in which it should start crawling. Its job is to look through the directory for documents (text files) and to hand them to the indexer, and to look for subdirectories; it recursively descends into each subdirectory to look for more documents and sub-sub-directories. For each document it encounters, it will assign the document a unique "document ID", or "docID". A docID is just a 64-bit unsigned integer.

Your crawler itself will build two hash tables in memory, adding to them each time it discovers a new text file.
For each document the crawler finds, it will make use of your Part A code to produce a word hashtable using ParseIntoWordPositionsTable().

Indexer
This is the heart of the search engine. The job of the indexer is to take each word hashtable produced by ParseIntoWordPositionsTable(), and fold its contents in to an inverted index. An inverted index is just a hash table that maps from a word to a "posting list," and a posting list is just a list of places that word has been found.
Walking through it, the inverted index is a hash table that maps from a (hash of a) word to a structure. The structure (shown in green) contains the word as a string, and also a HashTable. The HashTable maps from the docID (not the hash of docID) to a LinkedList. The LinkedList contains each position that word appeared in that docID.

Part C:
Now that you have a working inverted index, you're ready to build your search engine. The job of a search engine is to receive queries from a user, and return a list of matching documents in some rank order.

For us, a query is just a string of words, such as:

course friends my
The goal of our search engine is to find all of the documents that contain all of the words. So, if a document has the words "course" and "my" but not the word "friends," it shouldn't match.

To execute a query, first the query processor must split the query up into a list of words (the strtok_r() function is useful for this). Next, it looks up in the inverted index the list of documents that match the first word. This is our initial matching list.

Next, for each additional word in the query, the query processor uses the inverted index to get access to the HashTable that contains the set of matching documents. For each document in the matching list, the query processor tests to see if that document is also in the HashTable. If so, it keeps the document in the matching list, and if not, it removes the document from the matching list.

Once the processor has processed all of the words, it's done. Now, it just has to rank the results, sort the results by rank, and return the sorted result list to the user.

For us, our ranking function is very simple: given a document that matches against a query, we sum up the number of times each query word appears in the document, and that's our rank. So, if the user provides the query "foo" and that words appears on a document 5 times, the rank for that document given that query is 5. If the user provides a multi-word query, our sum includes the number of times each of the words in the query appears. So, if the user provides the query "foo bar", 
the word "foo" appears in the document 5 times, and the word "bar" appears 10 times, the rank of the document for that query is 15. The bigger the sum, the higher the rank, and therefore the "better" the search result.
