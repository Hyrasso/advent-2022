#include <stdio.h>
#include <stdlib.h>

#include "utils/dict.h"

typedef struct {
    char name[5];
    char op;
    long val;
    char op1[5];
    char op2[5];
} node_t;

size_t node_hash(const void* n) {
    const node_t* node = n;
    const char* key = node->name;
    size_t h = 2166136261UL;
    for(; *key; key++)
    {
        h ^= *key;
        h *= 16777619;
    }
    return h;
}

int node_cmp(const void* a, const void* b) {
    const node_t* node_a = a;
    const node_t* node_b = b;

    for (size_t i=0;i < 4;i++) {
        if (node_a->name[i] != node_b->name[i]) {
            return node_a->name[i] - node_b->name[i];
        }
    }
    return 0;
}

void node_print(void* n)  {
    node_t* node = n;
    if (node->op == 'c') {
        fprintf(stderr, "%s: %ld", node->name, node->val);
    } else {
        fprintf(stderr, "%s: %s %c %s", node->name, node->op1, node->op, node->op2);
    }
}

double compute_val(dict* nodes, node_t* at) {
    if (at->val >= 0 || at->op == 'c') {
        return at->val;
    }

    node_t query = {0};
    query.name[0] = at->op1[0];
    query.name[1] = at->op1[1];
    query.name[2] = at->op1[2];
    query.name[3] = at->op1[3];
    double val1 = compute_val(nodes, dict_get(nodes, &query));
    query.name[0] = at->op2[0];
    query.name[1] = at->op2[1];
    query.name[2] = at->op2[2];
    query.name[3] = at->op2[3];
    double val2 = compute_val(nodes, dict_get(nodes, &query));

    switch (at->op)
    {
    case '+':
        return val1 + val2;
        break;
    case '-':
        return val1 - val2;
        break;
    case '*':
        return val1 * val2;
        break;
    case '/':
        return val1 / val2;
        break;
    
    default:
        printf("Unknown op %c \n", at->op);
        exit(-1);
        break;
    }

}

int main(int argc, char* argv[]) {
    FILE* file = fopen("input/day21.txt", "r");
    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }

    char buffer[64];

    dict* nodes = dict_create(node_hash, node_cmp);
    
    while (fgets(buffer, 64, file) != NULL) {
        node_t* node = calloc(1, sizeof(node_t));
        node->name[0] = buffer[0];
        node->name[1] = buffer[1];
        node->name[2] = buffer[2];
        node->name[3] = buffer[3];

        if (buffer[6] < 'A') {
            // const value
            node->op = 'c';
            long val = strtod(buffer+6, NULL);
            node->val = val;
        } else {
            sscanf(buffer+6, "%s %c %s", node->op1, &node->op, node->op2);
            node->val = -1;
        }

        dict_insert(nodes, node);

    }
    dict_print(nodes, node_print);

    node_t query = {.name="humn", 0};
    // get humn node
    node_t* humn = dict_get(nodes, &query);
    query.name[0] = 'r';
    query.name[1] = 'o';
    query.name[2] = 'o';
    query.name[3] = 't';
    node_t* root = dict_get(nodes, &query);


    // 571353930 too low
    printf("Root val %lf\n", compute_val(nodes, root));

    query.name[0] = root->op1[0];
    query.name[1] = root->op1[1];
    query.name[2] = root->op1[2];
    query.name[3] = root->op1[3];
    node_t* root1 = dict_get(nodes, &query);
    query.name[0] = root->op2[0];
    query.name[1] = root->op2[1];
    query.name[2] = root->op2[2];
    query.name[3] = root->op2[3];
    node_t* root2 = dict_get(nodes, &query);
    printf("Root op 1: %f\n", compute_val(nodes, root1));
    printf("Root op 2: %f\n", compute_val(nodes, root2));
    humn->val = 0;
    double val_0 = compute_val(nodes, root1);
    printf("Root op 2 at 1: %f\n", val_0);
    // need a lot of 0 to get the right precision
    humn->val = 100000000;
    double val_10 = compute_val(nodes, root1);
    printf("Root op 2 at 10: %f\n", val_10);

    double target_val = compute_val(nodes, root2);

    double coef = (val_10 - val_0) / 100000000.0;
    // t = coef * x + val_0
    double x = (target_val - val_0) / coef;
    // not precise enough, finishing with a binary search
    printf("Coef %f, intercept %f\n", coef, x);


    // could probably be found by sweepinf for different order of magnitudes
    // or formally solving the polynomial -> turns out its not a polynomial, we can just compute exactly where to get the answer at
    // assuming linear with negative slope in between those
    long dir = (coef > 0) ? 1 : -1;
    long lower = x - dir * coef;
    long higher = x + dir * coef + 1;
    humn->val = lower;
    double higher_val = compute_val(nodes, root1);
    humn->val = higher;
    double lower_val = compute_val(nodes, root1);
    while (higher - lower > 1)
    {
        humn->val = (lower + higher) / 2; // can overflow
        fprintf(stderr, "Trying %ld ", humn->val);
        double val = compute_val(nodes, root1);
        fprintf(stderr, "Res %lf\n", val);
        if (dir * (val - target_val) < 0) {
            lower_val = val;
            lower = humn->val;
        } else if (dir * (val - target_val) > 0) {
            higher_val = val;
            higher = humn->val;
        } else if (abs(val - target_val) < .1) {
            printf("Hit %lf\n", humn->val);
            break;
        }
    }
    
    // humn->val = 3500000000000;
    // 3582317956030 do not work but 3582317956029 does, probably one gives not integer result....
    printf("Setting humn to: %ld\n", humn->val);
    printf("Root op 1: %lf\n", compute_val(nodes, root1));
    printf("Root op 2: %lf\n", compute_val(nodes, root2));


    return 0;
    
}