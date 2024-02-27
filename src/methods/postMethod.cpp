/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   postMethod.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsadiq <lsadiq@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 16:27:53 by lsadiq            #+#    #+#             */
/*   Updated: 2024/02/24 15:19:00 by lsadiq           ###   ########.fr       */
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
    if (request.getContentLength() < size - index)
        size = request.getContentLength() + index;
    
    upfile.write(con + index, size - index);
    upfile.flush();
    request.contentLength -= size - index;
    index = size;
    if (request.contentLength == 0)
    {
        upfile.close();
        status_code = 201;
    }
}

void    response::parseChunk(char *con, size_t& index, size_t size)
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
                return;
            }
            if (chunkSize == 0)
            {
                upfile.close();
                status_code = 201;
                return;
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
                return;
            }
            index++;
            flag = 2;
        }
    }
    
}

void    response::methodPost(std::string buff)
{
    (void) buff;
    if (flag == 0)
    {
        size_t index = 0;
        // std::cout << index << "|||\n";

        // tmpBuff + buff 
        char con[1024];
        // con = buff
        size_t size = read(fd, con, 1024);
        if (!passheader(con, index, size))
            return;
        // write(1, con + index, size - index);
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
        // std::cout << index << "|||ccc\n";
        char con[1024];
        size_t size = read(fd, con, 1024);
        
        // write(1, con + index, size - index);
        // write(1, con, size);
        parsLength(con, index, size);
    }
    else if (flag >= 2 && flag <= 10) {
        size_t index = 0;
        char con[1024];
        size_t size = read(fd, con, 1024);
        // write(1, con + index, size - index);
        // write(1, con, size);
        parseChunk(con, index, size);
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
        // std::cout << UplDir +"/" + randName + "." + extention << std::endl;
        upfile.open((UplDir+"/" + randName + "." + extention).c_str());
        if (upfile.is_open() == false) 
        {
            perror((UplDir + "/" + randName + "." + extention).c_str());
            status_code = 403;
        }
    }
}
