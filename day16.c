#include <stdio.h>
#include <stdlib.h>
#include <math.h>

//max number of nodes in the graph
#define N (26 * 26)

typedef struct node {
    int ncount;
    int flow;
    size_t neighbors[N];
} node;

size_t name_to_index(char* node_name, char (*node_names)[3]) {
    for (size_t j=0;j < N;j++) {
        if (node_names[j][0] == '\0') {
            node_names[j][0] = node_name[0];
            node_names[j][1] = node_name[1];
            return j;
        }
        if ((node_name[0] == node_names[j][0]) && (node_name[1] == node_names[j][1])) {
            return j;
        }
    }

}

int main(int argc, char* argv[]) {
    FILE* file = fopen("input/day16-sample.txt", "r");
    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }

    char line_buffer[64];
    char (*node_names)[3] = calloc(N, sizeof(char[3]));
    char node_name[3];
    int flow;

    size_t node_count = 0;

    node nodes[N] = {0};

    while (fscanf(file, "Valve %s has flow rate=%d; ", node_name, &flow) == 2)
    {
        size_t node_idx = name_to_index(node_name, node_names);
        nodes[node_idx].flow = flow;

        fgets(line_buffer, 64, file);
        // fprintf(stderr, "L %s", line_buffer);
        char *p = line_buffer + 21;
        // skip s of valves
        if (p[0] == 's') { p++; }
        p++;
        while (1) {
            node_name[0] = p[0];
            node_name[1] = p[1];
            size_t neighbor_index = name_to_index(node_name, node_names);
            // add neighbor
            nodes[node_idx].neighbors[nodes[node_idx].ncount++] = neighbor_index;

            if (p[2] == ',') {
                p = p + 4;
            } else {
                break;
            }
        }
        node_count++;
    }

    size_t max_to_open = 0;
    for (size_t i=0;i < node_count;i++) {
        if (nodes[i].ncount) {
            fprintf(stderr, "Node %s (%ld) with flow %d and %d neighbors ", node_names[i], i, nodes[i].flow, nodes[i].ncount);
            for (size_t j=0;j < nodes[i].ncount;j++)
                fprintf(stderr, "%s (%ld) ", node_names[nodes[i].neighbors[j]], nodes[i].neighbors[j]);
            fprintf(stderr, "\n");

            if (nodes[i].flow > 0) {
                max_to_open++;
            }
        }
    }

    
    // we do not have max_to_open state, but 2^max_to_open states to keep track of
    // lots of the states are not possible so use a hashmap to store them instead
    // and compute the graph by starting AA then check possible next steps, computing there score (max stuff for next possible with different possible parents)
    // , then moving to next step
    char* dp = calloc(30 * node_count * max_to_open, sizeof(size_t));
    // dp should actually be something like >>> (dict of opened configurations to score) dp[step][pos]
    // for each step, pos, look for possible previous pos and iterate over the valve configurations
    // might be possible to materialize it as 30 * node_count * max_to_open (2^16=65_536) * 2 bytes, allocate last dimension as needed

    fprintf(stderr, "Memo size 30*%ld = %ld with at most %zu valve to open\n", node_count, 30 * node_count, max_to_open);

    for (size_t step=1;step < 30;step++) {
        for (size_t current_node_idx=0;current_node_idx < node_count;current_node_idx++) {
            // get max score from possible previous steps
            // if already on same node previous step and current node not open, open it and increase score
            // step - 1 and nodes from which current_node is accessible
            // using only the current score and not taking into account which valve were previously opened valve is not an issue because
            // opening a valve now or later gives strictly less score than having it open before (is that true?)
            // (is there a case where the final score is less because we took the max score for any valve configuration?)
            // works if in a DAG, is it still a DAG if valve opening state is not kept? seems like no?




        }
    }


    return 0;
}