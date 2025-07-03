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
    bool useRainbow;

    struct Cell
    {
        char character;
        int intensity; // 0=invisible, 1=dim, 2=normal, 3=bright, 4=bold
        bool isBold;
        int color; // for rainbow mode: 0-6 (red, yellow, green, cyan, blue, magenta, white)
    };

    std::vector<std::vector<Cell>> screen;
    std::vector<int> drops;
    std::vector<int> dropSpeed;
    std::mt19937 rng;

    const char *matrixChars = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789@#$%^&*";
    int numChars;

public:
    DynamicMatrixRain(bool rainbow = false) : useRainbow(rainbow), rng(std::chrono::steady_clock::now().time_since_epoch().count())
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
        // fallback to common terminal size
        width = 80;
        height = 24;
        return false;
    }

    void calculateTrailLength()
    {
        // scale trail length based on terminal height
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
            // resize screen buffer to actual terminal size
            screen.clear();
            screen.resize(height, std::vector<Cell>(width));

            // clear screen
            for (int y = 0; y < height; ++y)
            {
                for (int x = 0; x < width; ++x)
                {
                    screen[y][x].character = ' ';
                    screen[y][x].intensity = 0;
                    screen[y][x].isBold = false;
                    screen[y][x].color = 0;
                }
            }

            // resize drop arrays
            drops.clear();
            drops.resize(width);
            dropSpeed.clear();
            dropSpeed.resize(width);

            // initialize drops with random delays and speeds
            for (int x = 0; x < width; ++x)
            {
                drops[x] = -(rng() % height);   // random start delay
                dropSpeed[x] = 1 + (rng() % 3); // speed 1-3
            }

            std::cout << "Terminal size detected: " << width << "x" << height << std::endl;
            std::cout << "Trail length: " << trailLength << std::endl;
            std::cout << "Mode: " << (useRainbow ? "Rainbow" : "Matrix Green") << std::endl;
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

    void setColor(int intensity, bool isBold, int color = 2)
    {
        if (useRainbow)
        {
            // rainbow colors: red, yellow, green, cyan, blue, magenta, white
            const int colors[] = {31, 33, 32, 36, 34, 35, 37};

            if (intensity == 0)
            {
                std::cout << "\033[30m"; // black (invisible)
            }
            else if (intensity == 1)
            {
                std::cout << "\033[2;" << colors[color] << "m"; // dim color
            }
            else if (intensity == 2)
            {
                std::cout << "\033[" << colors[color] << "m"; // normal color
            }
            else if (intensity == 3)
            {
                std::cout << "\033[1;" << colors[color] << "m"; // bright color
            }
            else
            {                                                          // intensity == 4
                std::cout << "\033[1;" << (colors[color] + 60) << "m"; // bold bright color
            }
        }
        else
        {
            // original green matrix colors
            if (intensity == 0)
            {
                std::cout << "\033[30m"; // black (invisible)
            }
            else if (intensity == 1)
            {
                std::cout << "\033[2;32m"; // dim green
            }
            else if (intensity == 2)
            {
                std::cout << "\033[32m"; // normal green
            }
            else if (intensity == 3)
            {
                std::cout << "\033[1;32m"; // bright green
            }
            else
            {                              // intensity == 4
                std::cout << "\033[1;92m"; // bold bright green
            }
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

    int getRandomColor()
    {
        return rng() % 7; // 0-6 for rainbow colors
    }

    void update()
    {
        // clear screen array
        for (int y = 0; y < height; ++y)
        {
            for (int x = 0; x < width; ++x)
            {
                screen[y][x].character = ' ';
                screen[y][x].intensity = 0;
                screen[y][x].isBold = false;
                screen[y][x].color = 0;
            }
        }

        // update each column
        for (int x = 0; x < width; ++x)
        {
            // draw trail with varying intensity
            for (int i = 0; i < trailLength; ++i)
            {
                int y = drops[x] - i;
                if (y >= 0 && y < height)
                {
                    int intensity;
                    if (i == 0)
                    {
                        // head of the drop - brightest
                        intensity = 4;
                        screen[y][x].isBold = (rng() % 3 == 0); // 33% chance of bold
                    }
                    else if (i < trailLength / 4)
                    {
                        // near head - bright
                        intensity = 3;
                        screen[y][x].isBold = (rng() % 5 == 0); // 20% chance of bold
                    }
                    else if (i < trailLength / 2)
                    {
                        // diddle - normal
                        intensity = 2;
                        screen[y][x].isBold = (rng() % 8 == 0); // 12.5% chance of bold
                    }
                    else if (i < (trailLength * 3) / 4)
                    {
                        // fading - dim
                        intensity = 1;
                        screen[y][x].isBold = false;
                    }
                    else
                    {
                        // very faint - barely visible
                        intensity = (rng() % 2); // random between 0 and 1
                        screen[y][x].isBold = false;
                    }

                    screen[y][x].character = getRandomChar();
                    screen[y][x].intensity = intensity;

                    // set color for rainbow mode
                    if (useRainbow)
                    {
                        screen[y][x].color = getRandomColor();
                    }

                    // add some random flickering
                    if (rng() % 20 == 0)
                    {
                        screen[y][x].intensity = std::max(0, screen[y][x].intensity - 1);
                    }
                }
            }

            // move drop
            drops[x] += dropSpeed[x];

            // reset if off screen
            if (drops[x] > height + trailLength)
            {
                drops[x] = -(rng() % (height / 2));
                dropSpeed[x] = 1 + (rng() % 3);
            }
        }
    }

    void render()
    {
        std::cout << "\033[H"; // move to top-left

        for (int y = 0; y < height - 1; ++y)
        {
            for (int x = 0; x < width; ++x)
            {
                if (screen[y][x].intensity > 0)
                {
                    setColor(screen[y][x].intensity, screen[y][x].isBold, screen[y][x].color);
                    std::cout << screen[y][x].character;
                    std::cout << "\033[0m"; // reset formatting after each character
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

        // if terminal size changed, reinitialize
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
        std::cout << "Mode: " << (useRainbow ? "Rainbow Colors" : "Classic Matrix Green") << "\n";
        std::cout << "Resize your terminal and watch it adapt!\n";
        std::cout.flush();
        std::this_thread::sleep_for(std::chrono::milliseconds(3000));

        // run animation with periodic size checks
        for (int frame = 0; frame < 1000; ++frame)
        {
            // check for terminal resize every 50 frames
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

void showMenu()
{
    std::cout << "\033[2J\033[H"; // clear screen
    std::cout << "\033[1;32m";    // bright green
    std::cout << "╔═══════════════════════════════════════╗\n";
    std::cout << "║           MATRIX RAIN EFFECT          ║\n";
    std::cout << "╠═══════════════════════════════════════╣\n";
    std::cout << "║                                       ║\n";
    std::cout << "║  \033[1;32m1. Classic Matrix Green\033[1;32m              ║\n";
    std::cout << "║  \033[1;91m2\033[1;93m.\033[1;92m \033[1;96mR\033[1;94ma\033[1;95mi\033[1;97mn\033[1;91mb\033[1;93mo\033[1;92mw\033[1;32m \033[1;96mC\033[1;94mo\033[1;95ml\033[1;97mo\033[1;91mr\033[1;93ms\033[1;32m                    ║\n";
    std::cout << "║  \033[1;31m3. Exit\033[1;32m                              ║\n";
    std::cout << "║                                       ║\n";
    std::cout << "╚═══════════════════════════════════════╝\n";
    std::cout << "\033[0m"; // reset color
    std::cout << "\nEnter your choice (1-3): ";
}

int getUserChoice()
{
    int choice;
    while (true)
    {
        std::cin >> choice;
        if (std::cin.fail() || choice < 1 || choice > 3)
        {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            std::cout << "\033[1;31mInvalid choice! Please enter 1, 2, or 3: \033[0m";
        }
        else
        {
            std::cin.ignore(10000, '\n'); // clear input buffer
            return choice;
        }
    }
}

int main()
{
    try
    {
        while (true)
        {
            showMenu();
            int choice = getUserChoice();

            switch (choice)
            {
            case 1:
            {
                DynamicMatrixRain matrix(false); // green matrix
                matrix.run();
                break;
            }
            case 2:
            {
                DynamicMatrixRain matrix(true); // rainbow colors
                matrix.run();
                break;
            }
            case 3:
            {
                std::cout << "\033[2J\033[H"; // clear screen
                std::cout << "\033[1;32mGoodbye! Thanks for using Matrix Rain!\033[0m\n";
                return 0;
            }
            }

            // ask if user wants to continue
            std::cout << "\033[1;36mPress Enter to return to menu or Ctrl+C to exit...\033[0m";
            std::cin.get();
        }
    }
    catch (const std::exception &e)
    {
        std::cout << "\033[?25h\033[0m\033[2J\033[H";
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}