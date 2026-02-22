#include <iostream>
#include <fstream>
#include <thread>
#include <sys/stat.h>
#include <sys/types.h>
#include <cstring>
#include <ctime>
#include <chrono>
#include <mutex>
#include <algorithm>
#include <unistd.h>

std::mutex log_mutex;

std::string GetCurrentTime() {
    auto now = std::chrono::system_clock::now();
    auto time_t = std::chrono::system_clock::to_time_t(now);
    std::tm tm = *std::localtime(&time_t);
    char buffer[80];
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &tm);
    return std::string(buffer);
}

void ProcessString(const std::string& input) {
    std::string reversed = input;
    auto start = std::chrono::high_resolution_clock::now();
    // Переворачиваем строку

    std::reverse(reversed.begin(), reversed.end());

    auto end = std::chrono::high_resolution_clock::now();
    auto calc_time = end - start;
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(calc_time);

    {
        std::lock_guard<std::mutex> lock(log_mutex);
        std::ofstream log_file;
        log_file.open("app2.log", std::ios::app);
        if (!log_file.is_open())
            std::cerr << "Ошибка открытия файла лога" << std::endl;
        else
        { 
            log_file << "[" << GetCurrentTime() << "] "
                      << "Input: '" << input << "', "
                      << "Reversed: '" << reversed << "', "
                      << "Processing time: " << duration.count() << " ns" << std::endl;
            log_file.close();
        }

    }

    std::ofstream fifo_out("/tmp/app_fifo");
    fifo_out << reversed << std::endl;
    fifo_out.close();
}

int main() {
    std::string fifo_path("/tmp/app_fifo");
    std::string input;

    while (true)
    {

        if (mkfifo(fifo_path.c_str(), 0666) == -1 && errno != EEXIST) {
            std::cerr << "Failed to create FIFO: " << std::strerror(errno) << std::endl;
            return 1;
        }


        std::ifstream fifo_in(fifo_path.c_str());
        if (!fifo_in.is_open()) {
            std::cerr << "Failed to open FIFO for reading" << std::endl;
            break;
        }

        
        std::getline(fifo_in, input);
        fifo_in.close();

        std::thread reverser(ProcessString, input);
        reverser.join();
        if (input == std::string("exit"))
            break;
    }

    unlink(fifo_path.c_str());
    return 0;
}
