#include "ADTLinkedList.h"
#include "core.h"

struct jobscheduler{
    int exec_threads; // number of execution threads
    List queue; // a queue that holds submitted jobs / tasks
    List query; // a queue that holds submitted jobs / tasks
    pthread_t* tids; // execution threads
    pthread_mutex_t mtx_queue;
    pthread_mutex_t mtx_query;
    pthread_mutex_t mtx_counter;
    pthread_mutex_t mtx_counter_query;
    pthread_mutex_t mtx_document;
    pthread_mutex_t mtx_finish;
    pthread_mutex_t mtx_exact_match;
    pthread_mutex_t mtx_hamming_dist;
    pthread_mutex_t mtx_edit_dist;
    int counter;
    int counter_query;
    int finish;
    pthread_cond_t queue_not_empty;
    pthread_cond_t query_not_empty;
    pthread_cond_t threads_finished;
    pthread_cond_t queries_finished;
    pthread_cond_t new_doc;
    // mutex, condition variable, ...
};

struct job {
    DocID doc_id;
    char * doc_str;
    ErrorCode errcode;
};

typedef struct jobscheduler* JobScheduler;

typedef struct job* Job;

void initialize_scheduler(int execution_threads, JobScheduler jscheduler);

void submit_job(JobScheduler sch, Job j);

void submit_query(JobScheduler sch, Query query);

int execute_all_jobs(JobScheduler sch);

int wait_all_tasks_finish(JobScheduler sch);

ErrorCode destroy_scheduler(JobScheduler sch);

void* help_MatchDocument (void* job);

void* Insert_Query(void* job);

