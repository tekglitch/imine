
namespace imine
{

class Engine;

struct Object
{
    int fd;
    std::function<void(int,uint8_t)> callback;
};





class EpollBaseObj
{

public:
    EpollBaseObj() {};
    ~EpollBaseObj() {};
    int fd,id;
    ///void(*callback)(std::shared_ptr<EpollBaseObj>, u_int);
    std::function<void()> callback;


};

class Web : public EpollBaseObj
{
public:
    Web() {};
    ~Web() {};
    int fd, id;
    ///void(*callback)(std::shared_ptr<Web>, u_int);
    std::function<void()> callback;
    std::string shit;
};





class Epoll
{
protected:
    Engine *im;
    int epoll;
    //struct epoll_event* evs;
    epoll_event* evs;
private:
    std::map<int, std::shared_ptr<Object>> objects;
public:
    int create_object(std::shared_ptr<imine::sockets::socket_set>,std::function<void(int,uint8_t)>);
    int add_fd(int);
    void check_events();

    Epoll(Engine*);
    ~Epoll();
};
} // namespace imine
