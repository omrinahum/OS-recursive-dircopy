#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <fcntl.h>  
#include <unistd.h>
#include <ctype.h> 
#include <dirent.h>   
#include <sys/stat.h>  
#include <sys/types.h>
#include <errno.h>     
#include <limits.h>    

#define MAX_SIZE 1024

void create_hard_link(const char *src, const char *dst);
void copy_symlink(const char *src, const char *dst);
void copy_directory(const char *src, const char *dst);


int main (int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <source_directory> <backup_directory>\n", argv[0]);
        return 1;
    }

    char* source_directory = argv[1];
    char* backup_directory = argv[2];

    printf("\n=== DIRECTORY BACKUP ===\n\n");
    printf("Source: %s\n", source_directory);
    printf("Backup: %s\n\n", backup_directory);

    struct stat src_info;

    // Check if source directory exists and is a directory
    if (stat(source_directory, &src_info) != 0 || !S_ISDIR(src_info.st_mode)) {
        // No such file or directory
        errno = ENOENT; 
        perror("src dir");
        return 1;
    }

    struct stat dst_info;
    // Check if backup directory already exists
    if (stat(backup_directory, &dst_info) == 0) {
        errno = EEXIST;
        perror("backup dir");
        return 1;
    }

    printf("Starting backup...\n\n");
    copy_directory(source_directory, backup_directory);
    printf("\n=== BACKUP COMPLETE ===\n\n");
    return 0;
}


void copy_directory(const char *src, const char *dst){
    struct stat src_info;
    if (lstat(src, &src_info) == -1) {
        perror("stat failed");
        return;
    }

    // Create destination directory with same permissions
    if (mkdir(dst, src_info.st_mode & 0777) != 0) { 
    perror("mkdir failed");
    exit(1);
    }
    
    printf("[DIR] Created: %s\n", dst);

     // Open source directory
     DIR *dir = opendir(src);
     if (dir == NULL) {
         perror("Failed to open directory");
         exit(1);
     }


    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        // Skip . and .. 
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        // Build full paths
        char src_path[MAX_SIZE];
        snprintf(src_path, sizeof(src_path), "%s/%s", src, entry->d_name);

        char dst_path[MAX_SIZE];
        snprintf(dst_path, sizeof(dst_path), "%s/%s", dst, entry->d_name);

        // Get file info
        struct stat st;
        if (lstat(src_path, &st) == -1) {
            perror("stat failed");
            continue;
        }

        // Handle based on file type
        if (S_ISDIR(st.st_mode)) {
            copy_directory(src_path, dst_path);
        } 
        else if (S_ISLNK(st.st_mode)) {
            copy_symlink(src_path, dst_path);
        }
        else if (S_ISREG(st.st_mode)) {
            create_hard_link(src_path, dst_path);
        }
    }

    closedir(dir);
}


void copy_symlink(const char *src, const char *dst){
    char target_link [MAX_SIZE];

    // Read the target of the symlink
    ssize_t len = readlink(src, target_link, sizeof(target_link) - 1);
    if (len == -1) {
        perror("readlink failed");
        return;
    }

    target_link[len] = '\0';

    // Create the symlink in the destination directory
    if (symlink(target_link, dst) != 0) {
        perror("symlink failed");
        exit(1);
    }
    
    printf("[LINK] Created symlink: %s -> %s\n", dst, target_link);
}


void create_hard_link (const char *src, const char *dst){
    // Create a hard link in the destination directory
    if (link(src, dst) != 0) {
        perror("link failed");
        exit(1);
    }
    
    printf("[FILE] Linked: %s\n", dst);
}





