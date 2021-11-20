/*
 *  Authors: Vasiliki Efstathiou Nikos Eftychiou
 *  File   : misc_test.c     
*/

#include "acutest.h"
#include "misc.h"
#include "ADTEntryList.h"
#include "ADTLinkedList.h"


void test_find_complete_queries(){

  EntryList entrylist = create_entry_list((CompareFunc) compare_entries);

  struct query q1 = {
    "this test looks good",
    4
  };

  struct query q2 = {
    "what does this thing",
    4
  };

  struct query q3 = {
    "marver looks good right",
    4
  };


  String word = strdup("this");
  Entry e1 = create_entry(word, (CompareFunc) compare_queries);
  List list = get_entry_payload(e1);
  list_insert(list, &q1);
  list_insert(list, &q2);
  add_entry(entrylist,e1);

  word = strdup("test");
  Entry e2 = create_entry(word, (CompareFunc) compare_queries);
  list = get_entry_payload(e2);
  list_insert(list, &q1);
  add_entry(entrylist,e2);


  word = strdup("looks");
  Entry e3 = create_entry(word, (CompareFunc) compare_queries); 
  list = get_entry_payload(e3);
  list_insert(list, &q1);
  list_insert(list, &q3);
  add_entry(entrylist,e3);

  
  word = strdup("good");
  Entry e4 = create_entry(word, (CompareFunc) compare_queries); 
  list = get_entry_payload(e4);
  list_insert(list, &q1);
  list_insert(list, &q3);
  add_entry(entrylist,e4);
  

  word = strdup("marvel");
  Entry e5 = create_entry(word, (CompareFunc) compare_queries); 
  list = get_entry_payload(e5);
  list_insert(list, &q3);
  add_entry(entrylist,e5);
  


  word = strdup("right");
  Entry e6 = create_entry(word, (CompareFunc) compare_queries); 
  list = get_entry_payload(e6);
  list_insert(list, &q3);
  add_entry(entrylist,e6);
  

  word = strdup("does");
  Entry e7 = create_entry(word, (CompareFunc) compare_queries);
  list = get_entry_payload(e7);
  list_insert(list, &q2);
  add_entry(entrylist,e7);



  list = find_complete_queries(entrylist);
  
  ListNode node = list_first(list);
  Query query = list_node_value(node);

  TEST_ASSERT((strcmp(q1.words,query->words) == 0 ));

  node  = list_find_next(node);
  query = list_node_value(node);

  TEST_ASSERT(( strcmp(q3.words,query->words) == 0 ) );

  node = list_find_next(node);

  TEST_ASSERT(node == NULL);

  list_destroy(list, NULL);

  destroy_entry_list(entrylist, (DestroyFunc) destroy_entry);

}


void test_seperate_sentence(){
  
  struct query q1 = {
    "This test looks good", 
    4
  };

  String *Array = Seperate_sentence(&q1);

  for(int i = 0; i < q1.length; i++){
    TEST_ASSERT(Array[i] != NULL);
  }

  for (int i = 0; i < q1.length; i++){
    free(Array[i]);
  }

  free(Array);


}


void test_deduplicate(){
    
    Map map = deduplicated_words_map("../misc/documents/Document1");
    
    TEST_ASSERT(map != NULL);

    TEST_ASSERT(map_find(map, "sotira") != NULL);
    TEST_ASSERT(map_find(map, "micro") != NULL);
    TEST_ASSERT(map_find(map, "music") != NULL);
    TEST_ASSERT(map_find(map, "portrait") != NULL);
    TEST_ASSERT(map_find(map, "book") != NULL);
    TEST_ASSERT(map_find(map, "forest") != NULL);
    TEST_ASSERT(map_find(map, "kiss") != NULL);
    TEST_ASSERT(map_find(map, "hospital") != NULL);
    TEST_ASSERT(map_find(map, "chocolate") != NULL);
    TEST_ASSERT(map_find(map, "manager") != NULL);
    TEST_ASSERT(map_find(map, "coffee") != NULL);
    TEST_ASSERT(map_find(map, "basket") != NULL);
    TEST_ASSERT(map_find(map, "somethingelse") == NULL);

    TEST_ASSERT(map_capacity(map) == 12);

    map_destroy(map, free);

}




TEST_LIST = {
  { "find_complete_queries", test_find_complete_queries },
  { "seperate_sentence", test_seperate_sentence },
	{ "deduplicate_map", test_deduplicate },

	{ NULL, NULL } // τερματίζουμε τη λίστα με NULL
}; 