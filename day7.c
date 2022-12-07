#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#define N 64

#define MAX_CHILDREN 16


typedef struct Node
{
    char dir_name[N];
    struct Node* parent;
    size_t file_size;
    int total_file_size;
    size_t n_children;
    struct Node** children;
} Node;

Node* create_node(const char* dir_name) {
    fprintf(stderr, "create_node: new node with name %s\n", dir_name);
    Node* node = calloc(1, sizeof(Node));
    if (node == NULL)
        return NULL;
    // copy dir_name
    for (int i=0;dir_name[i] && (i < N);i++) {
        node->dir_name[i] = dir_name[i];
    }
    
    node->children = calloc(MAX_CHILDREN, sizeof(Node*));
    if (node->children == NULL) {
        free(node);
        return NULL;
    }

    return node;
}

int get_size(Node* node, int* sum) {
    fprintf(stderr, "Computing size of %s\n", node->dir_name);
    node->total_file_size = node->file_size;
    for (size_t i=0;i<node->n_children;i++) {
        node->total_file_size += get_size(node->children[i], sum);
    }
    fprintf(stderr, "Size of %s is %d\n", node->dir_name, node->total_file_size);
    if (node->total_file_size <= 100000) {
        *sum = *sum + node->total_file_size;
    }
    return node->total_file_size;
}

int strcomp(char* a, char *b) {
    size_t i=0;
    while (a[i] && b[i])
    {
        int diff = a[i] - b[i];
        if (diff) {
            return diff;
        }
        i++;
    }
    if (a[i] == b[i])
        return 0;
    if (a[i] == 0)
        return b[i];
    return a[i];
}

void size_to_del(Node* current, int* size, int target) {
    if (current->total_file_size > target && current->total_file_size < *size) {
        *size = current->total_file_size;
    }
    for (size_t i=0;i < current->n_children;i++) {
        size_to_del(current->children[i], size, target);
    }
}

int main(int argc, char* argv[]) {
    FILE* file = fopen("input/day7.txt", "r");
    if (file == NULL) {
        perror("Error with file");
        return 1;
    }
    
    char line[N];

    Node* current = NULL;
    char dir_name[N];
    char file_name[N];
    int file_size;

    while (fgets(line, N, file))
    {
        fprintf(stderr, "\n");
        fprintf(stderr, "cmd: %s", line);
        if (current != NULL) 
            fprintf(stderr, "current dir: %s with size %zu\n", current->dir_name, current->file_size);
        switch (line[0])
        {
        case '$':
            // new command
            switch (line[2])
            {
            case 'c':
                // change current dir
                // cd .. -> parent dir
                if (line[5] == '.') {
                    fprintf(stderr, "Setting current to parent\n");
                    current = current->parent;
                } else {
                    // find the children with given name
                    sscanf(line, "$ cd %s", dir_name);
                    // intialize for /
                    if (current == NULL) {
                        current = create_node(dir_name);
                        continue;
                    }
                    for (size_t i=0;i < current->n_children;i++) {
                        if (strcomp(dir_name, current->children[i]->dir_name) == 0) {
                            current = current->children[i];
                            break;
                        }
                    }
                }
                break;
            
            case 'l':
                //ls nothing to do
                break;
            default:
                printf("Unknown command %s\n", line);
                exit(1);
            }
            
            break;
        case 'd':
            // adding a dir to current node
            sscanf(line, "dir %s", dir_name);
            Node* new_node = create_node(dir_name);
            new_node->parent = current;
            if (new_node == NULL) {
                fprintf(stderr, "New node alloc failed\n");
                return 1;
            }
            current->children[current->n_children] = new_node;
            current->n_children++;
            break;
        default:
            // adding a file to current dir
            assert(sscanf(line, "%d %s", &file_size, file_name) == 2);
            fprintf(stderr, "Adding file with size %d to current node", file_size);
            // size_t strlen = 0;
            // for (;file_name[strlen];strlen++);
            current->file_size += file_size;
            break;
        }
    }
    
    // get back to root
    while (current->parent != NULL)
    {
        current = current->parent;
    }
    

    // count total size
    int total_sum = 0;
    get_size(current, &total_sum);

    printf("Sum: %d\n", total_sum);

    int size = current->total_file_size;
    int target = size - 30000000;
    fprintf(stderr, "Target %d\n", target);
    size_to_del(current, &size, target);

    printf("To del: %d\n", size);
    // 11347870 too high
    return 0;
}