/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpException.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kel-baam <kel-baam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/16 21:32:54 by kel-baam          #+#    #+#             */
/*   Updated: 2024/03/19 12:40:08 by kel-baam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/HttpException.hpp"


int HttpException::getStatusCode()
{
    return statusCode;
}
const char* HttpException::what() const throw()
{
    return msg;
}

HttpException::HttpException(const char* init,int code):msg(init),statusCode(code){};
HttpException::HttpException(const char* init):msg(init){};
HttpBadRequest::HttpBadRequest(const char *msg):HttpException(msg,400){};
HttpVersionNotSupported::HttpVersionNotSupported(const char *msg):HttpException(msg,505){};
HttpNotImplemented::HttpNotImplemented(const char *msg):HttpException(msg,501){};
HttpUriTooLong::HttpUriTooLong(const char *msg):HttpException(msg,414){};
HttpLengthRequired::HttpLengthRequired(const char *msg):HttpException(msg,411){};
HttpForbidden::HttpForbidden(const char *msg):HttpException(msg,403){};
