/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   methodHelp.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kel-baam <kel-baam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 11:01:00 by lsadiq            #+#    #+#             */
/*   Updated: 2024/03/22 13:55:46 by kel-baam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/response.hpp"


template <typename T>
std::string to_string(T value) {
    std::ostringstream os;
    os << value;
    return os.str();
}

std::string response::getTargetUrl(){
			return target_url;
}

std::string response::getTargetUri(){
    return targetUri;
}

std::string response::getLine(){
    return line;
}

std::string response::getName(){
    return name;
}


std::string response::getExtension(){
    return fileType;
}

int response::getFlag(){
    return flag;
}

int response::getStatusCode(){
    return status_code;
}

int response::getFd(){
    return fd;
}

// bool response::getClose(){
//     return close;
// }

std::ifstream& response::getIfile() {
    return ifile;
}

std::ofstream& response::getOfile() {
    return ofile;
}

DIR* response::getDir(){
    return dir;
}

loc response::getLocation(){
    return request.location;
}

std::map<std::string, std::string> response::getMime() {
    return _mime;
}

std::map<std::string, std::string> response::getMime_() {
    return mime_;
}


void    response::fillMime()
{
    _mime["html"] = "text/html";
    _mime["htm"] = "text/html";
    _mime["shtml"] = "text/html";
    _mime["css"] = "text/css";
    _mime["xml"] = "text/xml";
    _mime["rss"] = "text/xml";
    _mime["gif"] = "image/gif";
    _mime["jpeg"] = "image/jpeg";
    _mime["jpg"] = "image/jpg";
    _mime["js"] = "application/x-javascript";
    _mime["txt"] = "text/plain";
    _mime["htc"] = "text/x-component";
    _mime["mml"] = "text/mathml";
    _mime["png"] = "image/png";
    _mime["ico"] = "image/x-icon";
    _mime["jng"] = "image/x-jng";
    _mime["wbmp"] = "image/vnd.wap.wbmp";
    _mime["jar"] = "application/java-archive";
    _mime["war"] = "application/java-archive";
    _mime["ear"] = "application/java-archive";
    _mime["hqx"] = "application/mac-binhex40";
    _mime["pdf"] = "application/pdf";
    _mime["cco"] = "application/x-cocoa";
    _mime["jardiff"] = "application/x-java-archive-diff";
    _mime["jnlp"] = "application/x-java-jnlp-file";
    _mime["run"] = "application/x-makeself";
    _mime["pl"] = "application/x-perl";
    _mime["pm"] = "application/x-perl";
    _mime["prc"] = "application/x-pilot";
    _mime["pdb"] = "application/x-pilot";
    _mime["rar"] = "application/x-rar-compressed";
    _mime["rpm"] = "application/x-redhat-package-manager";
    _mime["sea"] = "application/x-sea";
    _mime["swf"] = "application/x-shockwave-flash";
    _mime["sit"] = "application/x-stuffit";
    _mime["tcl"] = "application/x-tcl";
    _mime["tk"] = "application/x-tcl";
    _mime["der"] = "application/x-x509-ca-cert";
    _mime["pem"] = "application/x-x509-ca-cert";
    _mime["crt"] = "application/x-x509-ca-cert";
    _mime["xpi"] = "application/x-xpinstall";
    _mime["msm"] = "application/octet-stream";
    _mime["mp3"] = "audio/mpeg";
    _mime["mp4"] = "video/mp4";
    _mime["ra"] = "audio/x-realaudio";
    _mime["mpeg"] = "video/mpeg";
    _mime["mpg"] = "video/mpeg";
    _mime["mov"] = "video/quicktime";
    _mime["flv"] = "video/x-flv";
    _mime["zip"] = "application/zip";
    _mime["deb"] = "application/octet-stream";
    _mime["bin"] = "application/octet-stream";
    _mime["exe"] = "application/octet-stream";
    _mime["dll"] = "application/octet-stream";
    _mime["dmg"] = "application/octet-stream";
    _mime["eot"] = "application/octet-stream";
    _mime["iso"] = "application/octet-stream";
    _mime["img"] = "application/octet-stream";
    _mime["msi"] = "application/octet-stream";
    _mime["msp"] = "application/octet-stream";
    _mime["avi"] = "video/x-msvideo";
    _mime["wmv"] = "video/x-ms-wmv";
    _mime["asx"] = "video/x-ms-asf";
    _mime["asf"] = "video/x-ms-asf";
    _mime["mng"] = "video/x-mng";
}

int response::checkType(std::string path)
{
    std::fstream file(path.c_str());
    if (file.is_open())
    {
        file.close();
        return FILE;
    }
    DIR *type = opendir(path.c_str());
    if(type)
    {
        closedir(type);
        return DIRECTORY;
    }
	else
		return NOT_FOUND;
}

void response::check_extention(std::string file)
{
    fillMime();
    std::string::size_type idx;
    idx = file.rfind('.');
    if (idx != std::string::npos)
    {
        extention = file.substr(idx + 1);
        fileType =  _mime[extention];
    }
    else
        fileType = "text/plain";
}

bool    response::autoIndexCheck(){
    if (request.location.auto_index == "on")
        return true;
    return false;
}

bool    response::allowedMethods()
{
    std::vector<std::string>::iterator it = this->request.location.method.begin();
    for(;it != this->request.location.method.end(); it++){
        if(*it == request.getMethod()){
            // std::cout << "method : " << request.getMethod()<< std::endl;
                return true;
        }
    }
    return false;
}

void    response::allow()
{
    std::vector<std::string>::iterator it = this->request.location.method.begin();
    for(;it != this->request.location.method.end(); it++){
            std::cout << *it << std::endl;
    }
}

