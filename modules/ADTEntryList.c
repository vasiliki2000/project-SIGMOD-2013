/*
 *  Authors: Vasiliki Efstathiou Nikos Eftychiou
 *  File   : ADTEntryList.c     
*/


#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "ADTEntryList.h"

struct entry{
    String word;                //Holds keyword for queries who have the same keyword
    List payload;               //List that holds queries that have the same keyword "word"
};


Entry create_entry(String word, CompareFunc compare) {      
    Entry entry = malloc(sizeof(*entry));                   // Create new entry

    entry->word = word;
    entry->payload = list_create(compare);                  // Create empty list for payload

    return entry;
}


String get_entry_word(Entry entry){
    assert(entry);
    return entry->word;
}


List get_entry_payload(Entry entry){
    assert(entry);
    return entry->payload;
}


ErrorCode destroy_entry(Entry entry) {
    assert(entry);

    free(entry->word);                              
    list_destroy(entry->payload, NULL);
    free(entry);

    return EC_SUCCESS;
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////


EntryList create_entry_list(CompareFunc compare) {
    EntryList entrylist = list_create(compare);

    return entrylist;
}


unsigned int get_number_entries(EntryList entrylist) {
    return list_size(entrylist);
}


ErrorCode add_entry(EntryList entrylist, Entry new_entry){
    list_insert(entrylist, new_entry);
    return EC_SUCCESS;      
}


Entry get_first(EntryList entrylist){
    ListNode node = list_first(entrylist);
    if(node == NULL)
        return NULL;
        
    return list_node_value(node);
}


Entry get_next(EntryList entrylist, Entry current_entry){

    ListNode node = list_find(entrylist, current_entry);

    if (!node)                                      // If no node was found
        return NULL;
    else {
        node = list_find_next(node);

        if (node) {
            Entry entry = list_node_value(node);
            return entry;
        } else 
            return NULL;
    }
}


Entry find_entry(EntryList entrylist, Entry current_entry) {
    assert(entrylist);

    ListNode node = list_find(entrylist, current_entry);

    if (node) {
        Entry entry = list_node_value(node);
        return entry;
    } else
        return NULL;
}


ErrorCode destroy_entry_list(EntryList entrylist, DestroyFunc destroy_value) {
    assert(entrylist);
    list_destroy(entrylist, (DestroyFunc)destroy_value);
    return EC_SUCCESS;
}

