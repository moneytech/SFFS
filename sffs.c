#define FUSE_USE_VERSION 30

#include <fuse.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>

static int sffs_getattr(const char *path, struct stat *st) {
    printf("getattribute called \n");
    printf("Requested %s \n", path);

    st->st_uid = getuid();
    st->st_gid = getgid();
    st->st_atime = time(NULL);
    st->st_mtime = time(NULL);

    if(strcmp(path, "/") == 0) {
        st->st_mode = S_IFDIR | 0755;
        st->st_nlink = 2;
    } else {
        st->st_mode = S_IFREG | 0644;
        st->st_nlink = 1;
        st->st_size = 1024;
    }
    return 0;
}

static int sffs_readdir(const char *path, void *buffer, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi) {
    printf("Getting files of %s", path);
    filler( buffer, ".", NULL, 0 );
    filler( buffer, "..", NULL, 0 );

    if ( strcmp( path, "/" ) == 0 ) {
        filler( buffer, "First_File", NULL, 0 );
        filler( buffer, "Second_File", NULL, 0 );
    }
    return 0;
}

static int sffs_read(const char *path, char *buffer, size_t size, off_t offset, struct fuse_file_info *fi) {
    printf( "--> Trying to read %s, %li, %zu\n", path, offset, size);

    char file54Text[] = "Hello World From First File!";
    char file349Text[] = "Hello World From Second File!";
    char *selectedText = NULL;

    if ( strcmp( path, "/First_File" ) == 0 )
        selectedText = file54Text;
    else if ( strcmp( path, "/Second_File" ) == 0 )
        selectedText = file349Text;
    else
        return -1;

    memcpy( buffer, selectedText + offset, size );

    return (int) (strlen(selectedText ) - offset);
}

static struct fuse_operations operations = {
    .getattr = sffs_getattr,
    .readdir = sffs_readdir,
    .read = sffs_read,
};

int main(int argc, char *argv[]) {
    return fuse_main(argc, argv, &operations, NULL);
}