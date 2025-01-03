#pragma once
#include "iostream"
#include "windows.h"

class MenuBase {
  protected:
    void clearScreen() { system("cls"); }
    void pause() { system("pause"); }
};