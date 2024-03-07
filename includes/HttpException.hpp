/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpException.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kel-baam <kel-baam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 21:13:14 by kel-baam          #+#    #+#             */
/*   Updated: 2024/03/05 21:13:23 by kel-baam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <iostream>
#include <string>

class HttpException:public  std::exception
{
    private:
        const char* msg;
        int statusCode;
    public:
        HttpException(const char* init,int code):msg(init),statusCode(code){};

        int getStatusCode()
        {
            return statusCode;
        }

        const char* what() const throw()
        {
                return msg;
        }
};

class HttpBadRequest:public HttpException
{
    public:
        HttpBadRequest(const char *msg):HttpException(msg,400){};
};

class HttpVersionNotSupported:public HttpException
{
    public:
        HttpVersionNotSupported(const char *msg):HttpException(msg,505){};      
};

class HttpNotImplemented:public HttpException
{
    public:
        HttpNotImplemented(const char *msg):HttpException(msg,501){};  
};
class HttpUriTooLong:public HttpException
{
    public:
        HttpUriTooLong(const char *msg):HttpException(msg,414){};
};
