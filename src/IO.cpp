//
// Created by Vaclav Volhejn on 2019-04-24.
//

#include "IO.h"


#if defined(_WIN32) || defined(_WIN64)
#include <cstdio>
#include <conio.h>

void IO::init() {
}

char IO::keyPressed() {
    return !kbhit() ? 0 : getch();
}

void IO::println(const std::string &s) {
    printf("%s", (s + "\n").c_str());
}

void IO::print(const std::string &s) {
    printf("%s", s.c_str());
}

#else

#include <ncurses.h>
#include <cstdio>
#include <iostream>
#include <locale.h>

void IO::init() {
    initscr();
    cbreak();
    noecho();
    scrollok(stdscr, TRUE);
    nodelay(stdscr, TRUE);
    setlocale(LC_ALL, "");
}

char IO::keyPressed() {
    const int c = getch();
    return (c == -1) ? '\0' : char(c);
}

void IO::println(const std::string &s) {
    printw("%s\n", s.c_str());
}

void IO::print(const std::string &s) {
    printw("%s", s.c_str());
}

#endif
