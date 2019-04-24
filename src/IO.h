//
// Created by Vaclav Volhejn on 2019-04-24.
//

#ifndef JWALK_IO_H
#define JWALK_IO_H

#include <string>

class IO {
public:
    static void init();

    // Returns the pressed key, or '\0' if no key was pressed.
    static char keyPressed();

    static void println(const std::string& s);

    static void print(const std::string& s);
};


#endif //JWALK_IO_H
