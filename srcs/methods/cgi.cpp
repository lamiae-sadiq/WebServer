/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsadiq <lsadiq@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 03:01:02 by lsadiq            #+#    #+#             */
/*   Updated: 2024/03/07 03:08:18 by lsadiq           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../..//includes/cgi.hpp"
#include "../../includes/response.hpp"
#include "../../includes/Request.hpp"



void    Cgi::setUpEnv()
{
    // Set environment variables for CGI
    std::string cgiEnv = "CONTENT_LENGTH=" + std::to_string(request.getContentLength());
    putenv(const_cast<char*>(cgiEnv.c_str()));

    // Set other environment variables as needed
    // For example:
    // putenv("REQUEST_METHOD=POST");
    // putenv("CONTENT_TYPE=application/x-www-form-urlencoded");
    // putenv("QUERY_STRING=param1=value1&param2=value2");

    // Add more environment variables as required by your CGI script

    // Example: Set PATH environment variable
    std::string pathEnv = "PATH=/usr/local/bin:/usr/bin:/bin";
    putenv(const_cast<char*>(pathEnv.c_str()));
    
}

