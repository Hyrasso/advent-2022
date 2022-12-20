#include <stdio.h>
#include <stdlib.h>

#define N 5001

typedef struct node {
    struct node* prev;
    struct node* next;
    int val;
} node;

void debug_ll(node** nodes, size_t nn) {
    for (size_t i=0;i < nn;i++) {
        fprintf(stderr, "(%d) - %d - (%d)\n", nodes[i]->prev->val, nodes[i]->val, nodes[i]->next->val);
    }
}

void l_move(node* n) {
    node* new_pos = n;
    if (n->val > 0) {
        for (size_t i=0;i<n->val;i++) {
            new_pos = new_pos->next;
        }
    } else {
        for (size_t i=0;i<=-n->val;i++) {
            new_pos = new_pos->prev;
        }
    }

    if (new_pos == n) {
        return;
    }
    // fprintf(stderr, "Setting %d in place of %d\n", n->val, new_pos->val);
    // remove current node
    // fprintf(stderr, "Setting prev (%d -> %d) to n next %d\n", n->prev->val, n->prev->next->val, n->next->val);
    n->prev->next = n->next;
    // fprintf(stderr, "Setting next (%d -> %d) to n prev %d\n", n->next->val, n->next->prev->val, n->prev->val);
    n->next->prev = n->prev;

    // set n after new_pos
    // fprintf(stderr, "Setting n prev (%d) to n %d\n", n->prev->val, new_pos->val);
    n->prev = new_pos;
    // fprintf(stderr, "Setting n next (%d) to n %d\n", n->next->val, new_pos->next->val);
    n->next = new_pos->next;

    // set new_pos before n
    // fprintf(stderr, "Setting new pos (%d) next (%d) next to n %d\n", new_pos->val, new_pos->next->val, n->val);
    new_pos->next->prev = n;
    new_pos->next = n;
}

int main(int argc, char* argv[]) {
    FILE* file = fopen("input/day20.txt", "r");
    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }

    int val;

    node* nodes[N];
    node* lstart;
    size_t node_count = 0;

    while (fscanf(file, "%d\n", &val) == 1)
    {
        node* new_node = malloc(sizeof(node));
        if (val == 0) {
            lstart = new_node;
        }
        new_node->val = val;
        if (node_count > 0) {
            new_node->prev = nodes[node_count - 1];
            nodes[node_count - 1]->next = new_node;
        }
        nodes[node_count++] = new_node;
    }
    nodes[0]->prev = nodes[node_count - 1];
    nodes[node_count - 1]->next = nodes[0];

    node* n = nodes[0];
    do {
        fprintf(stderr, "%d ", n->val);
        n = n->next;
    }
    while (n != nodes[0]);
    fprintf(stderr, "\n");
     
    for (size_t i=0;i<node_count;i++) {
        // check linked list state
        // debug_ll(nodes, node_count);
        l_move(nodes[i]);
        // debug_ll(nodes, node_count);
        // check linked list state
        // node* n = nodes[0];
        // do {
        //     fprintf(stderr, "%d ", n->val);
        //     n = n->next;
        // }
        // while (n != nodes[0]);
        // fprintf(stderr, "\n");
    }

    // find elem 1000
    int dx = 1000 % node_count;
    int dxs[3] = {1000, 2000, 3000};
    int sum = 0;
    for (size_t k=0;k < 3;k++) {
        int dx = dxs[k] % node_count;
        node* start = lstart->next;
        for (size_t i=1;i<dx;i++) {
            start = start->next;
        }
        printf("%ld: %d\n", k+1, start->val);
        sum += start->val;
    }

    // 9663 too low
    printf("Sum %d\n", sum);

    return 0;
}