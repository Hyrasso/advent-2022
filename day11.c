#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define PART 2
#define NMONKEYS 8
#if PART == 1
#define NROUND 20
#else
#define NROUND 10000
#endif

typedef struct
{
    long* items;
    size_t stack_capacity;
    size_t nitems;

    int div;
    size_t next_true;
    size_t next_false;
    int update_factor;
    char update_op;
} monkey;

void monkey_push(monkey* m, long val) {
    if (m->items == NULL) {
        m->items = calloc(16, sizeof(*(m->items)));
        if (m->items == NULL) {
            exit(-1);
        }
        m->stack_capacity = 16;
    }

    if (m->stack_capacity == m->nitems) {
        long* new_stack = realloc(m->items, (sizeof(*(m->items))) * (m->stack_capacity << 1));
        if (new_stack == NULL) {
            exit(-1);
        }
        m->items = new_stack;
        m->stack_capacity = m->stack_capacity << 1;
    }
    m->items[m->nitems] = val;
    m->nitems = m->nitems + 1;
}

void monkey_debug(monkey* monkeys, int verbose) {
    fprintf(stderr, "Monkey debug print:\n");
    for (size_t i=0;i<NMONKEYS;i++) {
        fprintf(stderr, "Monkey %zu (stack size %zu)\n", i, monkeys[i].nitems);
        for (size_t j=0;j<monkeys[i].nitems;j++) {
            fprintf(stderr, "%ld, ", monkeys[i].items[j]);
        }
        fprintf(stderr, "\n");
        if (verbose > 1) {
            fprintf(stderr, "Total capacity: %zu\n", monkeys[i].stack_capacity);
            fprintf(stderr, "Op %c %d, test %d, true %zu, false %zu\n", monkeys[i].update_op, monkeys[i].update_factor, monkeys[i].div, monkeys[i].next_true, monkeys[i].next_false);
        }
    }
}

monkey* parse(FILE* file) {
    int val;
    char op;
    char *line_buffer = calloc(64, sizeof(char));
    monkey* monkeys = calloc(NMONKEYS, sizeof(monkey));
    if (monkeys == NULL) {
        exit(-1);
    }
    for (size_t i=0;i<NMONKEYS;i++) {
        assert (fscanf(file, "Monkey %d\n", &val) == 1);
        assert (val == i);

        assert (fseek(file, 18, SEEK_CUR) == 0);

        fgets(line_buffer, 64, file);
        // fprintf(stderr, "init vals: %s", line_buffer);
        // quite importnat not to increase line_buffer pointer
        char* val_start = line_buffer;
        while (1)
        {
            // skip ','
            val_start = val_start + 1;
            int v = strtol(val_start, &val_start, 10);
            // fprintf(stderr, "%d\n", v);
            monkey_push(monkeys + i, v);
            if (val_start[0] == '\n') {
                break;
            }
        }

        assert (fscanf(file, " Operation: new = old %c ", &op) == 1);
        monkeys[i].update_op = op;

        fgets(line_buffer, 64, file);
        if (line_buffer[0] == 'o') {
            monkeys[i].update_factor = 0;
        } else {
            sscanf(line_buffer, "%d", &val);
            monkeys[i].update_factor = val;
        }

        assert (fscanf(file, " Test: divisible by %d ", &val) == 1);
        monkeys[i].div = val;
        assert (fscanf(file, " If true: throw to monkey %d ", &val) == 1);
        monkeys[i].next_true = val;
        assert (fscanf(file, " If false: throw to monkey %d ", &val) == 1);
        monkeys[i].next_false = val;

    }
    return monkeys;
}

int main(int argc, char* argv[]) {
    FILE* file = fopen("input/day11.txt", "r");
    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }

    monkey* monkeys;
    monkeys = parse(file);
    int ninspect[NMONKEYS] = {0};

    // monkey_debug(monkeys, 1);

    long max_mod = 1;
    for (size_t i=0;i<NMONKEYS;i++) {
        max_mod *= monkeys[i].div;
    }
    // fprintf(stderr, "common d %ld\n", max_mod);

    for (size_t round=0;round < NROUND;round++) {
        for (size_t monkey_idx=0;monkey_idx < NMONKEYS;monkey_idx++) {

            monkey current_monkey = monkeys[monkey_idx];

            for (size_t item_idx=0;item_idx<current_monkey.nitems;item_idx++) {
                long worry_level = current_monkey.items[item_idx];

                if (current_monkey.update_op == '*') {
                    worry_level = worry_level * ((current_monkey.update_factor != 0) ? current_monkey.update_factor : worry_level);
                } else {
                    worry_level = worry_level + current_monkey.update_factor;
                }
                ninspect[monkey_idx] = ninspect[monkey_idx] + 1;
                #if PART == 1
                worry_level = worry_level / 3;
                #endif
                worry_level = worry_level % max_mod;

                size_t next_idx = 0;
                if (worry_level % current_monkey.div == 0) {
                    next_idx = current_monkey.next_true;
                } else {
                    next_idx = current_monkey.next_false;
                }
                monkey_push(monkeys + next_idx, worry_level);
            }
            monkeys[monkey_idx].nitems = 0;
        }
        // fprintf(stderr, "Round %zu\n", round);
        // monkey_debug(monkeys, 1);
    }
    // for (size_t i=0;i<NMONKEYS;i++) {
    //     printf("%d ", ninspect[i]);
    // }
    // printf("\n");

    long max = 0;
    for (size_t i=0;i<NMONKEYS;i++) {
        max = (ninspect[i] > max) ? ninspect[i] : max;
    }
    long max2 = 0;
    for (size_t i=0;i<NMONKEYS;i++) {
        max2 = (ninspect[i] > max2 && ninspect[i] != max) ? ninspect[i] : max2;
    }
    printf("Business score %ld\n", max * max2);

    return 0;
}