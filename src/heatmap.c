#include "hack.h"
#include "heatmap.h"

staticfn void pq_swap(struct p_node *, struct p_node *);
staticfn void pq_push(int heat, int x, int y);
staticfn struct p_node pq_pop(void);
staticfn void trash_heap(void);
staticfn void create_heatmap(int);
staticfn int bloodthirst(struct monst*);
staticfn int water_love(struct permonst *);

#define HMAP(id, nam, val) { nam, val }
struct heatmap all_heatmaps[] = { HMAP_LIST };
#undef HMAP
#undef HMAP_LIST

/* In theory based on https://www.geeksforgeeks.org/priority-queue-using-binary-heap/ */

static struct p_node heap[ROWNO * COLNO + 1];
static boolean visited[COLNO][ROWNO] = { 0 };
static int heap_size;
static coord dirs[8] = { {0, -1}, {0, 1}, {-1, 0}, {1, 0},
                         {1, 1}, {-1, 1}, {-1, -1}, {1, -1}};

/* swap nodes on the priority queue */
staticfn
void pq_swap(struct p_node *a, struct p_node *b) {
    struct p_node temp = *a;
    *a = *b;
    *b = temp;
}
 
/* push a node onto the priority queue */
staticfn
void pq_push(int heat, int x, int y) {
    heap_size++;
    int size = heap_size;

    heap[size].heat = heat;
    heap[size].x = x;
    heap[size].y = y;
    /* Heapify up */
    while (heap[size].heat < heap[pq_parent(size)].heat) {
        pq_swap(&(heap[size]), &(heap[pq_parent(size)]));
        size = pq_parent(size);
    }
}
 
/* pop a node from the priority queue */
staticfn
struct p_node pq_pop(void) {
    int max_index, max_heat, l, r;
    int index = 0;

    struct p_node node = heap[0];
    heap[0] = heap[heap_size];
    heap_size--;
    /* Heapify down */
    while (1) {
        max_index = index;
        max_heat = heap[index].heat;
        l = left_child(index);
        r = right_child(index);

        if (l <= heap_size) {
            if (heap[l].heat < max_heat)
                max_index = l;
        }
        if (r <= heap_size) {
            if (heap[r].heat < max_heat)
                max_index = r;
        }

        if (index != max_index) {
            pq_swap(&(heap[index]), &(heap[max_index]));
            index = max_index;
        } else {
            break;
        }
    }

    return node;
}

/* Clears the heap and visited queues */
staticfn
void trash_heap(void) {
    memset(&visited, 0, sizeof(visited));
    memset(&heap, 0, sizeof(heap));
    heap_size = -1;
}

/* Use Dijkstra algorithm to creat a heatmap. At a later point we 
   can pass in which map we want to create. */
staticfn 
void create_heatmap(int hm_index) {
    struct p_node cur;
    int nx, ny;
    short *n_heat;
    int cost = 1; /* we can add a cost to the map later*/

    //pline("Updated heatmap: %s", all_heatmaps[hm_index].name);
    trash_heap();
    /* Populate heap */
    /* We can actually populate the heap as we set up the heatmap,
       but only if we are working with a single heatmap. */
    for (int y = 0; y < ROWNO; y++) {
        for (int x = 0; x < COLNO; x++) {
            int val = svl.level.heatmap[hm_index][x][y];
            if (val < MAX_HEAT)
                pq_push(val, x, y);
        }
    }
    /* Dijkstra Algorithm */
    while (heap_size >= 0) {
        cur = pq_pop();
        for (int i = 0; i < 8; i++) {
            /* Loop through neighbors of cur */
            nx = cur.x + dirs[i].x;
            ny = cur.y + dirs[i].y;
            if (!isok(nx, ny) || visited[nx][ny]) continue;
            n_heat = &(svl.level.heatmap[hm_index][nx][ny]);
            visited[nx][ny] = 1;
            if (*n_heat == IMPASSABLE) continue;
            if (cur.heat + cost < *n_heat) {
                *n_heat = cur.heat + cost;
                pq_push(*n_heat, nx, ny);
            }
        }
    }
}

