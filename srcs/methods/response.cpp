/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kel-baam <kel-baam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 16:25:03 by lsadiq            #+#    #+#             */
/*   Updated: 2024/03/16 21:59:41 by kel-baam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/response.hpp"

template <typename T>
std::string to_string(T value) {
    std::ostringstream os;
    os << value;
    return os.str();
}
void    response::sendData()
{       
    if (flag == 2)
    {
        ifile.open(targetUri.c_str());
        if (!ifile.is_open())
            throw std::runtime_error("Error: failed to open video ifile");
        std::string resHeader = "HTTP/1.1 " + to_string(status_code) + " " + setStatus(status_code);
            resHeader += "\r\nContent-Type: ";
            resHeader += content_type;
            resHeader +="\r\n";
            resHeader += "Transfer-Encoding: chunked\r\n";
            resHeader += "\r\n";
            send(fd, resHeader.c_str(), resHeader.length(), 0);
        flag = 3;
    }
    if (flag == 3)
    {
        const int chunkSize = 1024;
        char buffer[chunkSize];
        ifile.read(buffer, chunkSize);
        int bytesRead = ifile.gcount();
        std::stringstream ss;
        ss << std::hex << bytesRead;
        std::string chunkSizeHex = ss.str();
        std::string chunkHeader = chunkSizeHex + "\r\n";
        char concatenatedSends[chunkHeader.length() + bytesRead + 2];
        memcpy(concatenatedSends, chunkHeader.c_str(), chunkHeader.length());
        memcpy(concatenatedSends + chunkHeader.length(), buffer, bytesRead);
        memcpy(concatenatedSends + chunkHeader.length() + bytesRead, "\r\n", 2);
        write (fd, concatenatedSends, chunkHeader.length() + bytesRead + 2);
        
        if (ifile.eof()){
            flag = 4;
        }
    }
    if (flag == 4) {
        send(fd, "0\r\n\r\n", 5, 0);
        ifile.close();
        flag = 30;
        // std::cout << "File sent" << std::endl;
    }
}

const std::string response::HTMLPage() {
    return ("<html lang=\"en\">\n"
            "<head>\n"
            "<meta charset=\"UTF-8\">\n"
            "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n"
            "<title>WebServe</title>\n"
            "<style>\n"
            "body {\n"
            "    margin: 0;\n"
            "    padding: 0;\n"
            "    height: 90vh;\n"
            "    background-image: url('https://64.media.tumblr.com/da82730a16d6b909da5d35044a4bafd1/tumblr_mr22e5xg0X1qbaxlqo1_400.gifv');\n"
            "    background-size: 2200px 1050px;\n"
            "    background-repeat: no-repeat;\n"
            "    background-attachment: fixed;\n"
            "    display: flex;\n"
            "    justify-content: center;\n"
            "    align-items: center;\n"
            "}\n"
            ".message {\n"
            "    font-size: 100px;\n"
            "    font-family: Fancopltasy, sans-serif;\n"
            "    color: hsl(217, 46%, 37%);\n"
            "    text-align: center;\n"
            "    text-shadow: 2px 2px 4px #000000;\n"
            "    font-weight: bold;\n"
            "}\n"
            "</style>\n"
            "</head>\n"
            "<body>\n"
            "<div class=\"message\">" + to_string(status_code) +"</span> "+ setStatus(status_code) +"</div>\n"
            "</body>\n"
            "</html>\n");
}

std::string response::getErrorPage() {
    if (serv.getErrorPage().find(status_code) != serv.getErrorPage().end()) {
        // std::cout << serv.getErrorPage()[status_code] << "00000\n";
        return serv.getErrorPage()[status_code];
    }
    return "404";
}

void response::sendErrorPage()
{
    targetUri = serv.getErrorPage()[status_code];
    std::cout << targetUri << std::endl;
    if(access(targetUri.c_str(), R_OK | W_OK) == 0)
    {
        flag = 2;
        check_extention(targetUri);
        content_type = fileType;
        sendData();
    }
    else{
        if(status_code >= 400)
            ErrorHeader();
        else
            setHeader();
    }
}

void response::ErrorHeader()
{
    const std::string errorPage = HTMLPage();
    std::cout << "method : " << request.getMethod() << std::endl;
    std::string header = "HTTP/1.1 " + to_string(status_code) + " " + setStatus(status_code);
    header.append("\r\nContent-Type: text/html\r\nContent-length:  ");
    header.append(to_string(errorPage.length()));
    header += "\r\n\r\n";
    if (request.getMethod() != "HEAD") {
        header.append(errorPage);
    }
    send(fd, header.c_str(), header.length(), 0);
    flag = 30;
}

void response::setHeader()
{
    if (status_code == 301)
    {
        std::string header = "HTTP/1.1 " + to_string(status_code) + " " + setStatus(status_code);
        header.append("\r\nLocation: " + target_url);
        header += "\r\n\r\n";
        send(fd, header.c_str(), header.length(), 0);
        flag = 30;
    }
    if (status_code >= 200 && status_code <= 204)
    {
        std::cout << "------------------" << status_code << std::endl;
        const std::string responsePage = HTMLPage();
        std::string header = "HTTP/1.1 " + to_string(status_code) + " " + setStatus(status_code);
        header.append("\r\nContent-Type: text/html\r\nContent-length:  ");
        header.append(to_string(responsePage.length()));
        header += "\r\n\r\n";
        header.append(responsePage);
        send(fd, header.c_str(), header.length(), 0);
        flag = 30;
    }
}
/// recheck
void response::listDirectories()
{
    std::string html;
    if(flag == 6){
        std::string header = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nTransfer-Encoding: chunked\r\n\r\n";
        send(fd, header.c_str(), header.length(), 0);
        flag = 7;
    }
    else if(flag == 7){
        html.append("<html>\n<head>\n<title>Index of ");
        html.append(target_url); 
        html.append("</title>\n</head>\n<body>\n<h1>Index of ");
        html.append(target_url); 
        html.append("</h1>\n<hr>\n<ul>\n");
        std::stringstream ss;
        ss << std::hex << html.length() << "\r\n";
        send(fd, ss.str().c_str(), ss.str().length(), 0);
        html.append("\r\n");
        send(fd, html.c_str(), html.length(), 0);
        if (!(dir = opendir(targetUri.c_str())))
        {
            std::cout << "Error: failed to open directory" << std::endl;
            status_code = 500;
            return;
        }
        flag = 8;
    }
    else if(flag == 8){
        struct dirent* entry;
        if (!(entry = readdir(dir)))
        {
            flag = 9;
            return;
        }
        std::string fileName = entry->d_name;
        if (fileName[0] == '.')
            return;
        html += "<li><a href=\"" + fileName + "\">" + fileName + "</a></li>\n";
        std::stringstream ss;
        ss << std::hex << html.length() << "\r\n";
        send(fd, ss.str().c_str(), ss.str().length(), 0);
        html.append("\r\n");
        send(fd, html.c_str(), html.length(), 0);
    }
    else if(flag == 9){
        closedir(dir);
        dir = NULL;
        html = "</ul>\n<hr>\n</body>\n</html>\n";
        std::stringstream ss;
        ss << std::hex << html.length() << "\r\n";
        send(fd, ss.str().c_str(), ss.str().length(), 0);
        html.append("\r\n");
        send(fd, html.c_str(), html.length(), 0);
        flag = 10;
    }
    else if(flag == 10){
        send(fd, "0\r\n\r\n", 5, 0);
        flag = 30;
    }
}


void response::executeMethodes(const char *buff,size_t size,int fd)
{
    (void)fd;
    
    if ((status_code != 200 && flag == 0) || status_code == 408){
        getErrorPage();
        sendErrorPage();
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