#include "debug.h"
#include "utility/file_ops.h"
#include "limit.h"
#include "config.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define Usage "./parser_test <config_directory>\n"

int main(int argc, char **argv)
{
    if(argc != 2)   elog("%s\n",Usage);
    
    //int cfg_fd, srv_fd;

    syserr(!isdir(argv[1]), "Dir doesn't exist\n");
    
    char *dir = argv[1];

    //char f[2][FNAME_LIMIT] = { STARTUP_FILE, ROOT_SERVER_FILE};
    /**
     * FIXME: We should distinguish `char **p`, `char *p[255]``char p[2][255])`.
     *  `char **p`: allocate a space to store a pointer pointed to `char *`
     *  `char *p[255]`: allocate an array whose element store a pointer pointed to `char *`
     *  **`char p[2][255]`**: allocate a 2*255 char space. It is JUST LIKE AN ONE DIMENSION ARRAY NOT TWO!!!
     *  => so they are not the same!
     *
     *  char p[2][255];
     *  p[0] != ((char **) p)[0]
     */
    /*char fname[2][FNAME_LIMIT] = { STARTUP_FILE, ROOT_SERVER_FILE};
    char path[2][PATH_LIMIT];
    apen_dir_file(dir, fname, 2, path);*/
    //
    char* fname[2] = { STARTUP_FILE, ROOT_SERVER_FILE};
    char* path[2];

    for(int i = 0; i < 2; i++) path[i] = (char *) malloc( PATH_LIMIT * sizeof(char));
    //char **path = (char **) malloc( 2 * sizeof(char *));
    //path[0] = (char *) malloc( PATH_LIMIT * sizeof(char));
    //path[1] = (char *) malloc( PATH_LIMIT * sizeof(char));
    apen_dir_file(dir, fname, 2, path);

    //strcpy(path[0], dir);
    //strcat(path[0],"/");
    //strcat(path[0],fname[0]);

    //dlog("%s\n", path[0]);

    syserr(!fexist(path[0]), "startup doesn't exist\n");
//    show_startup_cfg()
    
    syserr(!fexist(path[1]), "root server doesn't exist\n");
//    show_root_srv();


//    show_zone_rr();
}
