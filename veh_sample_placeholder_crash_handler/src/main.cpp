#include <iostream> 
#include <fstream>
#include <string>

int main()
{
    std::cout << "The program has crashed!" << std::endl;

    std::ifstream crash_log_file(CRASH_LOG_FILENAME);
    if (!crash_log_file.is_open())
    {
        std::cout << "Could not open crash log" << std::endl;
    }

    std::string line;
    while (std::getline(crash_log_file, line))
    {
        std::cout << line << std::endl;
    }

    crash_log_file.close();

    return 0;
}
