---

# print\_dir

**print\_dir** is a lightweight and safe C tool for Linux that displays a directory’s contents in a tree-like format.
It also counts directories, files, links, sockets, pipes, and devices — making it a handy utility for quickly inspecting folder structures.

---

## Features

* **Tree View**: Displays files and folders in a clear hierarchical structure.
* **Optional Recursion**: Use `-r` to explore all subfolders.
* **Object Counting**: Tracks:

  * Directories
  * Files
  * Symbolic links
  * Sockets
  * Pipes
  * Block devices
  * Character devices
* **Robust Path Handling**: Works safely with long paths and redundant slashes (e.g., `dir///file`).
* **Stack Safety**: Manages deep folder structures across hardware types without crashing.
* **Resource Limits**: Stays within system file descriptor limits.
* **Signal Handling**: Cleanly exits on interruptions (`Ctrl+C`).
* **Cross-Platform Hardware Support**: Runs on x86, x86\_64, ARM, AArch64, and RISC-V.

---

## Installation

### Requirements

* **System**: Linux (tested on Ubuntu/Debian)
* **Compiler**: GCC or any C99-compatible compiler
* **Libraries**: Standard C library (no extra dependencies)

### Build Instructions

```bash
# Compile with Make
make print_dir
# or build all targets
make all
```

---

## Usage

```bash
./print_dir <directory> [-r]
```

### Arguments

* `<directory>`: Path to the folder to list (required).
* `-r`: Recursively list subfolders (optional).

---

## Examples

### 1. List one folder

```bash
./print_dir /home/user
```

**Output:**

```
|- file1.txt
|- folder1
|- link1
Total: 1 directories, 1 files, 1 links, 0 sockets, 0 pipes, 0 block devices, 0 character devices
```

---

### 2. List recursively

```bash
./print_dir /home/user -r
```

**Output:**

```
|- file1.txt
|- folder1
|---- file2.txt
|---- folder2
|------ file3.txt
|- link1
Total: 2 directories, 3 files, 1 links, 0 sockets, 0 pipes, 0 block devices, 0 character devices
```

---

## Output Format

### Tree View

* Lines start with `|`
* Depth is indicated with `--` (two dashes per level)

### Summary

At the end, a summary shows counts:

```
Total: <N> directories, <M> files, <K> links, <L> sockets, <P> pipes, <Q> block devices, <R> character devices
```

---

## Improvements Over Previous Versions

* **Path Handling**: Correctly handles long names and redundant slashes.
* **Stack Safety**: Safer recursion for deep directory trees.
* **File Limits**: Manages open file descriptors responsibly.
* **Signal Support**: Exits gracefully on Interuptions,Termination,SegFault,handle Broken Pipes
* **Structured**: Structured files for project