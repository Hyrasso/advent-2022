#include <stdlib.h>
#include <stdio.h>

#define N 128

int out_of_order(char* a, char* b) {
    if (a[0] == '\0' && b[0] == '\0') {
        return 0;
    } else if (a[0] == '\0' || b[0] == '\0') {
        exit(-2);
    }
    // enter list
    if (a[0] == b[0] && a[0] == '[') {
        // while the next value is not ']' for both
        while (a[0] != ']' && b[0] != ']')
        {
            int r = out_of_order(a+1, b+1);
            if (r) {
                return r;
            }
        }
        if (!(a[0] == b[0] && a[0] == ']')) {
            exit(-1);
        }
        a++;b++;
        // rest of the messages
        return out_of_order(a, b);
    }
    // b as list
    if (a[0] == '[') {
        // check the case 4, [[4]]
        // while a[0] == '['
        int r = out_of_order(++a, b);
        if (r) {
            return r;
        }
        if (!(a[0] == ']')) {
            exit(-3);
        }
        a++;
        return out_of_order(a, b);
    }
    // a as list
    if (b[0] == '[') {
        out_of_order(a, ++b);
    }
    // compare value and return
    // skip comma if present
}

int main(int argc, char* argv[]) {
    FILE* file = fopen("input/day13-sample.txt", "r");
    if (file == NULL) {
        return -1;
    }

    char line1[N];
    char line2[N];

    int score = 0;

    while (1)
    {
        fgets(line1, N, file);
        fgets(line2, N, file);

        fprintf(stderr, "%s", line1);
        fprintf(stderr, "%s", line2);

        score += out_of_order(line1, line2);

        //empty line
        if (fgets(line1, N, file) == NULL) {
            break;
        }
    }
    

    return 0;
}