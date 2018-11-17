#include "imine.hpp"


namespace imine
{

void _error(std::string msg, int eno)
{
    throw std::runtime_error(msg + ":" + std::to_string(eno));
}

void daemonize()
{
    int pid = fork();
    switch (pid) {
        case -1:
        {
            std::cerr << "error forking to background. exiting\n";
            perror("fork");
            exit(-1);
        }
        break;
        case 0:
        {
            freopen("imine.log", "w", stdout);
            freopen("error.log", "w", stderr);
            //fclose(stdout);
            //fclose(stderr);
        }
        break;
        default:
        {
            std::cout << "Running in the background... (" << pid << ")\n";
            exit(0);
        }
        break;
    }
}


void read_config()
{
    /// read config file
    std::ifstream conf("imine-example.conf");
    if(!conf)
    {
        imine::_error("unable to open config file.",errno);
    }
    std::string line;
    int line_num = 0;
    while(std::getline(conf,line))
    {
        ++line_num;
        if(line.size()<1)
            continue;
        auto i = 0;
        while(line[i]==' ' || line[i]=='\t')
            ++i;

        line = line.substr(i);
        if(line.size()<1)
            continue;
        if(line[0]=='#') /// comment
            continue;

        if(line.size() <= 4)
        {
            std::string msg = "Malformed config parameter: " + line + ". " + std::to_string(line_num);
            imine::_error(msg,errno);
        }
        if(line[0]=='S') /// server
        {

        }
        else if(line[0]=='C') /// client(outgoing) connection
        {

        }
        else if(line[0]=='P') /// config parameter
        {

        }
        else if(line[0]=='W') /// web
        {
        std::cout << "in W" << std::endl;
            if(line[1] == 'S') /// web server
            {

            }
            else if(line[1] == 'C') /// web client
            {

            }
            else
            {
                std::cout << "Malformed config parameter: " << line << ". " << line_num << std::endl;
            }
        }
    }
    conf.close();

}

Engine::Engine()
{

}

Engine::~Engine()
{
    imine::sockets::destroy();
}



std::shared_ptr<Client> Engine::create_client(std::string rAddr,int16_t rPort,std::function<void(int,uint8_t)> cb,int useSSL)
{
    auto cl = std::make_shared<Client>();
    cl->ss = imine::sockets::create_client("",443,1);
    if(cl->ss==NULL)
    {

    }
    ep->create_object(cl->ss,cb);
    ep->add_fd(cl->ss->fd);

    return cl;

}
std::shared_ptr<Server> Engine::create_server(int16_t lPort,std::function<void(int,uint8_t)> scb, std::function<void(int,uint8_t)> cb, int useSSL)
{
    auto sv = std::make_shared<Server>();
    sv->ss = imine::sockets::create_server("127.0.0.1",lPort,1);
    if(sv->ss==NULL)
    {

    }
    ep->create_object(sv->ss,cb);
    ep->add_fd(sv->ss->fd);
    servers.push_back(sv);
    return sv;
}
void Engine::run()
{
    while(running)
    {
        //ep->check_events();
    }
}
}
