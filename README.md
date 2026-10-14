# Melbourne Census KD-Tree Queries

## Academic Context

University: University of Melbourne  
Course: Coursework; exact course number not independently established  
Year: Exact original submission year not independently established  
Project type: individual


## Overview

A handwritten C program for loading Melbourne census business records and answering nearest-location and radius queries using an explicit two-dimensional tree.

## Original Context

Chen Jiang’s algorithms/data-structures coursework, retaining the original alternating-axis nodes and linked duplicate-coordinate buckets. The preserved pre-maintenance snapshot is `43777e53ad251f2da570df720e1b3504b9205ca3`;
its most recent original commit is dated 2022-01-13. This date describes Git evidence,
not necessarily the complete implementation period.

## What I Implemented

CSV-to-record ingestion, alternating-axis tree insertion, duplicate-coordinate chains, recursive nearest and radius queries, comparison counts, and detailed record output.

## Technical Approach

Insertion preserves the original alternating-axis tree and duplicate buckets. Searches use the split-plane distance to decide whether another subtree can contain a nearer point or an in-radius record.

CSV location coordinates retain double precision for tree geometry; original fields remain available in the formatted output. Query processing supports two coordinates for nearest search and an optional nonnegative radius.

## Build / Run

Run `make`, then `./map2 dataset.csv output.txt < queries.txt`. Each query is `x y` or `x y radius`. Run `make test` and `make sanitize`; Python standard library is sufficient for CLI tests.

## Testing

- Five CLI regression methods passed in normal macOS and ASan/UBSan builds, including quoted fields, duplicate coordinates, malformed input, empty trees, 80 random radius queries, supplied data, a 2,000-node degenerate tree, and same-file preservation.
- 442 nearest queries passed an independent exhaustive C distance oracle; the original implementation missed one nearest point in that same harness.
- Strict -O0 Clang core harness and GCC 14 Linux -Werror ASan/UBSan/LeakSanitizer checks passed.
- Evidence: kdtree_before_tests.log, kdtree_nearest_before.log, kdtree_full_after.log, kdtree_linux_tests.log, kdtree_nearest_linux.log.

## 2026 Maintenance

- Bounded CSV field parsing now supports quotes, doubled quotes and empty fields; arguments, headers, numeric fields and queries are validated.
- Corrected split-plane pruning and preserved duplicate-bucket ownership during output.
- Rejected input/output paths that reference the same inode before opening output, protecting source data.
- Added deterministic fixtures, exhaustive distance oracles, sanitizer targets and portable build instructions.

The later work is AI-assisted maintenance, separate from the original implementation.
The exact original snapshot remains on `archive/handwritten-original`.
See [AI_PROJECT_REVIEW.md](AI_PROJECT_REVIEW.md) for a technical review.

## Known Limitations

- The original tree is not balanced; ordered data can produce linear-depth recursion. A 2,000-node degenerate tree was tested, not an unlimited adversarial workload.
- Inputs are finite numeric coordinates; this is planar Euclidean search, not a geographic projection or geodesic library.
- Allocation fault injection and all possible numeric extremes were not exhaustively tested.
