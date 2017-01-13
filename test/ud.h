#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>

#include "debug.h"

#define BUF_SIZE 10

#define PORT_NUM 8008 ///Server Port
