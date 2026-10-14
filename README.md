# Melbourne Census KD-Tree Queries

A handwritten C program for loading Melbourne census business records and answering nearest-location and radius queries using an explicit two-dimensional tree.

Chen Jiang’s University of Melbourne computing coursework.

University: University of Melbourne  

## Implementation

CSV-to-record ingestion, alternating-axis tree insertion, duplicate-coordinate chains, recursive nearest and radius queries, comparison counts, and detailed record output.

## Technical approach

Insertion preserves the original alternating-axis tree and duplicate buckets. Searches use the split-plane distance to decide whether another subtree can contain a nearer point or an in-radius record.

CSV location coordinates retain double precision for tree geometry; original fields remain available in the formatted output. Query processing supports two coordinates for nearest search and an optional nonnegative radius.

## Run

Run `make`, then `./map2 dataset.csv output.txt < queries.txt`. Each query is `x y` or `x y radius`. Run `make test` and `make sanitize`; Python standard library is sufficient for CLI tests.

## Checks and results

- Five CLI regression methods passed in normal macOS and ASan/UBSan builds, including quoted fields, duplicate coordinates, malformed input, empty trees, 80 random radius queries, supplied data, a 2,000-node degenerate tree, and same-file preservation.
- 442 nearest queries passed an independent exhaustive C distance oracle; the original implementation missed one nearest point in that same harness.
- Strict -O0 Clang core harness and GCC 14 Linux -Werror ASan/UBSan/LeakSanitizer checks passed.

## 2026 maintenance

- Bounded CSV field parsing now supports quotes, doubled quotes and empty fields; arguments, headers, numeric fields and queries are validated.
- Corrected split-plane pruning and preserved duplicate-bucket ownership during output.
- Rejected input/output paths that reference the same inode before opening output, protecting source data.
- Added deterministic fixtures, exhaustive distance oracles, sanitizer targets and portable build instructions.

These changes are 2026 AI-assisted maintenance of the original student implementation.

## Usage notes

- The original tree is not balanced; ordered data can produce linear-depth recursion. A 2,000-node degenerate tree was tested, not an unlimited adversarial workload.
- Inputs are finite numeric coordinates; this is planar Euclidean search, not a geographic projection or geodesic library.
- Allocation fault injection and all possible numeric extremes were not exhaustively tested.
