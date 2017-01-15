#include "type.h"
#include "protocol/message.h"
#include "core/dns.h"

int main(int argc, char** argv)
{
    struct DNS dns =
    {
        //.init
        //.init_database
        .init_service       = socket_config,
        //.query
        .listen             = socket_recvfrom,
        .respond           = socket_sendto,
    };

    ///Socket File Descriptor
    int sk_fd;
    ssize_t nBytes;

    uchar rbuf[BUF_SIZE] = {0};
    uchar wbuf[BUF_SIZE] = {0};

    /**
     * Default Configuration
     */
    int sk_domain       = AF_INET;
    int sk_type         = SOCK_DGRAM;
    int sk_protocol     = 0;
    struct sockaddr_in serv_addr = {
        .sin_family         = AF_INET,
        .sin_addr.s_addr    = htonl(INADDR_ANY),
        .sin_port           = htons(SERV_PORT),
    };

    struct sockaddr clnt_addr = {0};
    socklen_t clnt_addr_len = sizeof(clnt_addr_len);



    ///TODO: Implement: Parser and Load Config File
    //dns.init();
    //dns.init_database();

    dlog("DNS initinalize Service\n");
    /**
     * Initialize the socket and bind it
     */
    sk_fd = dns.init_service(sk_domain, sk_type, sk_protocol, (struct sockaddr *) &serv_addr, (socklen_t) sizeof(serv_addr));
    dlog("Done!\n");

    while(1)
    {
        dlog("DNS listen\n");
        nBytes = dns.listen(sk_fd, rbuf, &clnt_addr, &clnt_addr_len);
        dlog("Done!\n");

        

        dlog("DNS respond\n");
        dns.respond(sk_fd, wbuf, nBytes, &clnt_addr, clnt_addr_len);
        dlog("Done!\n");
    }

    printf("DDNS Server shutdown\n");
    return 0;
}
