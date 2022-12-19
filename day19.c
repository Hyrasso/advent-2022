#include <stdio.h>
#include <stdlib.h>

#define N 32

typedef struct
{
    int n;
    int ore;
    int clay_ore;
    int obsidian_ore;
    int obsidian_clay;
    int geode_ore;
    int geode_obsidian;
} blueprint;

typedef struct {
    int ore;
    int ore_bot;
    int clay;
    int clay_bot;
    int obsidian;
    int obsidian_bot;
    int geode;
    int geode_bot;
} state;

static int best_so_far = 0;

int max_bt(blueprint b, state s, int m) {

    if (m == 0) {
        // if ((s.obsidian_bot > 0) && (s.obsidian == 14)) {
        //     fprintf(stderr, "%d: state %d (+%d) ore, %d (+%d) clay, %d (+%d) obs, %d (+%d) geode\n", m, s.ore, s.ore_bot, s.clay, s.clay_bot, s.obsidian, s.obsidian_bot, s.geode, s.geode_bot);
        // }
        best_so_far = (best_so_far > s.geode) ? best_so_far : s.geode;
        return s.geode;
    }

    int potential_best = s.geode;
    int inc = s.geode_bot;
    for (size_t i=0;i < m;i++) {
        potential_best += inc;
        inc++;
    }
    if (best_so_far > potential_best) {
        return s.geode;
    }


    // find possible bot to build
    // a single bot is built at once
    
    int best_geode_score = 0;
    state new_s = s;
    new_s.ore += s.ore_bot;
    new_s.clay += s.clay_bot;
    new_s.obsidian += s.obsidian_bot;
    new_s.geode += s.geode_bot;
    // fprintf(stderr, "%d: state %d (+%d) ore, %d (+%d) clay, %d (+%d) obs, %d (+%d) geode\n", m, new_s.ore, new_s.ore_bot, new_s.clay, new_s.clay_bot, new_s.obsidian, new_s.obsidian_bot, new_s.geode, new_s.geode_bot);

    // if possible to build a geode bot, try that first
    if ((s.ore >= b.geode_ore) && (s.obsidian >= b.geode_obsidian)) {
        new_s.ore -= b.geode_ore;
        new_s.obsidian -= b.geode_obsidian;
        new_s.geode_bot += 1;

        // fprintf(stderr, "First geode bot built at step %d\n", m);
        // fprintf(stderr, "%d: state %d (+%d) ore, %d (+%d) clay, %d (+%d) obs, %d (+%d) geode\n", m, new_s.ore, new_s.ore_bot, new_s.clay, new_s.clay_bot, new_s.obsidian, new_s.obsidian_bot, new_s.geode, new_s.geode_bot);
        int ngeode = max_bt(b, new_s, m - 1);
        best_geode_score = (ngeode > best_geode_score) ? ngeode : best_geode_score;

        // revert bot construction state
        new_s.ore += b.geode_ore;
        new_s.obsidian += b.geode_obsidian;
        new_s.geode_bot -= 1;
    }

    // build ore bot, unless we are are producing more ore than we can spend per minute
    if ((s.ore >= b.ore) && !((s.ore_bot >= b.clay_ore) && (s.ore_bot >= b.geode_ore) && (s.ore_bot >= b.obsidian_ore))) {
        new_s.ore -= b.ore;
        new_s.ore_bot += 1;
        int ngeode = max_bt(b, new_s, m - 1);
        best_geode_score = (ngeode > best_geode_score) ? ngeode : best_geode_score;

        // revert bot construction state
        new_s.ore_bot -= 1;
        new_s.ore += b.ore;
    }

    // clay bot cost ore
    if ((s.ore >= b.clay_ore) && (s.clay_bot < b.obsidian_clay)) {
        new_s.ore -= b.clay_ore;
        new_s.clay_bot += 1;

        int ngeode = max_bt(b, new_s, m - 1);
        best_geode_score = (ngeode > best_geode_score) ? ngeode : best_geode_score;

        // revert bot construction state
        new_s.ore += b.clay_ore;
        new_s.clay_bot -= 1;
    }

    if ((s.ore >= b.obsidian_ore) && (s.clay >= b.obsidian_clay) && (s.obsidian_bot < b.geode_obsidian)) {
        new_s.ore -= b.obsidian_ore;
        new_s.clay -= b.obsidian_clay;
        new_s.obsidian_bot += 1;

        int ngeode = max_bt(b, new_s, m - 1);
        best_geode_score = (ngeode > best_geode_score) ? ngeode : best_geode_score;

        // revert bot construction state
        new_s.ore += b.obsidian_ore;
        new_s.clay += b.obsidian_clay;
        new_s.obsidian_bot -= 1;
    }

    // no need to accumulate indefinitely
    // if we have too much resource, skip waiting
    // too much is if we have enough to build all (possible) bots

    // we have enough to build an ore or a clay bot, but cant build obsidian or geode, no need to accumulate more
    if ((s.clay_bot == 0) && ((s.ore >= b.ore) && (s.ore >= b.clay_ore))) {
        return best_geode_score;
    }
    // we have enough to build an ore, a clay or an obsidian bot, but cannot build geode bot yet
    if ((s.obsidian_bot == 0) && ((s.ore >= b.ore) && (s.ore >= b.clay_ore) && ((s.ore >= b.obsidian_ore) && (s.clay >= b.obsidian_clay)))) {
        return best_geode_score;
    }
    // we can build any bot
    if (((s.ore >= b.ore) && (s.ore >= b.clay_ore) && ((s.ore >= b.obsidian_ore) && (s.clay >= b.obsidian_clay)) && ((s.ore >= b.geode_ore) && (s.obsidian >= b.geode_obsidian)))) {
        return best_geode_score;
    }

    // ok to accumulate because we could build a new bot thanks to that
    int ngeode = max_bt(b, new_s, m - 1);
    best_geode_score = (ngeode > best_geode_score) ? ngeode : best_geode_score;

    return best_geode_score;
}

