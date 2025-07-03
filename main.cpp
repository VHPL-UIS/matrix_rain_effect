#include <iostream>
#include <thread>
#include <chrono>

int main()
{
    // green color
    std::cout << "\033[32m";

    std::cout << "Hello Matrix World!" << std::endl;
    std::cout << "This text is green!" << std::endl;

    std::this_thread::sleep_for(std::chrono::seconds(3));

    // reset color
    std::cout << "\033[0m";
    std::cout << "Color reseted!" << std::endl;

    return 0;
}