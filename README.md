# External Merge Sort

A C++ implementation of an external merge sort system designed to sort datasets larger than available memory.

The project implements a two-phase external sorting algorithm:

1. Split Phase
   - Reads records in memory-sized chunks
   - Extracts keys
   - Sorts records in memory
   - Writes sorted runs to disk

2. Merge Phase
   - Performs a k-way merge using a min priority queue
   - Reads from multiple sorted runs
   - Produces a globally sorted output file

The implementation was tested on large datasets generated using `gensort` and verified using `valsort`.

---

# Implementation

## Split Phase

The split phase:

- Reads fixed-size 100 byte records
- Stores records until the memory limit is reached
- Sorts records using `std::sort`
- Writes sorted runs to disk

Each record contains:

```
[key (10 bytes)][value (90 bytes)]
```

The implementation supports configurable chunk sizes to control memory usage.

---

## Merge Phase

The merge phase uses a k-way merge:

- Each run file contributes one active record
- Records are stored in a min priority queue
- The smallest key is removed
- The next record from that run is inserted

Example:

```
run0 \
run1  \
run2 ---> priority queue ---> output.txt
run3  /
```

---

# Benchmark Environment

Hardware:
- MacBook Pro
- 36GB RAM

Software:
- C++17
- g++ compiler
- Optimization flag: `-O2`

Dataset tools:
- gensort
- valsort

---

# Large Dataset Benchmark

## Dataset

```
Records:
500,000,000

Input size:
~47GB

Record size:
100 bytes

Chunk size:
10,000,000 records

Runs generated:
50
```

---

## Split Phase

Command:

```bash
time ./split_program 10000000
```

Result:

```
Split time: 1323.57 seconds

real:
22:03.84

user:
1270.02s

system:
47.05s
```

---

## Merge Phase

Command:

```bash
time ./merge_program
```

Result:

```
Merge time: 1108.1 seconds

real:
18:28.37

user:
773.49s

system:
52.50s
```

---

## Total Runtime

```
Split:
~22 minutes

Merge:
~18.5 minutes

Total:
~40.5 minutes
```

---

## Validation

Command:

```bash
/Users/shrivas/Downloads/gensort-1.5/valsort output.txt
```

Result:

```
Records: 500000000
Checksum: ee6b6a9da7427ce
Duplicate keys: 0

SUCCESS - all records are in order
```

---

# Smaller Dataset Benchmark

Dataset:

```
Records:
10,000,000

Input size:
~1GB

Chunk size:
1,000,000
```

Results:

```
Split:
14.2376 seconds

Merge:
38.3778 seconds
```

Validation:

```
SUCCESS - all records are in order
```

---

# std::sort Comparison

A comparison was performed against sorting the entire dataset in memory.

Dataset:

```
Records:
10,000,000
```

Result:

```
Records loaded: 10000000

std::sort time:
7.82431 seconds
```

Validation:

```
Records: 10000000
Checksum: 4c48a881c779d5
Duplicate keys: 0

SUCCESS - all records are in order
```

---

# Makefile

The project includes a Makefile for compilation and testing.

Build:

```bash
make
```

Run:

```bash
make run CHUNK_SIZE=10000000
```

Clean:

```bash
make clean
```

---

# Future Optimizations

## KV Separation

Separate keys and values:

Current:

```
[key][value]
```

Future:

```
[key][pointer]
```

with values stored separately.

Benefits:

- Smaller objects during sorting
- Lower memory usage
- Better cache efficiency
- Less data movement

---

## Improved Merge I/O

Current design:

```
sorted keys
      |
      v
random value lookup
```

Future:

```
sorted key-pointer pairs
          |
          v
sequential value reads
```

This reduces expensive random disk access.

---

## Multithreading

Potential improvements:

- Parallel chunk sorting
- Multiple worker threads during split
- Parallel merge strategies

---

# Project Progress

Completed:

- External merge sort implementation
- Configurable chunk sizes
- k-way merge using priority queue
- Large-scale testing (500M records / ~47GB)
- Correctness validation using valsort
- Benchmark comparison against std::sort
- Makefile build system

Future:

- KV separation
- Pointer-based sorting
- Multithreaded optimization
