#include "imine.hpp"

namespace imine
{


Epoll::Epoll(Engine *i) : im(i)
{
    if((epoll = epoll_create1(0)) == -1)
    {
        std::cerr << "error creating epoll\n";
    }
    evs = new struct epoll_event[MAX_EPOLL_EVENTS];

}

Epoll::~Epoll()
{
    close(epoll);
    delete[] evs;
}

int Epoll::create_object(std::shared_ptr<imine::sockets::socket_set> ss,std::function<void(int,uint8_t)> cb)
{
    if(objects[ss->fd]!=NULL)
        return 0;
    auto obj = std::make_shared<Object>();
    obj->fd = ss->fd;
    objects[ss->fd] = obj;
    return 1;
}

int Epoll::add_fd(int fd)
{
    if (fd == -1)
        return -1;

    struct epoll_event ev;
    memset(&ev,0,sizeof(ev));
    ev.events = EPOLLIN; // temp
    ev.data.fd = fd;
    switch (epoll_ctl(epoll, EPOLL_CTL_ADD, fd, &ev))
    {
        case -1:
            ///int e = errno;
            std::cerr << "Error adding " <<fd<< " To epoll event handler: " << errno << std::endl;
            return -1;
        break;
    }
    return 1;
}

void Epoll::check_events()
{
    int i = epoll_wait(epoll, evs, MAX_EPOLL_EVENTS, 150);

    for(auto x = 0; x < i; ++x)
    {
        auto fd = evs[i].data.fd;
        if(objects[fd] != NULL && objects[fd]->fd == fd)
            if(evs[i].events & EPOLLIN)
                objects[fd]->callback(fd,1); //read
            else if(evs[i].events & EPOLLOUT)
                objects[fd]->callback(fd,2);
/**
        for(auto &it: objects)
            if(fd==it->fd)
                if(evs[i].events & EPOLLIN)
                    it->callback(); //read
                else if(evs[i].events & EPOLLOUT)
                    it->callback(); // write
**/
    }

}
}
