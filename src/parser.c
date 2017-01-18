#include "parser.h"
/**
 * startup.cfg
 */

/*struct startup_info *startup_parser(char *path)
{
    FILE* fp = fopen(path, "r");

    char *rbuf[BUF_SIZE];

    while(!fgets(rbuf, 
}*/

struct startup *startup_parser(char* in)
{
    FILE *fd = fopen(in, "r");
    //syserr(fd != -1, "fopen fail\n");   

    char rbuf[MAX_BUFF_SIZE];
    char info[4][100];

    struct startup *ret = (struct startup *) malloc(sizeof(struct startup));
    ret->zone_name = (char **) malloc(sizeof(char *) * 50);
    ret->zone_path = (char **) malloc(sizeof(char *) * 50);
    ret->z_count = 0;

    int i = 0;
    char *dname = dirname(in);
    while(fgets(rbuf, sizeof(rbuf), fd)) {
        if(rbuf[0] != '\n' && rbuf[0] != ';')
            //puts(rbuf);
        {
            sscanf(rbuf, "%s%s%s%s", info[0], info[1], info[2], info[3]);
            dlog("%s %s %s %s\n", info[0], info[1], info[2], info[3]);

            if(!strcmp(info[0], "load"))
            {
                if(!strcmp(info[1], "root") && !strcmp(info[2], "server"))
                {
                    ret->root_server_path = strndup(dname, PATH_LIMIT);
                    strcat(ret->root_server_path, "/");
                    strcat(ret->root_server_path, info[3]);
                }

                if(!strcmp(info[1], "zone"))
                {
                    ret->zone_name[i] = strdup(info[2]);
                    ret->zone_path[i] = strndup(dname, PATH_LIMIT);
                    strcat(ret->zone_path[i], "/");
                    strcat(ret->zone_path[i], strdup(info[3]));
                }
            }
        }
    }
    ret->z_count = i;

    return ret;
}
