#include <iostream>
#include <thread>
#include <chrono>
#include <random>
#include <cstring>
#include <vector>
#include <cstdlib>

#ifdef _WIN32
#include <windows.h>
#include <conio.h>
#else
#include <sys/ioctl.h>
#include <unistd.h>
#endif

class DynamicMatrixRain
{
private:
    int width, height;
    int trailLength;

    struct Cell
    {
        char character;
        int intensity; // 0=invisible, 1=dim, 2=normal, 3=bright, 4=bold
        bool isBold;
    };

    std::vector<std::vector<Cell>> screen;
    std::vector<int> drops;
    std::vector<int> dropSpeed;
    std::mt19937 rng;

    const char *matrixChars = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789@#$%^&*";
    int numChars;

public:
    DynamicMatrixRain() : rng(std::chrono::steady_clock::now().time_since_epoch().count())
    {
        numChars = strlen(matrixChars);
        getTerminalSize();
        calculateTrailLength();
        initializeScreen();
    }

    bool getTerminalSize()
    {
#ifdef _WIN32
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi))
        {
            width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
            height = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
            return true;
        }
#else
        struct winsize w;
        if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == 0)
        {
            width = w.ws_col;
            height = w.ws_row;
            return true;
        }
#endif
        // Fallback to common terminal size
        width = 80;
        height = 24;
        return false;
    }

    void calculateTrailLength()
    {
        // Scale trail length based on terminal height
        if (height <= 24)
        {
            trailLength = 12;
        }
        else if (height <= 40)
        {
            trailLength = 18;
        }
        else if (height <= 60)
        {
            trailLength = 25;
        }
        else
        {
            trailLength = 30;
        }
    }

    void initializeScreen()
    {
        try
        {
            // Resize screen buffer to actual terminal size
            screen.clear();
            screen.resize(height, std::vector<Cell>(width));

            // Clear screen
            for (int y = 0; y < height; ++y)
            {
                for (int x = 0; x < width; ++x)
                {
                    screen[y][x].character = ' ';
                    screen[y][x].intensity = 0;
                    screen[y][x].isBold = false;
                }
            }

            // Resize drop arrays
            drops.clear();
            drops.resize(width);
            dropSpeed.clear();
            dropSpeed.resize(width);

            // Initialize drops with random delays and speeds
            for (int x = 0; x < width; ++x)
            {
                drops[x] = -(rng() % height);   // random start delay
                dropSpeed[x] = 1 + (rng() % 3); // speed 1-3
            }

            std::cout << "Terminal size detected: " << width << "x" << height << std::endl;
            std::cout << "Trail length: " << trailLength << std::endl;
        }
        catch (const std::exception &e)
        {
            std::cerr << "Error initializing screen: " << e.what() << std::endl;
            exit(1);
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
        for (int y = 0; y < height; ++y)
        {
            for (int x = 0; x < width; ++x)
            {
                screen[y][x].character = ' ';
                screen[y][x].intensity = 0;
                screen[y][x].isBold = false;
            }
        }

        // Update each column
        for (int x = 0; x < width; ++x)
        {
            // Draw trail with varying intensity
            for (int i = 0; i < trailLength; ++i)
            {
                int y = drops[x] - i;
                if (y >= 0 && y < height)
                {
                    int intensity;
                    if (i == 0)
                    {
                        // Head of the drop - brightest
                        intensity = 4;
                        screen[y][x].isBold = (rng() % 3 == 0); // 33% chance of bold
                    }
                    else if (i < trailLength / 4)
                    {
                        // Near head - bright
                        intensity = 3;
                        screen[y][x].isBold = (rng() % 5 == 0); // 20% chance of bold
                    }
                    else if (i < trailLength / 2)
                    {
                        // Middle - normal
                        intensity = 2;
                        screen[y][x].isBold = (rng() % 8 == 0); // 12.5% chance of bold
                    }
                    else if (i < (trailLength * 3) / 4)
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
            if (drops[x] > height + trailLength)
            {
                drops[x] = -(rng() % (height / 2));
                dropSpeed[x] = 1 + (rng() % 3);
            }
        }
    }

    void render()
    {
        std::cout << "\033[H"; // Move to top-left

        for (int y = 0; y < height - 1; ++y)
        {
            for (int x = 0; x < width; ++x)
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

    void updateTerminalSize()
    {
        int oldWidth = width;
        int oldHeight = height;

        getTerminalSize();

        // If terminal size changed, reinitialize
        if (width != oldWidth || height != oldHeight)
        {
            calculateTrailLength();
            initializeScreen();
            clearScreen();
        }
    }

    void run()
    {
        clearScreen();
        hideCursor();

        std::cout << "Dynamic Full Screen Matrix Rain with Enhanced Effects!\n";
        std::cout << "Resize your terminal and watch it adapt!\n";
        std::cout.flush();
        std::this_thread::sleep_for(std::chrono::milliseconds(3000));

        // Run animation with periodic size checks
        for (int frame = 0; frame < 1000; ++frame)
        {
            // Check for terminal resize every 50 frames
            if (frame % 50 == 0)
            {
                updateTerminalSize();
            }

            update();
            render();
            std::this_thread::sleep_for(std::chrono::milliseconds(80));
        }

        showCursor();
        resetColor();
        clearScreen();
        std::cout << "Matrix effect completed!\n";
        std::cout << "Final terminal size was: " << width << "x" << height << std::endl;
    }
};

int main()
{
    try
    {
        DynamicMatrixRain matrix;
        matrix.run();
    }
    catch (const std::exception &e)
    {
        std::cout << "\033[?25h\033[0m\033[2J\033[H";
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}