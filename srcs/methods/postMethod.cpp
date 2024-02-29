/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   postMethod.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsadiq <lsadiq@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 16:27:53 by lsadiq            #+#    #+#             */
/*   Updated: 2024/02/29 16:34:08 by lsadiq           ###   ########.fr       */
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


bool passheader(const char *con, size_t& index, size_t size)
{
    (void)size;
    // con[size] = '\0';
    if (strstr(con, "\r\n\r\n") != NULL)
    {
        index = strstr(con, "\r\n\r\n") - con + 4;
        return true;
    }
    return false;
}


void response::parsLength(const char *con, size_t& index, size_t size)
{
    if (request.getContentLength() < size - index)
        size = request.getContentLength() + index;
    upfile.write(con + index, size - index);
    upfile.flush();
    request.contentLength -= size - index;

    index = size;
    if (request.contentLength == 0)
    {
        upfile.close();
        flag = 201;
        status_code = 201;
        flag = 0;
    }
}

void    response::parseChunk(const char *con, size_t& index, size_t size)
{
    while (index < size)
    {
        static int pflag = 0;
        
        if (pflag != flag)
            std::cout << "flag : " << flag << std::endl;
        pflag = flag;
        
        if (flag == 2)
        {
            while (index < size && ihex < 20)
            {
                if (con[index] == '\r')
                    break;
                chunkSizeStr += con[index];
                index++;
                ihex++;
            }
            if (ihex == 20)
            {
                upfile.close();
                status_code = 400;
                flag = 0;
                return;
            }
            if (con[index] != '\r')
            {
                return;
            }
            std::istringstream iss(chunkSizeStr);
            chunkSizeStr.clear();
            ihex = 0;
            if (!(iss >> std::hex >> chunkSize))
            {
                upfile.close();
                status_code = 400;
                flag = 0;
                return;
            }
            if (chunkSize == 0)
            {
                lastChunk = true;
            }
            flag = 3;
        }
        if (flag == 3)
        {
            if (index >= size)
                return;
            if (con[index] != '\r')
            {
                upfile.close();
                status_code = 400;
                flag = 0;
                return;
            }
            index++;
            flag = 4;
        }
        if (flag == 4)
        {
            if (index >= size)
                return;
            if (con[index] != '\n')
            {
                upfile.close();
                status_code = 400;
                flag = 0;
                return;
            }
            index++;
            flag = 5;
        }
        if (flag == 5)
        {
            if (size - index >= chunkSize)
            {
                upfile.write(con + index, chunkSize);
                upfile.flush();
                index += chunkSize;
                chunkSize = 0;
                flag = 6;
            }
            else
            {
                upfile.write(con + index, size - index);
                upfile.flush();
                chunkSize -= size - index;
                index = size;
            }
        }
        if (flag == 6)
        {
            if (index >= size)
                return;
            if (con[index] != '\r')
            {
                upfile.close();
                status_code = 400;
                flag = 0;
                return;
            }
            index++;
            flag = 7;
        }
        if (flag == 7)
        {
            if (index >= size)
                return;
            if (con[index] != '\n')
            {
                upfile.close();
                status_code = 400;
                flag = 0;
                return;
            }
            if (lastChunk)
            {
                upfile.close();
                status_code = 201;
                flag = 0;
                return;
            }
            index++;
            flag = 2;
        }
    }
    
}

void    response::methodPost(const char *con, size_t size)
{
    // if(!allowedMethods())
    //         status_code = 405;
    if (flag == 0)
    {
        size_t index = 0;
        createFile();
        if (status_code == 403)
            return;
        if (uplod_type == "Chunk")
        {
            flag = 2;
            parseChunk(con, index, size);
        }
        else if (uplod_type == "length")
        {
            flag = 1;
            parsLength(con, index, size);
        }
        else
            status_code = 400;
    }
    else if (flag == 1) {

        size_t index = 0;
        parsLength(con, index, size);
    }
    else if (flag >= 2 && flag <= 10) {
        size_t index = 0;
        parseChunk(con, index, size);
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
    if (request.location.upload[0] == '/')
		request.location.upload.erase(0, 1);
    if (request.location.upload.length() > 1){
        std::string UplDir = request.location.root + "/" + request.location.upload;
        std::cout << "ROOT "<< request.location.root << std::endl;

        std::cout <<"UPLOAD  " <<UplDir << std::endl;
        if(access(UplDir.c_str(), F_OK | W_OK) == -1)
        {
            perror("Upload Directory");
            status_code = 403;
        }
        std::string randName = generateName();
        uploadFileNmae = "vedio/mp4";
        check_extention(uploadFileNmae);
        //i need content type to create the file
        content_type = fileType;
        extention = "jpg";
        // std::cout << UplDir +"/" + randName + "." + extention << std::endl;
        upfile.open((UplDir+"/" + randName + "." + extention).c_str());
        if (upfile.is_open() == false) 
        {
            perror((UplDir + "/" + randName + "." + extention).c_str());
            status_code = 403;
        }
    }
}
