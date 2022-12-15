#include <stdlib.h>
#include <stdio.h>

#define N 128

#define LS (1 << 16)

typedef struct
{
    int depth;
    int value;
} val;


typedef struct {
    size_t len;
    val values[N];
} signal;

signal* signal_parse(char* s) {
    signal* sig = malloc(sizeof(signal));
    if (sig == NULL) {
        exit(1);
    }

    int depth = 0;
    int value;
    int val_flag = LS;
    while(*s && (*s != '\n')) {
        switch (*s)
        {
        case '[':
            depth++;
            val_flag = LS;
            // creates 'dummy' value for empty list
            if (s[1] == ']') {
                sig->values[sig->len++] = (val){.depth=depth, .value=-1};
            }
            break;
        case ']':
            depth--;
            break;
        case ',':
            val_flag = 0;
            break;
        default :
            value = strtol(s, &s, 10);
            s--;
            sig->values[sig->len++] = (val){.depth=depth, .value=value | val_flag};
        }
        s += 1;
    }

    return sig;
}

int signal_comp(signal* a, signal *b) {
    // if a is over and not b: -1 (list b is longer)
    // if b is over and not a: 1 (list a is longer)
    // if same depth and same val
    // move one step in a and b
    // if different depth and not list start return depth a - depth b
    // if different depth and list start, set depth to the highest between a and b
    // with same depth
    //  if val a == val b: next
    //  if a is LS and b is not: return -1 (list b is longer)
    //  if b is LS and a is not: return 1 (list b is longer)
    //  return a - b
    return 0;
}

int main(int argc, char* argv[]) {
    FILE* file = fopen("input/day13-sample.txt", "r");
    if (file == NULL) {
        return -1;
    }

    char line1[N];
    char line2[N];

    int score = 0;
    // list of pointers to signal
    signal** signals = malloc(0);
    size_t nsignal = 0;

    while (1)
    {
        fgets(line1, N, file);
        fgets(line2, N, file);
        nsignal += 2;
        signals = realloc(signals, nsignal * sizeof(*signals));

        fprintf(stderr, "%s", line1);
        signals[nsignal-2] = signal_parse(line1);
        for (size_t i=0;i < signals[nsignal - 2]->len;i++) {
            fprintf(stderr, "%d:%d ", signals[nsignal - 2]->values[i].depth, signals[nsignal - 2]->values[i].value);
        }
        fprintf(stderr, "\n");

        fprintf(stderr, "%s", line2);
        signals[nsignal-1] = signal_parse(line2);
        for (size_t i=0;i < signals[nsignal - 1]->len;i++) {
            fprintf(stderr, "%d:%d ", signals[nsignal - 1]->values[i].depth, signals[nsignal - 1]->values[i].value);
        }
        fprintf(stderr, "\n");

        if (signal_comp(signals[nsignal - 2], signals[nsignal - 1]) < 0) {
            score += nsignal / 2 + 1;
        }

        //empty line
        if (fgets(line1, N, file) == NULL) {
            break;
        }
    }
    
    printf("Score: %d\n", score);
    return 0;
}