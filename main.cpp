#include <iostream>
#include <thread>
#include <chrono>
#include <random>
#include <cstring>

class SimpleMatrix
{
private:
    static const int WIDTH = 80;
    static const int HEIGHT = 24;
    static const int TRAIL_LENGTH = 12;

    struct Cell
    {
        char character;
        int intensity; // 0=invisible, 1=dim, 2=normal, 3=bright, 4=bold
        bool isBold;
    };

    Cell screen[HEIGHT][WIDTH];
    int drops[WIDTH];
    int dropSpeed[WIDTH];
    std::mt19937 rng;

    const char *matrixChars = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789@#$%^&*";
    int numChars;

public:
    SimpleMatrix() : rng(std::chrono::steady_clock::now().time_since_epoch().count())
    {
        numChars = strlen(matrixChars);
        initializeScreen();
    }

    void initializeScreen()
    {
        // clear screen
        for (int y = 0; y < HEIGHT; ++y)
        {
            for (int x = 0; x < WIDTH; ++x)
            {
                screen[y][x].character = ' ';
                screen[y][x].intensity = 0;
                screen[y][x].isBold = false;
            }
        }

        // initialize drops with random delays and speeds
        for (int x = 0; x < WIDTH; ++x)
        {
            drops[x] = -(rng() % HEIGHT);   // random start delay
            dropSpeed[x] = 1 + (rng() % 3); // speed 1-3
        }
    }

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

    void setColor(int intensity, bool isBold)
    {
        if (intensity == 0)
        {
            std::cout << "\033[30m"; // Black (invisible)
        }
        else if (intensity == 1)
        {
            std::cout << "\033[2;32m"; // Dim green
        }
        else if (intensity == 2)
        {
            std::cout << "\033[32m"; // Normal green
        }
        else if (intensity == 3)
        {
            std::cout << "\033[1;32m"; // Bright green
        }
        else
        {                              // intensity == 4
            std::cout << "\033[1;92m"; // Bold bright green
        }

        if (isBold && intensity > 1)
        {
            std::cout << "\033[1m"; // Make it bold
        }
    }

    void resetColor()
    {
        std::cout << "\033[0m";
    }

    char getRandomChar()
    {
        return matrixChars[rng() % numChars];
    }

    void update()
    {
        // Clear screen array
        for (int y = 0; y < HEIGHT; ++y)
        {
            for (int x = 0; x < WIDTH; ++x)
            {
                screen[y][x].character = ' ';
                screen[y][x].intensity = 0;
                screen[y][x].isBold = false;
            }
        }

        // Update each column
        for (int x = 0; x < WIDTH; ++x)
        {
            // Draw trail
            for (int i = 0; i < TRAIL_LENGTH; ++i)
            {
                int y = drops[x] - i;
                if (y >= 0 && y < HEIGHT)
                {
                    int intensity;
                    if (i == 0)
                    {
                        // Head of the drop - brightest
                        intensity = 4;
                        screen[y][x].isBold = (rng() % 3 == 0); // 33% chance of bold
                    }
                    else if (i < 3)
                    {
                        // Near head - bright
                        intensity = 3;
                        screen[y][x].isBold = (rng() % 5 == 0); // 20% chance of bold
                    }
                    else if (i < 8)
                    {
                        // Middle - normal
                        intensity = 2;
                        screen[y][x].isBold = (rng() % 8 == 0); // 12.5% chance of bold
                    }
                    else if (i < 12)
                    {
                        // Fading - dim
                        intensity = 1;
                        screen[y][x].isBold = false;
                    }
                    else
                    {
                        // Very faint - barely visible
                        intensity = (rng() % 2); // Random between 0 and 1
                        screen[y][x].isBold = false;
                    }

                    screen[y][x].character = getRandomChar();
                    screen[y][x].intensity = intensity;

                    // Add some random flickering
                    if (rng() % 20 == 0)
                    {
                        screen[y][x].intensity = std::max(0, screen[y][x].intensity - 1);
                    }
                }
            }

            // Move drop
            drops[x] += dropSpeed[x];

            // Reset if off screen
            if (drops[x] > HEIGHT + TRAIL_LENGTH)
            {
                drops[x] = -(rng() % 20);
                dropSpeed[x] = 1 + (rng() % 3);
            }
        }
    }

    void render()
    {
        std::cout << "\033[H"; // Move to top-left

        for (int y = 0; y < HEIGHT - 1; ++y)
        {
            for (int x = 0; x < WIDTH; ++x)
            {
                if (screen[y][x].intensity > 0)
                {
                    setColor(screen[y][x].intensity, screen[y][x].isBold);
                    std::cout << screen[y][x].character;
                    std::cout << "\033[0m"; // Reset formatting after each character
                }
                else
                {
                    std::cout << ' ';
                }
            }
            std::cout << '\n';
        }

        std::cout.flush();
    }

    void run()
    {
        clearScreen();
        hideCursor();

        std::cout << "Full Screen Matrix Rain!\n";
        std::cout.flush();
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));

        auto startTime = std::chrono::steady_clock::now();
        auto duration = std::chrono::seconds(30);

        while (std::chrono::steady_clock::now() - startTime < duration)
        {
            update();
            render();
            std::this_thread::sleep_for(std::chrono::milliseconds(80));
        }

        showCursor();
        resetColor();
        clearScreen();
        std::cout << "Matrix effect completed!\n";
    }
};

int main()
{
    SimpleMatrix matrix;
    matrix.run();

    return 0;
}