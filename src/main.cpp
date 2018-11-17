#include "imine.hpp"







void test(int fd, uint8_t type)
{
    std::cout << "word^" << std::endl;
}

void cltest(int fd, uint8_t type)
{

}


int main(int argc, const char *argv[])
{
    try
    {
    std::unique_ptr<imine::Engine> im = std::make_unique<imine::Engine>();
    /// parse commandline arguments

    imine::read_config();



    if(argc>0)
    {

    }

    im->create_server(5555,test,cltest);
    im->run();
    }
    catch(const std::runtime_error &e)
    {
        std::cerr << e.what() << std::endl;
    }
    catch(const std::exception &e)
    {

    }
    catch(...)
    {
        std::cerr << "Caught unknown exception, exiting.";
    }
    return 0;
}
