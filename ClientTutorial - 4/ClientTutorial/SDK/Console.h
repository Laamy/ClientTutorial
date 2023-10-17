#pragma once

#include <cstdio>

class Console {
public:
    inline static void CreateConsole(const char* title)
    {
        HWND consoleWnd = GetConsoleWindow();
        if (!consoleWnd)
        {
            AllocConsole();

            freopen_s(&f, "CONOUT$", "w", stdout);
            freopen_s(&f, "CONIN$", "r", stdin);
            SetConsoleTitleA(title);
        }
        else
        {
            ShowWindow(GetConsoleWindow(), SW_SHOW);
        }
    }

    inline static void CloseConsole()
    {
        ShowWindow(GetConsoleWindow(), SW_HIDE);
        ClearConsole();
    }

    inline static void ClearConsole()
    {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        DWORD count;
        COORD homeCoords = { 0, 0 };

        if (hConsole == INVALID_HANDLE_VALUE || !GetConsoleScreenBufferInfo(hConsole, &csbi)) {
            return;
        }

        int cells = csbi.dwSize.X * csbi.dwSize.Y;

        FillConsoleOutputCharacter(hConsole, ' ', cells, homeCoords, &count);
        SetConsoleCursorPosition(hConsole, homeCoords);
    }

private:
    static inline FILE* f; // Stream used to redirect stdout to the console
};