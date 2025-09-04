
---

# README

## Overview

This project has **two approaches** for copying `.c` files and a **performance checker** for running tests.

---

## Copying Files

### 1. `find_copy_1.sh`

* POSIX `sh` version using `find` + `while read`.
* Copies `.c` files and adds `.orig` to each.
* Usage:

```sh
sh find_copy_1.sh
```

### 2. `find_copy_2.sh`

* Bash version using arrays and `for` loop.
* Copies `.c` files with `.orig`.
* Usage:

```bash
./find_copy_2.sh
```

---

## Performance Checker: `perf_check.sh`

* Runs an executable multiple times and measures runtime.
* Shows **Min**, **Max**, and **Average** times (rounded to 0.01s).
* Usage:

```bash
./perf_check.sh ./<exe_name> <test_nums>
```

* Example:

```bash
./perf_check.sh ./my_test.sh 100
```

---
