#include "MenuInterface.hpp"
#include <iostream>

void MenuInterface::clearScreen()
{
    std::cout << "\033[2J\033[H";
    std::cout.flush();
}

void MenuInterface::setColor(const char *colorCode)
{
    std::cout << colorCode;
}

void MenuInterface::resetColor()
{
    std::cout << "\033[0m";
}

void MenuInterface::showMenu()
{
    clearScreen();
    setColor("\033[1;32m"); // bright green

    std::cout << "╔═══════════════════════════════════════╗\n";
    std::cout << "║           MATRIX RAIN EFFECT          ║\n";
    std::cout << "╠═══════════════════════════════════════╣\n";
    std::cout << "║                                       ║\n";
    std::cout << "║  ";
    setColor("\033[1;32m");
    std::cout << "1. Classic Matrix Green";
    setColor("\033[1;32m");
    std::cout << "              ║\n";
    std::cout << "║  ";
    setColor("\033[1;91m");
    std::cout << "2";
    setColor("\033[1;93m");
    std::cout << ".";
    setColor("\033[1;92m");
    std::cout << " ";
    setColor("\033[1;96m");
    std::cout << "R";
    setColor("\033[1;94m");
    std::cout << "a";
    setColor("\033[1;95m");
    std::cout << "i";
    setColor("\033[1;97m");
    std::cout << "n";
    setColor("\033[1;91m");
    std::cout << "b";
    setColor("\033[1;93m");
    std::cout << "o";
    setColor("\033[1;92m");
    std::cout << "w";
    setColor("\033[1;32m");
    std::cout << " ";
    setColor("\033[1;96m");
    std::cout << "C";
    setColor("\033[1;94m");
    std::cout << "o";
    setColor("\033[1;95m");
    std::cout << "l";
    setColor("\033[1;97m");
    std::cout << "o";
    setColor("\033[1;91m");
    std::cout << "r";
    setColor("\033[1;93m");
    std::cout << "s";
    setColor("\033[1;32m");
    std::cout << "                    ║\n";

    std::cout << "║  ";
    setColor("\033[1;31m");
    std::cout << "3. Exit";
    setColor("\033[1;32m");
    std::cout << "                              ║\n";

    std::cout << "║                                       ║\n";
    std::cout << "╚═══════════════════════════════════════╝\n";

    resetColor();
    std::cout << "\nEnter your choice (1-3): ";
}

int MenuInterface::getUserChoice()
{
    int choice;
    while (true)
    {
        std::cin >> choice;
        if (std::cin.fail() || choice < 1 || choice > 3)
        {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            setColor("\033[1;31m");
            std::cout << "Invalid choice! Please enter 1, 2, or 3: ";
            resetColor();
        }
        else
        {
            std::cin.ignore(10000, '\n'); // clear input buffer
            return choice;
        }
    }
}

void MenuInterface::showGoodbye()
{
    clearScreen();
    setColor("\033[1;32m");
    std::cout << "Goodbye! Thanks for using Matrix Rain!";
    resetColor();
    std::cout << std::endl;
}

void MenuInterface::pressEnterToContinue()
{
    setColor("\033[1;36m");
    std::cout << "Press Enter to return to menu or Ctrl+C to exit...";
    resetColor();
    std::cin.get();
}