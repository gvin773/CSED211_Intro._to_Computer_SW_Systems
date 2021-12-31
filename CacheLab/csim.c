//name: Gyubin Kim
//ID: 20200516
#include "cachelab.h"
#include <getopt.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
typedef unsigned long long ull;

typedef struct _line
{
    int v; //valid bit
    ull tag;
    int cnt; //counter for LRU replacement policy
} line;

char *t; //tracefile
int s, E, b;

void init(int argc, char *argv[], int *flag_verbose)
{
    int c; //option
    while((c = getopt(argc, argv, "hvs:E:b:t:")) != -1) //[-hv] -s <s> -E <E> -b <b> -t <tracefile>
    {
        switch(c)
        {
            case 'h': break;
            case 'v': *flag_verbose = 1; break;
            case 's': s = atoi(optarg); break;
            case 'E': E = atoi(optarg); break;
            case 'b': b = atoi(optarg); break;
            case 't': t = optarg; break;
        }
    }
}

void update_hme(ull addr, ull size, int *LRU_cnt, line **cache, int flag_verbose, int *hit_count, int *miss_count, int *eviction_count) //udpate hit, miss, eviction count
{
    ull tag_bits = addr >> (s+b);
    ull set_index = (addr>>b) - (tag_bits<<s);
    int i, flag_hit = 0, flag_miss = 0, min_cnt, min_idx;

    (*LRU_cnt)++;
    for(i = 0; i < E; i++)
    {
        if(cache[set_index][i].v && (cache[set_index][i].tag == tag_bits))
        {
            flag_hit = 1;
            break;
        }
    } //hit check

    if(flag_hit) //hit
    {
        (*hit_count)++;
        cache[set_index][i].cnt = *LRU_cnt;
        if(flag_verbose) printf("%llx,%llu hit ", addr, size);
    }
    else //miss
    {
        if(flag_verbose) printf("%llx,%llu miss ", addr, size);
        min_cnt = *LRU_cnt;
        min_idx = 0;
        for(i = 0; i < E; i++)
        {
            if(cache[set_index][i].cnt < min_cnt) //get LRU line
            {
                min_cnt = cache[set_index][i].cnt;
                min_idx = i;
            }
            if(!cache[set_index][i].v && (cache[set_index][i].tag == tag_bits)) //get miss idx
            {
                flag_miss = 1;
                min_idx = i;
                break;
            }
        }

        if(cache[set_index][min_idx].v && !flag_miss) //eviction
        {
            (*eviction_count)++;
            if(flag_verbose) printf("eviction ");
        }

        (*miss_count)++;
        cache[set_index][min_idx].v = 1;
        cache[set_index][min_idx].tag = tag_bits;
        cache[set_index][min_idx].cnt = *LRU_cnt;
    }
}

void simulate(line **cache, char *t, int *hit_count, int *miss_count, int *eviction_count, int flag_verbose)
{
    FILE *file;
    char inst;
    ull addr, size;
    int LRU_cnt = 0;

    if((file = fopen(t, "r")) != NULL)
    {
        while(fscanf(file, " %c %llx,%llu", &inst, &addr, &size) == 3)
        {
            if(inst != 'I') //if instruction isn't I
            {
                switch(inst) //instruction; L - load, S - store, M - modify
                {
                    case 'L':
                        if(flag_verbose) printf("L ");
                        update_hme(addr, size, &LRU_cnt, cache, flag_verbose, hit_count, miss_count, eviction_count); break;
                    case 'S':
                        if(flag_verbose) printf("S ");
                        update_hme(addr, size, &LRU_cnt, cache, flag_verbose, hit_count, miss_count, eviction_count); break;
                    case 'M':
                        if(flag_verbose) printf("M ");
                        update_hme(addr, size, &LRU_cnt, cache, flag_verbose, hit_count, miss_count, eviction_count);
                        update_hme(addr, size, &LRU_cnt, cache, flag_verbose, hit_count, miss_count, eviction_count); break;
                }
                if(flag_verbose) puts("");
            }
        }
    }

    fclose(file);
}

int main(int argc, char *argv[])
{
    int hit_count = 0, miss_count = 0, eviction_count = 0;
    int S;
    //char *t;
    int flag_verbose = 0;
    line **cache;
    int i, j;

    init(argc, argv, &flag_verbose); //init val to allocate cache

    S = pow(2, s);
    cache = (line**)malloc(sizeof(line*)*S);
    for(i = 0; i < S; i++)
    {
        *(cache+i) = (line*)malloc(sizeof(line)*E);
        for(j = 0; j < E; j++)
        {
            cache[i][j].v = 0;
            cache[i][j].tag = 0;
            cache[i][j].cnt = 0;
        }
    } //allocate and init cache

    simulate(cache, t, &hit_count, &miss_count, &eviction_count, flag_verbose); //simulate cache memory

    printSummary(hit_count, miss_count, eviction_count);
    for(i = 0; i < S; i++) free(cache[i]);
    free(cache);
    return 0;
}