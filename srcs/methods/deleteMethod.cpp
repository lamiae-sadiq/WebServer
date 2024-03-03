/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   deleteMethod.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsadiq <lsadiq@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 09:36:00 by lsadiq            #+#    #+#             */
/*   Updated: 2024/02/29 19:44:43 by lsadiq           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/response.hpp"
#include "../../includes/cgi.hpp"

void	response::deleteDir(std::string uri)
{
	// std::cout << "dir2" << std::endl;	

	DIR *dir;
	struct dirent *entry;

	dir = opendir(uri.c_str());
	if(dir == NULL)
	{
		status_code = 403;
		return ;
	}
	while((entry = readdir(dir)) != NULL)
	{
		if (entry->d_type == DT_DIR)
		{
			std::string path = uri + "/" + entry->d_name;
			if (entry->d_name[0] != '.')
			{
				// std::cout << path << std::endl;
				deleteDir(path);
			}
		}
		else
		{
			std::string path = uri + "/" + entry->d_name;
			std::remove(path.c_str());
		}
	}
	closedir(dir);
	remove(uri.c_str());
	status_code = 204;
}


void	response::Delete()
{
	std::cout << "targetUri   " << targetUri << std::endl;
	if (access(this->targetUri.c_str(), F_OK) == 0)
	{
		if (checkType(targetUri) == FILE)
		{
			if(std::remove(targetUri.c_str()) < 0)
				status_code = 403;
			else
			status_code = 204;
		}
		if(checkType(targetUri) == DIRECTORY)
		{	
			if(targetUri[targetUri.length() - 1] != '/'){
				deleteDir(targetUri);
			}
			else
				status_code = 409;
		}
	}
	else
		status_code = 404;
}