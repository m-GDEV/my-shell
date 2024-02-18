make clean && make && valgrind --leak-check=full --track-origins=yes --show-leak-kinds=all ./bin/shell
