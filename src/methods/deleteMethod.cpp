/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   deleteMethod.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsadiq <lsadiq@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 09:36:00 by lsadiq            #+#    #+#             */
/*   Updated: 2024/02/24 17:48:17 by lsadiq           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/response.hpp"
#include "../../includes/cgi.hpp"

void	response::Delete()
{
	if (!access(this->targetUri.c_str(), F_OK | R_OK))
	{
		if (checkType(targetUri) == FILE)
		{
			std::remove(targetUri.c_str());
			status_code = 204;
		}
		if(checkType(targetUri) == DIRECTORY)
		{
			if(targetUri[targetUri.length() - 1] == '/')
			{
				//if(location has CGI)
				if(CGI){
					//check if dir has index files
					//if yes run cgi on requested file with delete method
					//return code depend on cgi
				}
				else{
					std::remove(targetUri.c_str());
					status_code = 204;
				}
			}
			else
				status_code = 409;
		}
		if (checkType(targetUri) == NOT_FOUND)
		{
			std::cout << " not found "<< std::endl;
			status_code = 403;
		}
		else
			status_code = 403;
	}
}