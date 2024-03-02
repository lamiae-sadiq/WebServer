#include <iostream>
#include <fstream>
#include <unordered_map>

class Config {
public:
    Config(const std::string& filePath) {
        loadConfig(filePath);
    }

    std::string getErrorPage(int statusCode) {
        if (errorPages.find(statusCode) != errorPages.end()) {
            return errorPages[statusCode];
        }
        return "";
    }

private:
    std::unordered_map<int, std::string> errorPages;

    void loadConfig(const std::string& filePath) {
        std::ifstream configFile(filePath);
        if (configFile.is_open()) {
            std::string line;
            while (std::getline(configFile, line)) {
                if (line.empty() || line[0] == '#') {
                    continue; // Skip empty lines and comments
                }
                int statusCode;
                std::string errorPage;
                if (parseConfigLine(line, statusCode, errorPage)) {
                    errorPages[statusCode] = errorPage;
                }
            }
            configFile.close();
        } else {
            std::cerr << "Failed to open config file: " << filePath << std::endl;
        }
    }

    bool parseConfigLine(const std::string& line, int& statusCode, std::string& errorPage) {
        size_t colonPos = line.find(':');
        if (colonPos != std::string::npos) {
            std::string statusCodeStr = line.substr(0, colonPos);
            std::string errorPageStr = line.substr(colonPos + 1);
            try {
                statusCode = std::stoi(statusCodeStr);
                errorPage = errorPageStr;
                return true;
            } catch (const std::exception& e) {
                std::cerr << "Invalid config line: " << line << std::endl;
            }
        }
        return false;
    }
};

int main() {
    Config config("/path/to/config.txt");
    std::string errorPage = config.getErrorPage(404);
    if (!errorPage.empty()) {
        std::cout << "Error page for 404: " << errorPage << std::endl;
    } else {
        std::cout << "No error page found for 404" << std::endl;
    }
    return 0;
}
