#include "universal.h"
int isCharInArray(char ch, const char *characters)
{
    // Procházíme všechny znaky v poli, dokud nenajdeme shodu
    while (*characters != '\0') // Pokračuj dokud není konec pole
    {
        if (*characters == ch) // Pokud je aktuální znak shodný s hledaným
        {
            return 0; // Nalezeno
        }
        characters++; // Přejdeme na další znak
    }
    return 1; // Znak nebyl nalezen
}