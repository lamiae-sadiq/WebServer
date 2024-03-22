/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   postMethod.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsadiq <lsadiq@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 16:27:53 by lsadiq            #+#    #+#             */
/*   Updated: 2024/03/22 18:28:25 by lsadiq           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/response.hpp"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


std::map<std::string, std::string> response::mime_;

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
    (void)index;
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
    if (!_isCgi){
        upfile.write(con + index, size - index);
        upfile.flush();
    }
    else{
        _cgiFile.write(con + index, size - index);
        _cgiFile.flush();
    }
    // upfile.write(con + index, size - index);
    // upfile.flush();
    request.contentLength -= size - index;

    index = size;
    if (request.contentLength == 0)
    {
        if (!_isCgi){
            upfile.close();
            status_code = 201;
        }
        else
            _cgiFile.close();
        postDone = true;
        flag = 0;
    }
}

void    response::parseChunk(const char *con, size_t& index, size_t size)
{
    totalChunkedLength +=size;
    
    if(isLargeContent(totalChunkedLength))
        return;
    while (index < size)
    {

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
                if(!_isCgi)
                    upfile.close();
                else
                    _cgiFile.close();
                // upfile.close();
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
                if(!_isCgi)
                    upfile.close();
                else
                    _cgiFile.close();
                // upfile.close();
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
                if(!_isCgi)
                    upfile.close();
                else
                    _cgiFile.close();
                // upfile.close();
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
                if(!_isCgi)
                    upfile.close();
                else
                    _cgiFile.close();
                // upfile.close();
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
                if(!_isCgi){
                    upfile.write(con + index, chunkSize);
                    upfile.flush();
                }
                else{
                    _cgiFile.write(con + index, chunkSize);
                    _cgiFile.flush();
                }
                // upfile.write(con + index, chunkSize);
                index += chunkSize;
                chunkSize = 0;
                flag = 6;
            }
            else
            {
                if(!_isCgi){
                    upfile.write(con + index, size - index);
                    upfile.flush();
                }
                else{
                    _cgiFile.write(con + index, size - index);
                    _cgiFile.flush();
                }
                // upfile.write(con + index, size - index);
                // upfile.flush();
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
                if(!_isCgi)
                    upfile.close();
                else
                    _cgiFile.close();
                // upfile.close();
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
                if(!_isCgi)
                    upfile.close();
                else
                    _cgiFile.close();
                // upfile.close();
                status_code = 400;
                flag = 0;
                return;
            }
            if (lastChunk)
            {
                if(!_isCgi){
                    upfile.close();
                    status_code = 201;
                }
                else
                    _cgiFile.close();
                // upfile.close();
                // if (!_isCgi)
                postDone = true;
                flag = 0;
                return;
            }
            index++;
            flag = 2;
        }
    }
    
}
bool response::isLargeContent(long long int len)
{
    if(len > request.location.max_body_size)
    {
        remove(uplfile.c_str());
        status_code = 413;
        //remove file
        return true;
    }
    return false;
}


void    response::methodPost(const char *con, size_t size)
{
    targetUri = request.getRealPath();
    if(!allowedMethods())
    {
        status_code = 405;
    }
    else if (flag == 0)
    {
        if(checkType(targetUri) == FILE)
        {
            check_extention(targetUri);
            if(extention == "php" || extention == "py")
                _isCgi = true;
        }
        if (!_isCgi && request.getContentType().find("boundary") != std::string::npos)
        {
            status_code = 501;
            return ; 
        }
        size_t index = 0;
        createFile();
        if (status_code == 403)
            return;
        if (request.getUploadType() == "Chunk")
        {
            flag = 2;
            parseChunk(con, index, size);
        }
        else if (request.getUploadType() == "length")
        {
            if(isLargeContent(request.contentLength))
                return;
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

void    response::fileExtention()
{
    mime_[ "text/html"] = "html";
    mime_[ "text/css"] = "css";
    mime_[ "text/xml"] =     "xml";
    mime_[ "image/gif"] =   "gif";
    mime_[ "image/jpeg"] = "jpeg";
    mime_[ "image/jpg"] = "jpg";
    mime_[ "application/x-javascript"] = "js";
    mime_[ "text/plain"] = "txt";
    mime_[ "text/x-component"] = "htc";
    mime_[ "text/mathml"] = "mml";
    mime_[ "image/png"] = "png";
    mime_[ "image/x-icon"] = "ico";
    mime_[ "image/x-jng"] = "jng";
    mime_[ "image/vnd.wap.wbmp"] =   "wbmp";
    mime_[ "application/mac-binhex40"] = "hqx";
    mime_[ "application/pdf"] = "pdf";
    mime_[ "application/x-cocoa"] = "cco";
    mime_[ "application/x-java-archive-diff"] = "jardiff";
    mime_[ "application/x-java-jnlp-file"] = "jnlp";
    mime_[ "application/x-makeself"] =   "run";
    mime_[ "application/x-perl"] = "pl";
    mime_[ "application/x-pilot"] = "prc";
    mime_[ "application/x-rar-compressed"] = "rar";
    mime_[ "application/x-redhat-package-manager"] = "rpm";
    mime_[ "application/x-sea"] = "sea";
    mime_[ "application/x-shockwave-flash"] = "swf";
    mime_[ "application/x-stuffit"] = "sit";
    mime_[ "application/x-tcl"] =    "tcl";
    mime_[ "application/x-tcl"] =   "tk";
    mime_[ "application/x-x509-ca-cert"] = "der";
    mime_[ "application/x-x509-ca-cert"] = "pem";
    mime_[ "application/x-x509-ca-cert"] = "crt";
    mime_[ "application/x-xpinstall"] = "xpi";
    mime_[ "application/octet-stream"] = "msm";
    mime_[ "audio/mpeg"] = "mp3";
    mime_["video/mp4"] = "mp4";
    mime_[ "audio/x-realaudio"] = "ra";
    mime_[ "video/mpeg"] = "mpeg";
    mime_[ "video/mpeg"] = "mpg";
    mime_[ "video/quicktime"] = "mov";
    mime_[ "video/x-flv"] = "flv";
    mime_[ "application/zip"] = "zip";
    mime_[ "application/octet-stream"] = "deb";
    mime_[ "application/octet-stream"] = "bin";
    mime_[ "application/octet-stream"] = "exe";
    mime_[ "application/octet-stream"] = "dll";
    mime_[ "application/octet-stream"] = "dmg";
    mime_[ "application/octet-stream"] = "eot";
    mime_[ "application/octet-stream"] = "iso";
    mime_[ "application/octet-stream"] = "img";
    mime_[ "application/octet-stream"] = "msi";
    mime_[ "application/octet-stream"] = "msp";
    mime_[ "video/x-msvideo"] = "avi";
    mime_[ "video/x-ms-wmv"] = "wmv";
    mime_[ "video/x-ms-asf"] = "asx";
    mime_[ "video/x-ms-asf"] =   "asf";
    mime_[ "video/x-mng"] = "mng";
}

void response::createFile()
{
    if (request.location.upload.length() > 1) {
        std::string randName = generateName();
        std::string ileType = request.getContentType();
        extention = mime_[ileType];
        if (_isCgi){
            std::string cgitmp = request.location.root + "/tmp";
            if (access(cgitmp.c_str(), F_OK | W_OK) == -1) {
                perror("cgitmp Directory");
                status_code = 500;
            }
            uplfile = cgitmp + "/" + randName + "." + extention;
            _cgiFile.open((uplfile).c_str());
        }
        else{
            std::string UplDir = request.location.upload;
            if (access(UplDir.c_str(), F_OK | W_OK) == -1) {
                perror("Upload Directory");
                status_code = 500;
            }
            uplfile = UplDir + "/" + randName + "." + extention;
            upfile.open((uplfile).c_str());
        }
    }
}


