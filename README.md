
# Lab 5: The Cache Controller Simulator

## 1. Overview & Objectives

In this lab, you will write the core lookup and eviction logic for a 32-bit CPU cache controller simulator. Understanding how memory hierarchies operate is fundamental to writing high-performance software. By completing this simulator, you will gain hands-on experience with:

Bitwise Address Decoding: Extracting tag and set index bits from 32-bit addresses using bitwise shift (>>) and mask (&) operations.

Set-Associative Tag Search: Querying dynamic cache structures to identify hits and cold misses.

FIFO Eviction Policy: Managing line replacement in set-associative caches using a First-In, First-Out (FIFO) strategy.

## 2. Starter Code & Your Task

To allow you to focus entirely on cache mechanics, file handling, command-line parsing, argument validation, and dynamic memory allocation are already provided in the starter code template (`csim.c`).

Your assignment is to implement the core simulation function:

C
```
void access_cache(cache_t cache, uint32_t address, int verbose);
```

### Provided Data Structures

The starter code provides pre-allocated cache structures defined as follows:

C
```
typedef struct {
    int valid;        // 1 if block is valid, 0 if empty
    uint32_t tag;     // Tag bits extracted from address
} cache_line_t;

typedef struct {
    cache_line_t *lines; // Dynamic array of E cache lines
    int next_evict;      // FIFO pointer tracking the next line index to evict
} cache_set_t;

typedef struct {
    int s;               // Set index bits (S = 2^s sets)
    int E;               // Associativity (lines per set)
    int b;               // Block offset bits (B = 2^b bytes)
    cache_set_t *sets;   // Array of S sets
} cache_t;
```

## 3. Memory Addressing Geometry

The simulator operates on 32-bit physical addresses. Every memory address is partitioned into three distinct bit fields determined by command-line parameters $s$ and $b$:

$$
\underbrace{\text{Tag Bits } (t)}_{\text{Upper } 32 - s - b \text{ bits}} \quad \mid \quad \underbrace{\text{Set Index Bits } (s)}_{\text{Middle } s \text{ bits}} \quad \mid \quad \underbrace{\text{Block Offset Bits } (b)}_{\text{Lower } b \text{ bits}}
$$

### Address Extraction Requirements

Inside `access_cache()`, you must calculate:

Set Index: Shift off the $b$ block bits, then apply a mask to isolate the lower $s$ bits.

Tag: Shift off both the $b$ block bits and the $s$ set index bits (shift right by $s + b$).

## 4. Cache Access & Eviction Rules

For every call to `access_cache()`, your function must evaluate the target set and update the global statistics (`hit_count`, `miss_count`, `eviction_count`):

Tag Match (HIT): Search the target set's $E$ lines. If a line has `valid == 1` and its tag matches the extracted tag:

Increment `hit_count`.

If verbose is enabled, print "hit ".

Cold Miss (MISS): If no matching tag is found, but the set contains an empty line (`valid == 0`):

Increment `miss_count`.

Populate the first empty line with `valid = 1` and the current tag.

If verbose is enabled, print "miss ".

Conflict Miss (MISS + EVICTION): If no matching tag is found and all $E$ lines in the set are valid:

Increment both `miss_count` and `eviction_count`.

Evict the line indicated by `target_set->next_evict`.

Overwrite that line's tag with the new tag.

Advance `next_evict` using modulo arithmetic: (next_evict + 1) % E.

If verbose is enabled, print "miss eviction ".

## 5. Trace File & Operation Processing

The provided `main()` driver automatically parses input trace files line-by-line and triggers your `access_cache()` function based on the operation type:

I (Instruction Fetch): Automatically ignored by the driver (Data Cache simulation only).

L (Load): Driver invokes `access_cache()` 1 time.

S (Store): Driver invokes `access_cache()` 1 time.

M (Modify): Driver invokes `access_cache()` 2 times back-to-back (Load phase followed by Store phase).

## 6. Execution & Sample Output

The driver compiled from `csim.c` accepts standard CS:APP command-line flags:

Bash
```
./csim [-v] -s <s> -E <E> -b <b> -t <tracefile>
```

### Standard Execution Output
Bash
```
$ ./csim -s 2 -E 1 -b 4 -t sample.trace
hits:1 misses:4 evictions:1
```

### Verbose Execution Output (-v)
Bash
```
$ ./csim -v -s 2 -E 1 -b 4 -t sample.trace
L 00000000,1 miss
L 00000004,1 hit
L 00000010,1 miss
L 00000000,1 miss eviction
S 00000020,1 miss eviction
hits:1 misses:4 evictions:1
```

## 7. Submission Checklist

[ ] csim.c contains your completed `access_cache()` function implementation.

[ ] Code compiles cleanly without warnings using:

Bash
```
gcc -Wall -Werror -std=c99 -o csim csim.c
```

[ ] Program accurately tracks hits, misses, and evictions across provided test traces.
