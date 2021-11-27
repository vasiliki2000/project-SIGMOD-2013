#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ADTIndex.h"
#include "misc.h"
#include "ADTMap.h"
#include "ADTBKTree.h"
#include "core.h"

Index Index_Exact;
Index Index_Edit;
Index Index_Hamming;

Map Map_Queries;


int compare_entries(Entry e1, Entry e2){
    return strcmp(e1->word, e2->word);
}

int hash_entries(Entry entry){
    return hash_string(entry->word);
}

int hash_queries(Query query){
    return query->queryID;
}

Map get_map_queries(){
    return Map_Queries;
}

Index get_index_exact(){
    return Index_Exact;
}

Index get_index_edit(){
    return Index_Edit;
}

Index get_index_hamming(){
    return Index_Hamming;
}


void destroy_entry(Entry entry){
    list_destroy(entry->payload, NULL);
    free(entry->word);
    free(entry);
}

void destroy_entry_full(Entry entry) {
    list_destroy(entry->payload, (DestroyFunc) free);
    free(entry->word);
    free(entry);
}

void destroy_query(Query query){
    free(query->words);
    free(query);
}

int compare_ids(int *a, int *b){
    return *a-*b;
}

int compare_queries(Query q1, Query q2){
    return q1->queryID - q2->queryID;
}


ErrorCode InitializeIndex() {

    Map_Queries = map_create((CompareFunc)compare_queries, 100);
    if(Map_Queries == NULL)
        return EC_NO_AVAIL_RES;

    map_set_hash_function(Map_Queries, (HashFunc) hash_queries);

    Index_Exact = create_index(MT_EXACT_MATCH, (CompareFunc)compare_entries, 100);
    if(Index_Exact == NULL)
        return EC_NO_AVAIL_RES;

    map_set_hash_function(index_index(Index_Exact), (HashFunc) hash_entries);

/////////////////////////////////////////////////////////////////////////////////////

    Index_Edit = create_index(MT_EDIT_DIST, (CompareFunc)compare_entries, 0);
    if(Index_Edit == NULL)
        return EC_NO_AVAIL_RES;

/////////////////////////////////////////////////////////////////////////////////////

    Index_Hamming = create_index(MT_HAMMING_DIST, (CompareFunc)compare_entries, 0);
    if(Index_Hamming == NULL)
        return EC_NO_AVAIL_RES;


    return EC_SUCCESS;
}

ErrorCode DestroyIndex(){

    destroy_entry_index(Index_Exact,(DestroyFunc) destroy_entry);
    destroy_entry_index(Index_Edit,(DestroyFunc) destroy_entry);
    destroy_entry_index(Index_Hamming,(DestroyFunc) destroy_entry);
    map_destroy(Map_Queries, (DestroyFunc)destroy_query);

    return EC_SUCCESS;
}


int Insert_Query(Query query){
    
    String *Array = Seperate_sentence(query);

    for(int i = 0; i < query->length; i++){

        Entry entry = create_entry(Array[i], (CompareFunc) compare_queries);
        list_insert(entry->payload, query);

        if (query->match_type == MT_EXACT_MATCH) {
            Entry node = map_find(index_index(Index_Exact), entry);
            if(!node){
                map_insert(index_index(Index_Exact), entry);
            } else {
                destroy_entry(entry);
                Entry entry2 = node;
                list_insert(entry2->payload, query);
            }

        } else if (query->match_type == MT_EDIT_DIST) {
            bk_insert(index_index(Index_Edit), entry);

        } else if (query->match_type == MT_HAMMING_DIST) {
            bk_insert(index_index(Index_Hamming), entry);

        } else {
            return EC_NO_AVAIL_RES;
        }
    }

    free(Array);
    return EC_SUCCESS;
}


ErrorCode StartQuery(QueryID query_id, const char* query_str, MatchType match_type, unsigned int match_dist){

    int count = 0;
    String string = strdup(query_str);
    String token = strtok(string, " \t\n");

    while (token != NULL ){                             // Count number of words
        count++;
        token = strtok(NULL, "  \t\n");   
    }

    free(string);

    Query query = malloc(sizeof(*query));
	query->queryID=query_id;
    query->words = strdup(query_str);
	query->match_type=match_type;
	query->match_dist=match_dist;
    query->length = count;

    map_insert(Map_Queries, query);

    Insert_Query(query);


    return EC_SUCCESS;
}

ErrorCode EndQuery(QueryID query_id){
    return EC_SUCCESS;
}

ErrorCode MatchDocument (DocID doc_id, const char * doc_str){
    return EC_SUCCESS;
}

ErrorCode GetNextAvailRes (DocID * p_doc_id, unsigned int * p_num_res, QueryID ** p_query_ids){
    return EC_SUCCESS;
}