int max_geode(blueprint b, size_t stop) {
    best_so_far = 0;
    state s = {0};
    s.ore_bot = 1;
    int minute = stop;

    return max_bt(b, s, minute);
}


int main(int argc, char* argv[]) {
    FILE* file = fopen("input/day19.txt", "r");
    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }

    blueprint blueprints[N] = {0};
    size_t bcount = 0;

    while (fscanf(file, "Blueprint %d: Each ore robot costs %d ore. Each clay robot costs %d ore. Each obsidian robot costs %d ore and %d clay. Each geode robot costs %d ore and %d obsidian. ",
                        &blueprints[bcount].n,
                        &blueprints[bcount].ore,
                        &blueprints[bcount].clay_ore,
                        &blueprints[bcount].obsidian_ore,
                        &blueprints[bcount].obsidian_clay,
                        &blueprints[bcount].geode_ore,
                        &blueprints[bcount].geode_obsidian
                        ) == 7)

    {
        bcount++;
    }

    int total_quality = 0;
    int part_2[3] = {1, 1, 1};
    for (size_t i=0;i < bcount;i++) {
        blueprint b = blueprints[i];
        fprintf(stderr, "B %d: ore cost %d clay cost %d obs cost %d %d geode cost %d %d\n", b.n, b.ore, b.clay_ore, b.obsidian_ore, b.obsidian_clay, b.geode_ore, b.geode_obsidian);

        int quality = max_geode(b, 24);
        fprintf(stderr, "Q: %d\n", quality);

        if (i < 3) {
            part_2[i] = max_geode(b, 32);
            fprintf(stderr, "Q: %d\n", part_2[i]);
        }


        total_quality += quality * b.n;

    }

    printf("Total quality %d\n", total_quality);
    // 1136 too low
    printf("Quality %d\n", part_2[0]*part_2[1]*part_2[2]);

    return 0;
}