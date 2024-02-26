#include "./includes/Parse.hpp"
#include "./includes/Multiplixer.hpp"


int main(int ac, char**av)
{
  try
  {
    Multiplixer multiplixer;
    std::vector<Server> servers;
    if(ac == 2)
    {
      std::string configueFile = av[1];
      servers = Parser::paseConfigueFile(configueFile);
	    multiplixer.start(servers);
    }
    else
    throw "please enter the configue file\n";

  }
  catch(const std::exception& e)
  {
      std::cerr << e.what() << '\n';
  }
  catch(std::string msgError)
  {
        std::cerr << msgError << '\n';
  }
}