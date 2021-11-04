#include "common_types.h"
#include "core.h"
#include "ADTLinkedList.h"
#include "ADTEntryList.h"
#include "ADTMap.h"
#include "ADTIndex.h"


#include <stdlib.h>
#include <string.h>
#include <stdio.h>

Query convert_to_query(String string){
    Query query = malloc(sizeof(*query));

    query->words = strdup(string);

    int count = 0;

    String token = strtok(string, " \t\n");

    while (token != NULL ){
        count++;
        token = strtok(NULL, "  \t\n");   
    }
    
    query->length = count;

    return query;
    //Free string na ginete meta tin sinartisi
}

String *Seperate_sentence(Query query){
    String *Array = malloc(sizeof(String)*query->length);
    String dummy = strdup(query->words);

    int i  = 0;

    String token = strtok(dummy, " \t\n");

    Array[i] = strdup(token);


    while( (token = strtok(NULL, " \t\n")) != NULL){
        i++;
        Array[i] = strdup(token);
    }

    free(dummy);
    return Array;
}

List deduplicated_words(String filename){
    FILE *FP = fopen(filename,"r");

    if(!FP){
        puts("Is null");
        return NULL;
    }
    


    char buffer[MAX_WORD_LENGTH+1];
    char letter[2];
    letter[1] = '\0';
    char a;


    List list = list_create((CompareFunc) strcmp);
            

    strcpy(buffer, "");

    while ((a = fgetc(FP)) != EOF){
        if(a == ' ' || a == '\n'){
            if(!list_find(list, buffer)){
                String value = strdup(buffer);
                list_insert(list, value);
            } 
            strcpy(buffer, "");
        } else {
            letter[0] = a;
            strcat(buffer, letter);
        }
    }

    fclose(FP);
    
    return list;

}

const int compare(Query query1, Query query2){
    return strcmp(query1->words, query2->words);
}

int hash_func(Query query){
    return hash_string(query->words);
}

int compare_entries(Entry e1, Entry e2){
    puts(get_entry_word(e1));
    puts(get_entry_word(e2));
    return (strcmp(get_entry_word(e1), get_entry_word(e2)));
}

Map map_of_queries(String filename, EntryList entrylist){
    

    FILE *FP = fopen(filename, "r");
    if(FP == NULL)
        return NULL;

    size_t buffsize = MAX_QUERY_LENGTH;
    String buffer = NULL;

    size_t bytes;


    Map map = map_create( (CompareFunc) compare, 120);
    map_set_hash_function(map, (HashFunc) hash_func);
    int i = 0;
    while((bytes = getline(&buffer, &buffsize, FP)) != -1 ){
        i++;
        Query new_query = convert_to_query(buffer);
        map_insert(map, new_query);

        String *Array = Seperate_sentence(new_query);

        for(int i = 0; i < new_query->length; i++){
            Entry e1 = create_entry(Array[i], NULL);            
            Entry entry = find_entry(entrylist, e1);
            if(entry != NULL){
                list_insert(get_entry_payload(entry), new_query);
                free(Array[i]);
            } else {
                entry = create_entry(Array[i], NULL);
                list_insert(get_entry_payload(entry), new_query);
                add_entry(entrylist, entry);
            }
            List e1_payload = get_entry_payload(e1);
            list_destroy(e1_payload, NULL);
            free(e1);
        }
    
        free(Array);
    }

    
    free(buffer);
    
    fclose(FP);

    return map;

}

const void destroy_query(Query q){
    free(q->words);
    free(q);
}


int main(){
    
    EntryList entrylist = create_entry_list((CompareFunc)compare_entries);
    EntryList result = create_entry_list((CompareFunc)compare_entries);
    
    puts("Inserting queries in hashtable and entrylist...");
    Map map = map_of_queries("../misc/queries.txt", entrylist);


    puts("Deduplicating \"Document1\"...");
    List list = deduplicated_words("../misc/documents/Document1");

    Index index_exact = create_index(MT_EXACT_MATCH, (CompareFunc)compare_entries, 100);

    puts("Building index...");
    build_entry_index(index_exact, entrylist);
     puts("#############################");
     puts("#############################");
     puts("#############################");

    for(ListNode node = list_first(list); node != NULL; node = list_find_next(node)){
        String word = list_node_value(node);
        lookup_entry_index(index_exact, word, 0, result);

        printf("%d\n",get_number_entries(result));
    }

    for(Entry entry = get_first(result); entry != NULL; entry = get_next(entrylist, entry)){
        puts(get_entry_word(entry));
    }



//###################################################################
    list_destroy(result, (DestroyFunc) free);

    list_destroy(list, (DestroyFunc) free);

    destroy_entry_index(index_exact);

    destroy_entry_list(entrylist, (DestroyFunc) destroy_entry);

    map_destroy(map,(DestroyFunc) destroy_query);

    return 0;
}