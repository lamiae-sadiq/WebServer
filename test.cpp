#include <cstdlib>
#include <iostream>



int main()
{
    const char* phpscriptpath = "./python.py";

    std::string command = "python3 ";
    command += phpscriptpath;

    int rslt = system(command.c_str());

    if (rslt)
        std::cerr <<"Error executing Python script\n";
    return 0;
}


//freopen()