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
    const int WIDTH = 80;
    const int TRAIL_LENGTH = 8;
    const int NUM_COLUMNS = 10;

    // array to track each column's position
    int columnPositions[NUM_COLUMNS];
    int columnSpacing = WIDTH / NUM_COLUMNS;

    // initialize column positions
    for (int i = 0; i < NUM_COLUMNS; ++i)
    {
        columnPositions[i] = 1;
    }

    clearScreen();
    hideCursor();

    // animate multiple columns
    for (int frame = 0; frame <= 100; ++frame)
    {
        clearScreen();

        for (int col = 0; col < NUM_COLUMNS; ++col)
        {
            int x = (col + 1) * columnSpacing;

            for (int i = 0; i < TRAIL_LENGTH; ++i)
            {
                int row = columnPositions[col] - i;
                if (row >= 1 && row <= HEIGHT)
                {
                    moveCursor(row, x);

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

            columnPositions[col]++;

            if (columnPositions[col] > HEIGHT + TRAIL_LENGTH)
            {
                columnPositions[col] = 1;
            }
        }

        std::cout.flush();
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }

    // reset everything
    std::cout << "\033[0m";
    showCursor();
    clearScreen();

    return 0;
}