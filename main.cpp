#include <iostream>
#include <thread>
#include <chrono>
#include <random>

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

char getRandomChar()
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static const char chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789@#$%^&*";
    static std::uniform_int_distribution<> dis(0, sizeof(chars) - 2);

    return chars[dis(gen)];
}

int main()
{
    const int HEIGHT = 24;
    const int COLUMN = 40;
    const int TRAIL_LENGTH = 8;

    clearScreen();
    hideCursor();

    for (int pos = 1; pos <= HEIGHT + TRAIL_LENGTH; ++pos)
    {
        clearScreen();

        for (int i = 0; i < TRAIL_LENGTH; ++i)
        {
            int row = pos - i;
            if (row >= 1 && row <= HEIGHT)
            {
                moveCursor(row, COLUMN);

                if (i == 0)
                {
                    std::cout << "\033[1;32m"; // Bright green for head
                }
                else if (i < 3)
                {
                    std::cout << "\033[32m"; // Normal green
                }
                else
                {
                    std::cout << "\033[2;32m"; // Dim green for tail
                }

                std::cout << getRandomChar();
            }
        }

        std::cout.flush();
        std::this_thread::sleep_for(std::chrono::milliseconds(150));
    }

    std::this_thread::sleep_for(std::chrono::seconds(1));

    // reset everything
    std::cout << "\033[0m";
    showCursor();
    clearScreen();

    return 0;
}