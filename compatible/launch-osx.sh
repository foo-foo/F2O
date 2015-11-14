#!/bin/sh

cd F2O && g++ -W -Wall -Wextra -Werror -o F2O Srcs/*.cpp -L./SFML-2.3.2-osx-clang-universal/lib/ -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio && export LD_LIBRARY_PATH="./SFML-2.3.2-osx-clang-universal/lib/" && ./F2O && cd ..;