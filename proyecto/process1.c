#include "stdio.h"

void process1_main() {
    while (1) {
        for (char c = 'a'; c <= 'z'; c++) {
            PRINT("%c", c);
        }
    }
}
