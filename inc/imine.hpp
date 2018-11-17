#ifndef __IMINE__HPP__
#define __IMINE__HPP__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <errno.h>
#include <iostream>
#include <time.h>
#include <sys/time.h>
#include <vector>
#include <openssl/conf.h>
#include <unordered_map>
#include <sstream>
#include <openssl/bio.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <signal.h>
#include <regex>
#include <iomanip>
#include <fstream>
#include <exception>
#include <stdexcept>

#define MAX_EPOLL_EVENTS 1024
#define MAX_EPOLL_OBJECTS 1024

#define SSL_DEFAULT_KEYFILE "imine-cl.key"
#define SSL_DEFAULT_PEMFILE "imine-cl.pem"

#include "config.hpp"
#include "sockets.hpp"

#include "epoll.hpp"
namespace imine
{
    void daemonize();
    void read_config();
    void _error(std::string, int eno=0);

    class Client
    {
    public:
        std::shared_ptr<imine::sockets::socket_set> ss;

    };

    class Server
    {
    public:
        std::shared_ptr<imine::sockets::socket_set> ss;

    };

    class Engine
    {

    public:
        void init();
        void run();
///        std::shared_ptr<Client> create_client(std::string,std::function<void(int,uint8_t)>,int=0);
        std::shared_ptr<Client> create_client(std::string,int16_t,std::function<void(int,uint8_t)>,int=0);
        std::shared_ptr<Server> create_server(int16_t,std::function<void(int,uint8_t)>,std::function<void(int,uint8_t)>,int=0);
        std::shared_ptr<Epoll> get_epoll() { return ep; }

        Engine();
        ~Engine();
    protected:
        std::vector<std::shared_ptr<Server>> servers;
        bool running { 1 };
        std::shared_ptr<Epoll> ep { std::make_shared<Epoll>(this) };
    };




} /// namespace imine
#endif /// __IMINE__HPP__
