/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpException.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kel-baam <kel-baam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 21:13:14 by kel-baam          #+#    #+#             */
/*   Updated: 2024/03/16 14:47:13 by kel-baam         ###   ########.fr       */
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
        HttpException(const char* init,int code);
        HttpException(const char* init);
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
        HttpBadRequest(const char *msg);
};

class HttpVersionNotSupported:public HttpException
{
    public:
        HttpVersionNotSupported(const char *msg);     
};

class HttpNotImplemented:public HttpException
{
    public:
        HttpNotImplemented(const char *msg);
};
class HttpUriTooLong:public HttpException
{
    public:
        HttpUriTooLong(const char *msg);
};

class HttpLengthRequired:public HttpException
{
    public:
        HttpLengthRequired(const char *msg);
};
