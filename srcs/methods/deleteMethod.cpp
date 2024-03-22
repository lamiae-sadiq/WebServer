/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   deleteMethod.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kel-baam <kel-baam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 09:36:00 by lsadiq            #+#    #+#             */
/*   Updated: 2024/03/22 02:33:28 by kel-baam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/response.hpp"
#include "../../includes/cgi.hpp"

void	response::deleteDir(std::string uri)
{
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
				deleteDir(path);
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
	targetUri = request.getRealPath();
	// std::cout << targetUri <<"|\n";
	if(!allowedMethods())
            status_code = 405;
	else if (access(this->targetUri.c_str(), F_OK) == 0)
	{
		if (access(this->targetUri.c_str(), W_OK) == 0)
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
				if(targetUri[targetUri.length() - 1] == '/')
					deleteDir(targetUri);
				else
					status_code = 409;
			}
		}
		else 
			status_code = 403;
	}
	else
		status_code = 404;
}