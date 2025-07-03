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

void moveCursor(int row, int col)
{
    std::cout << "\033[" << row << ";" << col << "H";
}

int main()
{
    const int HEIGHT = 24;
    const int COLUMN = 40;

    clearScreen();
    hideCursor();

    // green color
    std::cout << "\033[32m";

    for (int row = 1; row <= HEIGHT; ++row)
    {
        clearScreen();
        moveCursor(row, COLUMN);
        std::cout << "A";
        std::cout.flush();

        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }

    std::this_thread::sleep_for(std::chrono::seconds(1));

    // reset everything
    std::cout << "\033[0m";
    showCursor();
    clearScreen();

    return 0;
}