void do_heatmaps(short hm_bits) {
    int i;
    /* Setup */
    for (int y = 0; y < ROWNO; y++) {
        for (int x = 0; x < COLNO; x++) {
            for (i = 0; i < NUM_HEATMAPS; i++) {
                /* Do not update heatmaps that do not need updating.*/
                if (!(hm_bits & all_heatmaps[i].val))
                    continue;
                /* Update the heatmaps that do. */
                if (i == HM_BLOOD && (hm_bits & HM_NEED_BLOOD)
                            && has_coating(x, y, COAT_BLOOD)) {
                    svl.level.heatmap[i][x][y] = 1;
                } else if (!ACCESSIBLE(levl[x][y].typ)) {
                    svl.level.heatmap[i][x][y] = IMPASSABLE;
                } else {
                    svl.level.heatmap[i][x][y] = MAX_HEAT;
                }
            }
        }
    }
    /* Constant-time fixes */
    svl.level.heatmap[HM_PLAYER][u.ux][u.uy] = 1;
    /* Calculate each heatmap that is needed */
    for (i = 0; i < NUM_HEATMAPS; i++) {
        if (hm_bits & all_heatmaps[i].val)
            create_heatmap(i);
    }
    /* Flip the bits of the heatmaps that we updated. */
    svc.context.heatmap.hmflags &= ~(hm_bits);
}

/* check if we need to update the player heatmap. should
   need to be called once a turn, since on game load and
   restore we recalculate all heatmaps. */
boolean check_player_heatmap(void) {
    boolean ret = FALSE;
    if (svc.context.heatmap.old_cc.x != u.ux) {
        svc.context.heatmap.old_cc.x = u.ux;
        svc.context.heatmap.hmflags |= HM_NEED_PLAYER;
        ret = TRUE;
    }
    if (svc.context.heatmap.old_cc.y != u.uy) {
        svc.context.heatmap.old_cc.y = u.uy;
        svc.context.heatmap.hmflags |= HM_NEED_PLAYER;
        ret = TRUE;
    }
    return ret;
}

/* calculate the score of the tile using multiplicative
   Dijkstra maps, as done in Brogue. */
int
calculate_tile_score(struct monst *mtmp, int x, int y)
{
    int score;
    score = svl.level.heatmap[HM_PLAYER][x][y] * 100;
    score += (svl.level.heatmap[HM_BLOOD][x][y] * bloodthirst(mtmp));
    return score;
}

/* bloodthirst coefficient for various monsters */
staticfn int
bloodthirst(struct monst *mon)
{
    struct permonst *ptr = mon->data;
    switch(monsndx(ptr)) {
    case PM_BLOOD_IMP:
    case PM_CRIMSON_DEATH:
        return 100;
    case PM_GHOUL:
    case PM_VAMPIRE_BAT:
    case PM_VAMPIRE:
        return 80;
    case PM_VAMPIRE_LEADER:
    case PM_VAMPIRE_MAGE:
        return 70;
    case PM_VLAD_THE_IMPALER:
        return 60;
    case PM_JACKAL:
        return 20;
    default:
        break;
    }
    if (is_demon(ptr) || is_were(ptr))
        return 50;
    if (is_orc(ptr) || (is_animal(ptr) && carnivorous(ptr)))
        return 10;
    if (ptr->mlet == S_ANGEL)
        return -20;
    if (mon_aligntyp(mon) == A_LAWFUL)
        return -3;
    return 0;
}

/* coefficient for water-lovers (and haters) */
staticfn int
water_love(struct permonst *ptr)
{
    switch(monsndx(ptr)) {
    case PM_SODDEN_ONE:
        return 100;
    case PM_WATER_NYMPH:
    case PM_GREMLIN:
        return 95;
    case PM_WATER_MOCCASIN:
    case PM_SPANNER:
        return 80;
    case PM_RABID_RAT:
        return -100;
    default:
        break;
    }
    if (is_swimmer(ptr))
        return 20;
    return 0;
}