/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   postMethod.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsadiq <lsadiq@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 16:27:53 by lsadiq            #+#    #+#             */
/*   Updated: 2024/02/21 21:47:00 by lsadiq           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/response.hpp"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

bool    response::checkUpload()
{
    if(getLocation().upload == "on")
        return true;
    else
        return false;
}

std::string	response::getUploadFN(){
    return uploadFileNmae;
}


bool passheader(char *con, size_t& index, size_t size)
{
    con[size] = '\0';
    if (strstr(con, "\r\n\r\n") != NULL)
    {
        index = strstr(con, "\r\n\r\n") - con + 4;
        return true;
    }
    return false;
}


void response::parsLength(char *con, size_t& index, size_t size)
{
    if (body_length < size - index)
        size = body_length + index;
    
    upfile.write(con + index, size - index);
    upfile.flush();
    body_length -= size - index;
    index = size;
    if (body_length == 0)
    {
        upfile.close();
        status_code = 201;
    }
}

void    response::parseChunk(char *con, size_t& index, size_t size)
{
    std::string chunkSizeStr;
    while (index < size)
    {
        if (con[index] == '\r' && con[index + 1] == '\n')
        {
            index += 2;
            break;
        }
        chunkSizeStr += con[index];
        index++;
    }
    int chunkSize = 0;
    std::istringstream iss(chunkSizeStr);
    iss >> std::hex >> chunkSize;
    if (chunkSize == 0)
    {
        upfile.close();
        status_code = 201;
        return;
    }
    if (size - index >= chunkSize)
    {
        upfile.write(con + index, chunkSize);
        upfile.flush();
        body_length -= chunkSize;
        index += chunkSize;
    }
    else
    {
        upfile.write(con + index, size - index);
        upfile.flush();
        body_length -= size - index;
        index = size;
    }
}

void    response::methodPost()
{
    if (flag == 0)
    {
        size_t index = 0;
        char con[1024];
        size_t size = read(fd, con, 1024);
        if (!passheader(con, index, size))
            return;
        // write(1, con + index, size - index);
        createFile();
        if (status_code == 403)
            return;
        if (uplod_type == "Chunk")
            parseChunk(con, index, size);
        else if (uplod_type == "length")
            parsLength(con, index, size);
        else
            status_code = 400;
        flag = 1;    
    }
    else if (flag == 1) {
        size_t index = 0;
        char con[1024];
        size_t size = read(fd, con, 1024);
        // write(1, con + index, size - index);
        // write(1, con, size);
        if (uplod_type == "Chunk")
            parseChunk(con, index, size);
        else if (uplod_type == "length")
            parsLength(con, index, size);
    }
    
    // if(!allowedMethods())
    //         status_code = 405;
    // if (!checkUpload())
    // {
    //     if (access(getUploadFN().c_str(), F_OK | R_OK) == 0)
    //     {
    //         // targetUri = loc.root + target_url.substr(loc.location_name.size());
    //         if(checkType(loc.upload) == FILE)
    //         {
    //             check_extention(loc.upload);
    //             if (extention == "php" || extention == "py")
    //                 std::cout << "its a cgi" << std::endl;
    //             else
    //             {
    //                 std::remove(uploadFileNmae.c_str());
    //                 status_code = 403;
    //                 return ;
    //             }
    //         }
    //         if (checkType(loc.upload) == DIRECTORY)
    //         {
    //             if (loc.upload[loc.upload.length() - 1] != '/')
    //             {
    //                 status_code = 301;
    //                 target_url.append("/");
    //                 std::remove(uploadFileNmae.c_str());
    //                 return;
    //             }
    //             if (!loc.index.empty())
    //             {
    //                 std::remove(uploadFileNmae.c_str());
    //                 status_code = 403;
    //                 return ;
    //             }
    //         }
    //         if (checkType(loc.upload) == NOT_FOUND)
    //         {
    //             std::remove(uploadFileNmae.c_str());
    //             status_code = 403;
    //             return ;
    //         }
    //     }
    //     else
    //         std::remove(getUploadFN().c_str());
    // }
    // else
    //     status_code = 404;
}

std::string response::newUpload()
{
    if (loc.upload[loc.upload.size() - 1] == '/' && targetUri[0] == '/'){
		pathUpload = loc.upload + targetUri.substr(1);
		// std::cout << "pathUpload : " << pathUpload << std::endl;
        return (pathUpload);
	}
    else if (loc.upload[loc.upload.size() - 1] != '/' && targetUri[0] != '/')
	{
		pathUpload = loc.upload + "/" + targetUri;
		// std::cout << "pathUpload :: " << pathUpload << std::endl;
		return (pathUpload);
	}
    else{
		pathUpload = loc.upload + target_url.substr(loc.location_name.size());
		// std::cout << "pathUpload ::: " << pathUpload << std::endl;
        return (pathUpload );
	}
}

std::string response::generateName()
{
    std::string name = "";
	srand((unsigned)time(0));
	for (int i = 0; i < 20; i++)
		name += 97 + rand() % 25;
	return (name);
}

void    response::createFile()
{
    if (loc.upload[0] == '/')
		loc.upload.erase(0, 1);
    if (loc.upload.length() > 1){
        std::string UplDir = loc.root + "/" + loc.upload;
        if(access(UplDir.c_str(), F_OK | W_OK) == -1)
        {
            perror("Upload Directory");
            status_code = 403;
        }
        std::string randName = generateName();
        uploadFileNmae = "text/html";
        check_extention(uploadFileNmae);
        //i need content type to create the file
        content_type = fileType;
        extention = "mp4";
        upfile.open((UplDir +"/" + randName + "." + extention).c_str());
        if (upfile.is_open() == false)
        {
            perror((UplDir + "/" + randName + "." + extention).c_str());
            status_code = 403;
        }
    }
}
