#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
    std::string fifo_path("/tmp/app_fifo");

    while(1)
    {

        if (mkfifo(fifo_path.c_str(), 0666) == -1 && errno != EEXIST) {
            std::cerr << "Failed to create FIFO: " << std::strerror(errno) << std::endl;
            return 1;
        }

        std::string input("");
        std::cout << "Enter string to reverse or \"exit\" to stop apps:  ";
        while(input.empty())
        {
            std::getline(std::cin, input);
            if (input.empty())
                std::cout << "Please, enter non-empty string or \"exit\" to stop apps: ";
        }

        std::ofstream fifo_out(fifo_path);
        if (!fifo_out) {
            std::cerr << "Failed to open FIFO for writing" << std::endl;
            break;
        }
        fifo_out << input << std::endl;
        fifo_out.close();


        // Читаем ответ
        std::ifstream fifo_in(fifo_path);
        if (!fifo_in.is_open()) {
            std::cerr << "Failed to open FIFO for reading" << std::endl;
            break;
        }
        std::string reversed;
        std::getline(fifo_in, reversed);
        fifo_in.close();

        if (input == std::string("exit"))
            break;

        std::cout << "Reversed string: " << reversed << std::endl;
    }

    unlink(fifo_path.c_str());
    
    return 0;
}
