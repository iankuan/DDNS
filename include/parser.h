#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "debug.h"
#include "utility/file_ops.h"
#include "limit.h"
#include "config.h"

#define MAX_BUFF_SIZE 200

typedef enum {
    STARTUP_CFG,
    ROOT_SERVERS,
    ZONE,
} file_t;

struct startup {
    char *root_server_path;
    
    int      z_count;
    char **zone_name;
    char **zone_path;
};

///#define parser(type, file) parser_##type(file)

struct startup *startup_parser(char* in);
