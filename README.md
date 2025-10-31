# Directory Backup Utility

## Topic
**Directory Traversal and File System Operations**

## Description
A directory backup program that creates a complete copy of a directory structure using hard links. Preserves file structure, permissions, and symbolic links while saving disk space.

**How it works:**
- Recursively traverses source directory
- Creates hard links for files (saves space)
- Preserves symbolic links
- Maintains directory structure and permissions

## Key Concepts
- **Directory Traversal**: `opendir()`, `readdir()`, `closedir()`
- **Hard Links**: `link()` - Multiple names for same file data
- **Symbolic Links**: `symlink()`, `readlink()` - Link preservation
- **File Metadata**: `stat()`, `lstat()` - Get file information
- **Directory Creation**: `mkdir()` with permission preservation

## How It Works

1. **Validation**:
   - Check source exists
   - Ensure backup doesn't exist

2. **Recursive Copy**:
   - Create directories with same permissions
   - For each entry:
     - **Directory** → Recurse into it
     - **Regular File** → Create hard link
     - **Symlink** → Recreate symlink

3. **Hard Links**:
   - Shares file data (saves disk space)
   - Both names point to same inode
   - No data duplication!

## How to Run

### Compile
```bash
gcc -o backup backup.c
```

### Run
```bash
./backup source_directory backup_directory
```

**Requirements:**
- `source_directory` must exist
- `backup_directory` must NOT exist (prevents overwriting)

### Test
```bash
bash tester.sh
```

## Example Run

### Command
```bash
./backup test_source test_backup
```

### Output
```
=== DIRECTORY BACKUP ===

Source: test_source
Backup: test_backup

Starting backup...

[DIR] Created: test_backup
[FILE] Linked: test_backup/file1.txt
[FILE] Linked: test_backup/file2.txt
[FILE] Linked: test_backup/executable.sh
[DIR] Created: test_backup/emptydir
[DIR] Created: test_backup/subdir1
[FILE] Linked: test_backup/subdir1/file3.txt
[DIR] Created: test_backup/subdir1/subdir2

=== BACKUP COMPLETE ===
```

### Result Structure
```
test_backup/
├── file1.txt       (hard link to test_source/file1.txt)
├── file2.txt       (hard link)
├── executable.sh   (hard link)
├── emptydir/       (empty directory)
└── subdir1/
    ├── file3.txt   (hard link)
    └── subdir2/    (empty directory)
```

## Why Hard Links?

- **Regular Copy**: Duplicates all file data (wastes space)
- **Hard Link**: Only new directory entry, same file data (efficient)
- Both files synchronized (same inode)

## Files
- `backup.c` - Main backup program
- `test_source/` - Example source directory
- `test_backup/` - Backup directory (created by program)
- `tester.sh` - Automated test script

## System Calls
- `opendir()`, `readdir()`, `closedir()` - Directory operations
- `mkdir()` - Create directories
- `stat()`, `lstat()` - Get file metadata
- `link()` - Create hard links
- `symlink()`, `readlink()` - Handle symbolic links
