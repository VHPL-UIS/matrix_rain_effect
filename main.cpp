#include "MatrixRain.hpp"
#include "MenuInterface.hpp"
#include <iostream>
#include <stdexcept>

int main()
{
    try
    {
        while (true)
        {
            MenuInterface::showMenu();
            int choice = MenuInterface::getUserChoice();

            switch (choice)
            {
            case 1:
            {
                DynamicMatrixRain matrix(false); // Classic green matrix
                matrix.run();
                break;
            }
            case 2:
            {
                DynamicMatrixRain matrix(true); // Rainbow colors
                matrix.run();
                break;
            }
            case 3:
            {
                MenuInterface::showGoodbye();
                return 0;
            }
            default:
            {
                std::cerr << "Unexpected choice: " << choice << std::endl;
                break;
            }
            }

            // Ask if user wants to continue
            MenuInterface::pressEnterToContinue();
        }
    }
    catch (const std::exception &e)
    {
        std::cout << "\033[?25h\033[0m\033[2J\033[H"; // Cleanup terminal
        std::cerr << "Fatal error: " << e.what() << std::endl;
        return 1;
    }
    catch (...)
    {
        std::cout << "\033[?25h\033[0m\033[2J\033[H"; // Cleanup terminal
        std::cerr << "Unknown error occurred!" << std::endl;
        return 1;
    }

    return 0;
}