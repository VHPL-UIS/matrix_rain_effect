#ifndef __MATRIX_RAIN_HPP__
#define __MATRIX_RAIN_HPP__

#include <vector>
#include <random>

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

    const char *matrixChars;
    int numChars;

    void calculateTrailLength();
    void initializeScreen();
    void setColor(int intensity, bool isBold, int color = 2);
    void resetColor();
    char getRandomChar();
    int getRandomColor();
    void update();
    void render();
    void updateTerminalSize();

    static DynamicMatrixRain *currentInstance;
    static void signalHandler(int signal);

public:
    explicit DynamicMatrixRain(bool rainbow = false);

    ~DynamicMatrixRain();

    bool getTerminalSize();
    void clearScreen();
    void hideCursor();
    void showCursor();
    void run();

    int getWidth() const { return width; }
    int getHeight() const { return height; }
    int getTrailLength() const { return trailLength; }
    bool isRainbowMode() const { return useRainbow; }
};

#endif // __MATRIX_RAIN_HPP__