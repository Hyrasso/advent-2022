#include <stdio.h>
#include <stdlib.h>

#define N 5001

typedef struct node {
    struct node* prev;
    struct node* next;
    long val;
} node;

void debug_ll(node** nodes, size_t nn) {
    for (size_t i=0;i < nn;i++) {
        fprintf(stderr, "(%ld) - %ld - (%ld)\n", nodes[i]->prev->val, nodes[i]->val, nodes[i]->next->val);
    }
}

void l_move(node* n, size_t nn) {

    if (n->val == 0) {
        return;
    } 

    node* new_prev = n, *new_next = n;
    // either do steps = val % node_count - 1
    // or do not modulo and first remove the current element from the list to skip itself
    if (n->val > 0) {
        for (size_t i=0;i<(n->val % nn);i++) {
            new_prev = new_prev->next;
        }
        new_next = new_prev->next;
    } else {
        // one off when moving backward
        for (size_t i=0;i<(-n->val % nn);i++) {
            new_next = new_next->prev;
        }
        new_prev = new_next->prev;
    }


    // remove current node
    n->prev->next = n->next;
    n->next->prev = n->prev;

    n->prev = new_prev;
    n->next = new_next;

    new_prev->next = n;
    new_next->prev = n;
}

int main(int argc, char* argv[]) {
    FILE* file = fopen("input/day20.txt", "r");
    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }

    long val;

    node* nodes[N];
    node* lstart = NULL;
    size_t node_count = 0;

    while (fscanf(file, "%ld ", &val) == 1)
    {
        node* new_node = malloc(sizeof(node));
        val = val * 811589153;
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

    // node* n = nodes[0];
    // do {
    //     fprintf(stderr, "%d ", n->val);
    //     n = n->next;
    // }
    // while (n != nodes[0]);
    // fprintf(stderr, "\n");
     
    for (size_t k=0;k < 10;k++) {
    for (size_t i=0;i<node_count;i++) {
        // check linked list state
        // debug_ll(nodes, node_count);
        l_move(nodes[i], node_count - 1); // -1 very important to not count passing itself as moving one forward when steps > node_count
        // debug_ll(nodes, node_count);
        // check linked list state
        // node* n = nodes[0];
        // do {
        //     fprintf(stderr, "%d ", n->val);
        //     n = n->next;
        // } while (n != nodes[0]);
        // fprintf(stderr, "\n");
    }
    }


    // find elem 1000
    long dxs[3] = {1000, 2000, 3000};
    long sum = 0;
    for (size_t k=0;k < 3;k++) {
        long dx = dxs[k] % node_count;
        node* start = lstart->next;
        for (size_t i=1;i<dx;i++) {
            start = start->next;
        }
        printf("%ld (%ld): %ld\n", k+1, dx, start->val);
        sum += start->val;
    }

    // 9663 too low
    // part 2 1186475640 toolow
    printf("Sum %ld\n", sum);

    return 0;
}