# C File Backup Fixed Script 

Creates secure backups of all `.c` files by copying them to `.orig` versions.

## What it does

Finds all `.c` files and creates backup copies like `file.c` → `file.c.orig`

## How to use

```bash
chmod +x find_copy
./find_copy
```

## Security features

- Blocks dangerous files (symlinks, huge files, weird characters)
- Asks before overwriting existing backups  
- Uses safe copying (no data corruption)
- Stops hackers from breaking your system
- Unsets all aliases, Loading library paths
- Uses secure paths for execution only 
- Sets file ownership
- Checks the ownership of file, read write permissions
- Checks file size and OS remained memory to avoid memory issues
- Checks file name lenght
- Checks the directory of file to be copied for being sure that it is current directory
- Checks the snapshot of file to prevent TOCTOU As much as possible

## Options when backups exist

- `y` = overwrite this file
- `n` = skip this file
- `a` = overwrite all files
- `n0` = skip all files

That's it! 🚀