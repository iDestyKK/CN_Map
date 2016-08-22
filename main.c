#include <stdio.h>
#include "cn_comp.h"
#include "cn_map.h"

main() {
	CN_MAP map = cn_map_init(int, int, cn_cmp_cstr);
	int a = 4,
	    b = 0;
	cn_map_insert(map, &a, &b);
	
	a = 3; b = 6;
	cn_map_insert(map, &a, &b);
	
	a = 2; b = 7;
	cn_map_insert(map, &a, &b);
	
	a = 1; b = 4;
	cn_map_insert(map, &a, &b);
	
	a = 0; b = 4;
	cn_map_insert(map, &a, &b);
	
	a = 5; b = 4;
	cn_map_insert(map, &a, &b);
	
	a = 6; b = 4;
	cn_map_insert(map, &a, &b);
	
	a = 7; b = 4;
	cn_map_insert(map, &a, &b);
	
	a = 8; b = 4;
	cn_map_insert(map, &a, &b);
	
	a = 11; b = 4;
	cn_map_insert(map, &a, &b);
	
	a = 10; b = 4;
	cn_map_insert(map, &a, &b);

	a = 9; b = 12;
	cn_map_insert(map, &a, &b);

	a = 13; b = 4;
	cn_map_insert(map, &a, &b);

	a = 12; b = 4;
	cn_map_insert(map, &a, &b);

	a = 17; b = 4;
	cn_map_insert(map, &a, &b);

	CNM_NODE* lol = cn_map_find(map, &a);
	
	b = 0;
	//printf("%d\n", cn_map_find_deref(map, &a, int));
	
	//__cn_map_print_tree(map);

	//__cn_map_nodes_adjust(map, lol);
	CNM_ITERATOR* ii = cn_map_begin(map);
	for (ii = cn_map_begin(map); ii->node != cn_map_end(map); ii = cn_map_next(map, ii)) {
		printf("%d\n", *(int*)ii->node->key);
	}
	
	cn_map_free(map);
}
