/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsadiq <lsadiq@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 16:25:03 by lsadiq            #+#    #+#             */
/*   Updated: 2024/02/29 19:47:11 by lsadiq           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/response.hpp"

void response::executeMethodes(const char *buff,size_t size,int fd)
{
    if (status_code != 200 && flag == 0){
        // std::cout << status_code << std::endl;
        handel_error();
        return;
    }
    setFd(fd);
    if (request.getMethod() == "GET")
        methodGet();
    else if (request.getMethod() == "DELETE")
        Delete();
    else if (request.getMethod() == "POST")
    {
        if(request.getFirstReadBody())
        {
            methodPost(request.tmpBuff.c_str(),request.tmpBuff.size());
            request.setFirstReadOfBody(false);
        }
        else
            methodPost(buff, size);
    }
    else
        status_code = 405;
}