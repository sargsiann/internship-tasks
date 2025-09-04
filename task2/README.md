---

# README

## Overview

This program is a **simple analog of `ls` and `tree`** commands.
It prints the directory structure with indentation and counts different types of files.

---

## Features

* Shows directories and files recursively (with `-r` option).
* Prints **file types** and counts:

  * Regular files
  * Directories
  * Symbolic links
  * Sockets
  * Pipes
  * Block devices
  * Character devices
* Displays tree-like structure with `|` and `-` for indentation.

---

## Usage

```bash
./print_dir <directory> [-r]
```

* `<directory>` → directory to scan
* `-r` → optional flag to enable recursive traversal

**Example:**

```bash
./print_dir /etc -r
```
