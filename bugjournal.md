# Bug 1
Segmentation Fault
## A) How is your program acting differently than you expect it to?
- Our program is going through GDB properly when we're running through InsertContent in FileParser.c, but when we run it it's giving us a Segfault error.

## B) Brainstorm a few possible causes of the bug
- Maybe we're not freeing somewhere we should be? But this doesn't make sense because we need all the space we allocated in AddWordPosition
- Maybe we're adding the wrong thing into the LinkedListAppend call in our AddWordPosition function
- Maybe we're resetting a variable on accident and are trying to use it after we rest it in AddWordPosition

## C) How you fixed the bug and why the fix was necessary
- We figured our bug was in AddWordPosition because we can walk through GDB in InsertContent and see that that function was working fine, but when we walked through GDB in AddWordPosition we could only tell that we were adding something, but couldn't tell what we were adding. We looked at it and realized that we were mallocing space for our new word and using our parameter word that stored the word we wanted to add to hold the malloced space. This was resetting the word and wasn't adding the proper thing to the LinkedList. We fixed it by making a temp variable to malloc the space, and then set temp equal to word and had wp -> word store the temp. We needed this fix to actually add the word to the LinkedList or else it never would've worked correctly.


# Bug 2
Another Segfault

## A) How is your program acting differently than you expect it to?
- Our program is acting different than we expect it too because we expect our
program to run and get passed the qsort call in the HandleDir function, but it doesn't 
even run that line and returns a segfault/core aborted instead.

## B) Brainstorm a few possible causes of the bug
- Maybe we're not incrementing the variable i correctly since we're in a for loop and it auto increments
even when the cases aren't supposed to be counted as an entry
- Maybe we incorrectly passed something in so the entries contents aren't what the qsort call is looking for
- Maybe we need to close the directory when we exit the loop

## C) How you fixed the bug and why the fix was necessary
- My partner and I thought about it and realized that since we're indexing using our for loop
that auto increments (incluiding when we want to skip files and not include them), our indexing could be way off
and leading us to a segfault. We attempted to decrement i in our for loop for every case where we wanted to skip,
but it looked sloppy and didn't fix the segfault so we switched our code to a while loop so we could choose where to 
increment i where we needed and not have it auto increment. This fixed our segfault and our code was working great since we 
were finally correctly indexing the amount of times we needed too! 


# Bug 3
Core Aborted

## A) How is your program acting differently than you expect it to?
- Our program is acting different than we expected it too because we expected our Doctable.c file to work
properly when it was called in CrawlFileTree. It wasn't working and giving us a core aborted every time we tried to add 
to our doctable.

## B) Brainstorm a few possible causes of the bug
- Maybe our entrie numbers aren't right and we need to fix the numbers to be accurate in HandleDir()
- Maybe there's a slight error in DocTable.c that we overlooked when we ran the tests for that file
- Maybe we are passing in the wrong thing to be added to the doctable?

## C) How you fixed the bug and why the fix was necessary
- We looked at our DocTable.c code and saw in the GDB that it was failing when we were copying over 
our contents from one char* to another in Doctable_Add(). We looked closer at when we were mallocing space and realized
that we forgot to add one at the end of the malloced space for the null terminator, which meant we weren't mallocing 
the proper amount of space for everything resulting in our core aborted error. We added one to our malloced space and it fixed our code and we were passing all tests!
