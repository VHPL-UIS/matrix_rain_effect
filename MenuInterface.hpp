#ifndef __MENU_INTERFACE_HPP__
#define __MENU_INTERFACE_HPP__

class MenuInterface
{
private:
    static void clearScreen();
    static void setColor(const char *colorCode);
    static void resetColor();

public:
    static void showMenu();
    static int getUserChoice();
    static void showGoodbye();
    static void pressEnterToContinue();
};

#endif // __MENU_INTERFACE_HPP__