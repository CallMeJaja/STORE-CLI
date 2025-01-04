#pragma once
#include "iostream"

class MenuBase {
  protected:
    void clearScreen() { system("cls"); }
    void pause() { system("pause"); }
};