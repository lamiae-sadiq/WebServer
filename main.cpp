/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kel-baam <kel-baam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/20 22:44:06 by kel-baam          #+#    #+#             */
/*   Updated: 2024/03/21 17:21:51 by kel-baam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./includes/Parse.hpp"
#include "./includes/Multiplixer.hpp"


int main(int ac, char**av)
{
  try
  {
    Multiplixer multiplixer;
    std::vector<Server> servers;
    std::string configueFile;

    if(ac == 2 || ac == 1)
    {
      configueFile ="./srcs/configuefile/cgfile.conf";
      if(ac == 2)
        configueFile = av[1];
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