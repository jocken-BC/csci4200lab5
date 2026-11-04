/* 
 * CS:APP Lab 5: Cache Controller Simulator (32-Bit Address Version)
 *
 * Name:   [Student Name]
 * Andrew: [Student ID / Email]
 */

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

/* Global counters for cache statistics */
int hit_count = 0;
int miss_count = 0;
int eviction_count = 0;

/* Structural definition of a single cache line */
typedef struct {
    int valid;
    uint32_t tag;
} cache_line_t;

/* Structural definition of a cache set */
typedef struct {
    cache_line_t *lines;
    int next_evict; // Points to the next line to evict under FIFO
} cache_set_t;

/* Structural definition of the entire cache */
typedef struct {
    int s;               // Set index bits (S = 2^s sets)
    int E;               // Associativity (lines per set)
    int b;               // Block offset bits (B = 2^b bytes)
    cache_set_t *sets;   // Array of sets
} cache_t;

/* Function Prototypes */
cache_t init_cache(int s, int E, int b);
void free_cache(cache_t cache);
void access_cache(cache_t cache, uint32_t address, int verbose);
void print_usage(char *argv[]);

int main(int argc, char *argv[]) {
    int s = -1, E = 0, b = 0;
    char *trace_file = NULL;
    int verbose = 0;
    int opt;

    while ((opt = getopt(argc, argv, "vs:E:b:t:")) != -1) {
        switch (opt) {
            case 'v':
                verbose = 1;
                break;
            case 's':
                s = atoi(optarg);
                break;
            case 'E':
                E = atoi(optarg);
                break;
            case 'b':
                b = atoi(optarg);
                break;
            case 't':
                trace_file = optarg;
                break;
            default:
                print_usage(argv);
                exit(1);
        }
    }

    if (s < 0 || E <= 0 || b <= 0 || trace_file == NULL) {
        printf("Error: Missing required command line arguments.\n");
        print_usage(argv);
        exit(1);
    }

    if (s + b >= 32) {
        printf("Error: Invalid cache configuration. (s + b = %d) must be less than 32 bits.\n", s + b);
        print_usage(argv);
        exit(1);
    }

    cache_t cache = init_cache(s, E, b);

    FILE *fp = fopen(trace_file, "r");
    if (!fp) {
        perror("Error opening trace file");
        free_cache(cache);
        exit(1);
    }

    char operation;
    uint32_t address;
    int size;

    /* Read 32-bit hex addresses (%x) */
    while (fscanf(fp, " %c %x,%d", &operation, &address, &size) == 3) {
        if (operation == 'I') continue;

        if (verbose) {
            printf("%c %08x,%d ", operation, address, size);
        }

        if (operation == 'L' || operation == 'S') {
            access_cache(cache, address, verbose);
        } else if (operation == 'M') {
            access_cache(cache, address, verbose);
            access_cache(cache, address, verbose);
        }

        if (verbose) {
            printf("\n");
        }
    }

    fclose(fp);
    free_cache(cache);

    printf("hits:%d misses:%d evictions:%d\n", hit_count, miss_count, eviction_count);
    return 0;
}

cache_t init_cache(int s, int E, int b) {
    cache_t cache;
    cache.s = s;
    cache.E = E;
    cache.b = b;

    int num_sets = 1 << s;

    cache.sets = (cache_set_t *)malloc(num_sets * sizeof(cache_set_t));
    if (!cache.sets) {
        fprintf(stderr, "Fatal error: Dynamic memory allocation failed.\n");
        exit(1);
    }

    for (int i = 0; i < num_sets; i++) {
        cache.sets[i].lines = (cache_line_t *)malloc(E * sizeof(cache_line_t));
        if (!cache.sets[i].lines) {
            fprintf(stderr, "Fatal error: Dynamic memory allocation failed.\n");
            exit(1);
        }
        cache.sets[i].next_evict = 0;
        for (int j = 0; j < E; j++) {
            cache.sets[i].lines[j].valid = 0;
            cache.sets[i].lines[j].tag = 0;
        }
    }

    return cache;
}

void free_cache(cache_t cache) {
    int num_sets = 1 << cache.s;
    for (int i = 0; i < num_sets; i++) {
        free(cache.sets[i].lines);
    }
    free(cache.sets);
}

/*
 * TODO: Implement Core Cache Lookup, Hit/Miss/Eviction & FIFO logic.
 *
 * Instructions:
 *  1. Extract set index and tag from 32-bit address using bitwise shifts and masks.
 *     - set_index = (address >> b) & ((1U << s) - 1)
 *     - tag = address >> (s + b)
 *
 *  2. Search target set lines for a HIT (valid == 1 AND line.tag == tag).
 *     - If HIT: Increment hit_count, if (verbose) printf("hit "); and return.
 *
 *  3. Handle MISS (empty line available, valid == 0):
 *     - Increment miss_count.
 *     - Populate line (valid = 1, tag = tag).
 *     - if (verbose) printf("miss "); and return.
 *
 *  4. Handle MISS + EVICTION (set is full, all lines valid == 1):
 *     - Increment miss_count AND eviction_count.
 *     - Overwrite target_set->lines[target_set->next_evict] with new tag.
 *     - Update FIFO pointer: target_set->next_evict = (target_set->next_evict + 1) % E;
 *     - if (verbose) printf("miss eviction ");
 */
void access_cache(cache_t cache, uint32_t address, int verbose) {

    /* [STUDENT TODO: Implement 32-bit bitwise parsing & FIFO state machine] */

    /* 1. Extract set index and tag using 32-bit bitwise operations */

    /* 2. Check for Cache HIT                                         */
    /*    Include the following code when a hit is encountered:       */
    /*       hit_count++;                                             */
    /*       if (verbose) printf("hit ");                             */

    /* 3. Cache MISS = Search for empty / invalid line                */
    /*    Include the following code when a miss is encountered:      */
    /*       miss_count++;                                            */
    /*       if (verbose) printf("miss ");                            */

    /* 4. Cache MISS + EVICTION - FIFO replacement policy             */
    /*    Include the following code when an eviction is encountered: */
    /*       eviction_count++;                                        */
    /*       if (verbose) printf("eviction ");                        */

}

void print_usage(char *argv[]) {
    printf("Usage: %s [-v] -s <s> -E <E> -b <b> -t <tracefile>\n", argv[0]);
    printf("Options:\n");
    printf("  -v          Optional verbose flag that displays trace info\n");
    printf("  -s <s>      Number of set index bits (S = 2^s sets)\n");
    printf("  -E <E>      Associativity (number of lines per set)\n");
    printf("  -b <b>      Number of block offset bits (B = 2^b bytes)\n");
    printf("  -t <file>   Trace file name\n");
}
