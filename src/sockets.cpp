#include "imine.hpp"


namespace imine
{
namespace sockets
{

    void init()
    {
        SSL_library_init();
        OpenSSL_add_all_algorithms();
        SSL_load_error_strings();
        ERR_load_crypto_strings();
        ERR_load_BIO_strings();
    }

    void destroy()
    {
        ERR_free_strings();
        EVP_cleanup();
        ERR_clear_error(); // do we have data in memory?
        ///BIO_free_all();
        /// call SSL_free() on ALL of yoursocket_set elements that are ssl;
    }

    int ssl_init(const std::shared_ptr<socket_set> ss)
    {
        if(ss==NULL)
            return -1;

        if(ss->useSSL==1)
            ss->bio = NULL;
        ss->ctx = SSL_CTX_new( SSLv23_method());
        SSL_CTX_set_options(ss->ctx, SSL_OP_SINGLE_DH_USE);

        int use_cert = SSL_CTX_use_certificate_file(ss->ctx, ss->pemcert.c_str() , SSL_FILETYPE_PEM);
        int use_prv = SSL_CTX_use_PrivateKey_file(ss->ctx,ss->keycert.c_str(), SSL_FILETYPE_PEM);
        ss->ssl = SSL_new(ss->ctx);
        if (ss->ssl==NULL)
        {
            std::cerr << "failed to create SSL." << std::endl;
            SSL_CTX_free(ss->ctx);
            SSL_shutdown(ss->ssl);
            SSL_free(ss->ssl);
            ERR_clear_error();
            return -1;
        }
        SSL_set_fd(ss->ssl,ss->fd);
        if(ss->outgoing)
            if(SSL_connect(ss->ssl) == 100)
            {
                std::cerr << "failed to create SSL connection." << std::endl;

                SSL_CTX_free(ss->ctx);
                SSL_shutdown(ss->ssl);
                SSL_free(ss->ssl);
                ERR_clear_error();
                return -1;
            }
        if (!SSL_CTX_check_private_key(ss->ctx))
        {
            fprintf(stderr, "Private key does not match\n");
            abort();
        }
        SSL_CTX_set_verify(ss->ctx, SSL_VERIFY_FAIL_IF_NO_PEER_CERT | SSL_VERIFY_PEER, NULL);
        SSL_CTX_set_verify_depth(ss->ctx, 4);
        return 0;
    }
    std::shared_ptr<socket_set>
    create_client(std::string rAddr, uint16_t rPort,std::string lAddr,int useSSL)
    {

        return NULL;
    }

    std::shared_ptr<socket_set>
    create_client(std::string rAddr, uint16_t rPort,int useSSL)
    {
        auto ss = std::make_shared<socket_set>();
        ss->useSSL = useSSL;
        if(useSSL==2)
        {
            std::cerr << "ssl BIO is not currently implimented" << std::endl;
            return NULL;
        }
        auto fd = -1;
        if ((fd = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1)
        {
            if(useSSL>0)
            {
                ERR_print_errors(ss->bio);
                ERR_clear_error(); // just incase the buffer contains any remaining data for some odd reason.
            }
            throw std::runtime_error("");
        }
        struct sockaddr_in saddr;
        struct hostent     *hostent_p;
        if (!(hostent_p = ::gethostbyname(rAddr.c_str())))
        {
            std::cerr << "Error resolving remote hostname." << std::endl;
            return NULL; // temp
        }
        //set_nonblock(s,1);

        memset(&saddr, 0, sizeof(saddr));
        saddr.sin_family = AF_INET;
        saddr.sin_addr.s_addr = *(long *) hostent_p->h_addr;
        saddr.sin_port = htons(rPort);
        if (::connect(ss->fd, (struct sockaddr *)&saddr, sizeof(saddr)) < 0)
        {
            std::cerr << "Error connecting to remote host" << std::endl;
            return NULL;
        }
        if(useSSL==1)
            ssl_init(ss); //

        return ss;
    }
    std::shared_ptr<socket_set>
    create_server(std::string lAddr, uint16_t lPort, int useSSL)
    {
        auto ss = std::make_shared<socket_set>();
        ss->useSSL = useSSL;
        if(useSSL==2)
        {
            std::cerr << "ssl BIO is not currently implimented" << std::endl;
            return NULL;
        }
        auto fd = -1;
        if ((fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1)
        {
            std::cerr << "Error creating socket" << std::endl;
            return NULL;
        }
        struct sockaddr_in saddr;
        struct hostent *h_p;
        if (!(h_p = gethostbyname(lAddr.c_str())))
        {
           std::cerr << "Error resolving ip addr" << std::endl;
            return NULL;
        }
        ///set_nonblock(s,1);
        memset(&saddr, 0, sizeof(saddr));
        saddr.sin_family = AF_INET;
        saddr.sin_addr.s_addr = *(long *) h_p->h_addr;
        saddr.sin_port = htons(lPort);
        if (::bind(fd, (struct sockaddr*)(&saddr), sizeof(saddr)) < 0)
        {
            std::cerr << "Error binding to ip addr" << std::endl;
            return NULL;
        }
        if(::listen(fd, 10) == -1)
        {
            std::cerr << "Error listening on port:" << lPort << std::endl;
            return NULL;
        }
        ss->fd = fd;
        if(useSSL==1)
            ssl_init(ss);


        return ss;
    }
    int _read(std::unique_ptr<socket_set> ss)
    {

        char buffer[512];
        memset(&buffer,0,512);
        int len;
        if (ss->useSSL==1)
         {
                len = SSL_read(ss->ssl,buffer,512);
        } else
            len = ::recv(ss->fd,buffer,512,0);

        if (len > 0) {
            buffer[len] = '\0';
            ss->data.append(buffer);


        }
        return len;
    }
    int _write(std::unique_ptr<socket_set> ss,std::string data)
    {
        auto len = -1;
        if(ss->useSSL == 1)
            len = SSL_write(ss->ssl,data.c_str(),data.size());
        else
            len = ::send(ss->fd,data.c_str(),data.size(),0);

        return len;
    }
} /// namespace socket


} /// namespace imine
