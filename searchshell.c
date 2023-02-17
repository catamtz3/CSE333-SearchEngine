/*
 * Copyright ©2023 Justin Hsia.  All rights reserved.  Permission is
 * hereby granted to students registered for University of Washington
 * CSE 333 for use solely during Winter Quarter 2023 for purposes of
 * the course.  No other use, copying, distribution, or modification
 * is permitted without prior written consent. Copyrights for
 * third-party components of this work must be honored.  Instructors
 * interested in reusing these course materials should contact the
 * author.
 */

// Feature test macro for strtok_r (c.f., Linux Programming Interface p. 63)
#define _XOPEN_SOURCE 600

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>

#include "libhw1/CSE333.h"
#include "./CrawlFileTree.h"
#include "./DocTable.h"
#include "./MemIndex.h"

//////////////////////////////////////////////////////////////////////////////
// Helper function declarations, constants, etc
static void Usage(void);
// begin processing the user's input
static void ProcessQueries(DocTable* dt, MemIndex* mi);
// get the user's command line input
static int GetNextLine(FILE* f, char** ret_str);


//////////////////////////////////////////////////////////////////////////////
// Main
int main(int argc, char** argv) {
  if (argc != 2) {
    Usage();
  }

  // Implement searchshell!  We're giving you very few hints
  // on how to do it, so you'll need to figure out an appropriate
  // decomposition into functions as well as implementing the
  // functions.  There are several major tasks you need to build:
  //
  //  - Crawl from a directory provided by argv[1] to produce and index
  //  - Prompt the user for a query and read the query from stdin, in a loop
  //  - Split a query into words (check out strtok_r)
  //  - Process a query against the index and print out the results
  //
  // When searchshell detects end-of-file on stdin (cntrl-D from the
  // keyboard), searchshell should free all dynamically allocated
  // memory and any other allocated resources and then exit.
  //
  // Note that you should make sure the fomatting of your
  // searchshell output exactly matches our solution binaries
  // to get full points on this part.
  char *dir = argv[1];
  // char* query;
  DocTable *table;
  MemIndex *index;

  printf("Indexing '%s'\n", dir);
  if (!CrawlFileTree(dir, &table, &index)) {
    Usage();
  }
  ProcessQueries(table, index);

  // free allocated space
  DocTable_Free(table);
  MemIndex_Free(index);

  return EXIT_SUCCESS;
}


//////////////////////////////////////////////////////////////////////////////
// Helper function definitions

static void Usage(void) {
  fprintf(stderr, "Usage: ./searchshell <docroot>\n");
  fprintf(stderr,
          "where <docroot> is an absolute or relative " \
          "path to a directory to build an index under.\n");
  exit(EXIT_FAILURE);
}

// begin processing the user's input
static void ProcessQueries(DocTable* dt, MemIndex* mi) {
  LinkedList *result;
  char *output;
  int i, num_elements;

  // prompting
  while (1) {
    char *user_query = (char*) malloc(sizeof(char) * 1024);
    char **user_query_copy = (char**) malloc(sizeof(char) * 1024);
    int num = GetNextLine(stdin, &user_query);
    if (num != 0) {
      free(user_query);
      free(user_query_copy);
      return;
    }
    *(user_query + (strlen(user_query) - 1)) = '\0';
    // make sure all of the user's inputs are lowercase
    for (int i = 0; i < strlen(user_query); i++) {
      *(user_query + i) = tolower(*(user_query + i));
    }

    // begin splitting the user's input
    i = 0;
    while (1) {
      if (i == 0) {
        *(user_query_copy + i) = strtok_r(user_query, " ", &output);
      } else {
        *(user_query_copy + i) = strtok_r(NULL, " ", &output);
      }
      if (*(user_query_copy + i) == NULL) {
        break;
      }
      i++;
    }

    // begin searching for documents with user's input
    result = MemIndex_Search(mi, user_query_copy, i);
    // if no documents available, free everything
    if (result == NULL) {
      free(user_query);
      free(user_query_copy);
      LinkedList_Free(result, (LLPayloadFreeFnPtr)free);
      return;
    }
    LLIterator *ll_it = LLIterator_Allocate(result);
    num_elements = LinkedList_NumElements(result);
    SearchResult *match;
    // go through each document to find
    // the number of occurances of a specified word(s)
    for (int j = 0; j < num_elements; j++) {
      LLIterator_Get(ll_it, (LLPayload_t)&match);
      DocID_t doc_id = match -> doc_id;
      char *doc_name = DocTable_GetDocName(dt, doc_id);

      // print results
      // (i.e., document and number of occurances of each word(s))
      printf("  %s", doc_name);
      printf(" (%d) \n", match->rank);
      LLIterator_Next(ll_it);
    }

    // all done! make sure to free everything
    LinkedList_Free(result, (LLPayloadFreeFnPtr)free);
    LLIterator_Free(ll_it);
    free(user_query);
    free(user_query_copy);
  }
}

// get the user's command line input
static int GetNextLine(FILE *f, char **ret_str) {
  printf("enter query:\n");
  fgets(*ret_str, 1024, f);

  return feof(f);
}
