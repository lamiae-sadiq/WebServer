/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsadiq <lsadiq@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 09:40:22 by lsadiq            #+#    #+#             */
/*   Updated: 2024/03/10 16:58:29 by lsadiq           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Request.hpp"

#define CGI 0

class Request;
// class response;
class Cgi
{
    private :
        std::map<std::string, std::string>		cgiHeader;
		char** 									env;
        // response							*_response;
        int									_fd;
    public :
        Request									grequest;
        Cgi();
        Cgi(Request grequest);
        ~Cgi();
        void		setEnv();
        void		executePHP(std::string &file);
        void		executePython();
        char**		getEnv();
};
