# AI Project Review

## Project Summary

A handwritten C program for loading Melbourne census business records and answering nearest-location and radius queries using an explicit two-dimensional tree.

Chen Jiang’s algorithms/data-structures coursework, retaining the original alternating-axis nodes and linked duplicate-coordinate buckets.

## Original Engineering Strengths

The original project represents geographic search directly: each node owns a census record, alternates its split axis with depth, and groups records at identical coordinates in a linked bucket. The structures connect the assignment’s real dataset to inspectable search mechanics.

Nearest and radius operations are handwritten recursive traversals. Their comparison counts expose the tradeoff between pruning and visiting every record, making both the algorithm and its behavior available for discussion.

## Technical Highlights

Insertion preserves the original alternating-axis tree and duplicate buckets. Searches use the split-plane distance to decide whether another subtree can contain a nearer point or an in-radius record.

CSV location coordinates retain double precision for tree geometry; original fields remain available in the formatted output. Query processing supports two coordinates for nearest search and an optional nonnegative radius.

## 2026 Maintenance Improvements

- Bounded CSV field parsing now supports quotes, doubled quotes and empty fields; arguments, headers, numeric fields and queries are validated.
- Corrected split-plane pruning and preserved duplicate-bucket ownership during output.
- Rejected input/output paths that reference the same inode before opening output, protecting source data.
- Added deterministic fixtures, exhaustive distance oracles, sanitizer targets and portable build instructions.

The changes retain the project's original mechanisms and strengthen their execution
boundaries and verification contract. They do not substitute a new project concept.

## Verification

- Five CLI regression methods passed in normal macOS and ASan/UBSan builds, including quoted fields, duplicate coordinates, malformed input, empty trees, 80 random radius queries, supplied data, a 2,000-node degenerate tree, and same-file preservation.
- 442 nearest queries passed an independent exhaustive C distance oracle; the original implementation missed one nearest point in that same harness.
- Strict -O0 Clang core harness and GCC 14 Linux -Werror ASan/UBSan/LeakSanitizer checks passed.
- Evidence: kdtree_before_tests.log, kdtree_nearest_before.log, kdtree_full_after.log, kdtree_linux_tests.log, kdtree_nearest_linux.log.

Verification is limited to the commands and cases listed above; passing these checks is not a claim of universal correctness.

## Skills Demonstrated

C data structures, spatial reasoning, CSV state machines, ownership, geometric pruning, differential oracles and memory-safety verification.

## Engineering Assessment

The project is a concrete spatial-algorithm implementation with meaningful dataset and output boundaries. Maintenance preserves its original tree and makes search correctness independently checkable.

## Maintenance Transparency

The original implementation is preserved in `archive/handwritten-original` at
`43777e53ad251f2da570df720e1b3504b9205ca3`. The reviewed maintained version contains a later AI-assisted engineering
maintenance pass focused on verification, robustness, testing, and documentation
while retaining the original project approach.
