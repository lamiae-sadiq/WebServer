/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsadiq <lsadiq@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 16:25:03 by lsadiq            #+#    #+#             */
/*   Updated: 2024/03/02 16:00:31 by lsadiq           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/response.hpp"

void response::executeMethodes(const char *buff,size_t size,int fd)
{
    setFd(fd);
    if (status_code != 200 && flag == 0){
        // handel_error();
        std::cout << "check error flags" << status_code << "\n";
        getErrorPage();
        sendErrorPage();
        // std::cout << "Error page sent" << std::endl;
    }
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