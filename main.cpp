#include <iostream>
#include <thread>
#include <chrono>

void clearScreen()
{
    std::cout << "\033[2J\033[H";
    std::cout.flush();
}

void hideCursor()
{
    std::cout << "\033[?25l";
    std::cout.flush();
}

void showCursor()
{
    std::cout << "\033[?25h";
    std::cout.flush();
}

int main()
{
    clearScreen();
    hideCursor();

    // green color
    std::cout << "\033[32m";

    std::cout << "Screen cleared and cursor hidden!" << std::endl;
    std::cout << "Notice how clean this looks..." << std::endl;

    std::this_thread::sleep_for(std::chrono::seconds(3));

    clearScreen();
    std::cout << "Screen cleared again!" << std::endl;

    std::this_thread::sleep_for(std::chrono::seconds(2));

    // reset everything
    std::cout << "\033[0m";
    showCursor();
    clearScreen();

    return 0;
}