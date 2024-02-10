void Response::find_files() {
    DIR *dir;
	struct dirent *d;
	struct stat s;
	char buff[18];
    std::map<std::string, unsigned char> files;
	std::string file, name, path;
    unsigned char type;
	file = "<!DOCTYPE html>\n<html>\n<head>\n<title>Index of ";
	file += _old_uri;
	file += "</title>\n</head>\n<body>\n<h1>Index of ";
	file += _old_uri + "</h1>\n<hr>\n<pre>\n<table>\n<tbody>\n";
	dir = opendir(_uri.c_str());
    if (dir)    {
        d = readdir(dir);
        while (d)   {
            name = d->d_name;
            if (name == "." || (name != ".." && name[0] == '.'))    {
                d = readdir(dir);
                std::cout << "d " << d << std::endl;
                continue;
            }
            type = d->d_type;
            files.insert(std::pair<std::string, unsigned char>(name, type));
            d = readdir(dir);
        }
        for(std::map<std::string, unsigned char>::iterator it = files.begin(); it != files.end(); it++) {
            name = it->first;
            type = it->second;
			path = _uri + "/" + name;
			stat(path.c_str(), &s);
			file += "<tr>\n<td><a href=" + name;
			if (type == DT_DIR)
				file += "/>" + name + "/</a></td>\n";
			else
				file += ">" + name + "</a></td>\n";
			strftime(buff, sizeof(buff), "%d-%b-%Y %H:%M", gmtime(&s.st_mtim.tv_sec));
			file += "<td>";
			file += buff;
			file += "</td>";
            file += "<td>";
            std::stringstream ss;
            std::string tmp;
            ss << s.st_size;
            ss >> tmp;
            if (type == DT_DIR)
                file += "-";
            else
                file += tmp;
            file += "</td></tr>\n";
        }
        file += "</tbody>\n</table>\n<hr>\n</pre>\n</body>\n</html>";
    }
	closedir(dir);
    std::string tmp = "/nfs/homes/";
    tmp += USER;
    tmp += "/.cache/autoindex.html"; 
    _file.open(tmp.c_str(), std::ios::out | std::ios::trunc);
    if (!_file.good())  {
        _status_code = 404;
        return;
    }
    _file.write(file.c_str(), file.length());
    std::stringstream sg;
    sg << file.length();
    sg >> _content_length;
    _file.close();
    _file.open(tmp.c_str(), std::ios::in);
}

