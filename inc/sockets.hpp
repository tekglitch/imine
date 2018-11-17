
#ifndef __IMINE__SOCKETS__HPP__
#define __IMINE__SOCKETS__HPP__


namespace imine
{
namespace sockets
{

    struct socket_set
    {
        int fd;
        uint8_t useSSL = {0}; /// 0 = no, 1 = standard,2 = BIO ssl
        SSL_CTX* ctx;
        SSL* ssl;
        BIO* bio;
        bool outgoing; /// are we an outgoing connection?
        std::string keycert,pemcert,data;

        socket_set(std::string k,std::string p) : keycert(k), pemcert(p),data("") {};
        socket_set() : keycert(SSL_DEFAULT_KEYFILE), pemcert(SSL_DEFAULT_PEMFILE),data("") {};
    };
    void destroy();
    int ssl_init(std::shared_ptr<socket_set>);
    std::shared_ptr<socket_set> create_client(std::string, uint16_t,std::string,int);
    std::shared_ptr<socket_set> create_server(std::string, uint16_t,int);
    std::shared_ptr<socket_set> create_client(std::string, uint16_t,int);

}


}



#endif
