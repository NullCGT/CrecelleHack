#ifndef HEATMAP_H
#define HEATMAP_H

struct p_node {
    int heat;
    int x;
    int y;
};

/* priority queue helpers */
#define left_child(i) ((i * 2) + 1)
#define right_child(i) ((i * 2) + 2)
#define pq_parent(i) ((i - 1) / 2)

/* dijkstra algorithm constants */
#define MAX_HEAT 999
#define IMPASSABLE MAX_HEAT + 1

#endif