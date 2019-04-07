#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../cn_cmp.h"
#include "../cn_map.h"

main() {
	CN_MAP map = cn_map_init(int, int, cn_cmp_int);
	int key;
	int value;

	int i;
	for (i = 0; i <= 50; i++) {
		//Generate a key between 0 and 99 (Inclusive)
		key = rand() % 100;

		//If key already is in the map, don't insert.
		CNM_ITERATOR it;
		cn_map_find(map, &it, &key);
		if (!cn_map_at_end(map, &it))
			continue;

		printf("Insert %d\n", key);

		//Set some value and insert into the CN_Map
		value = rand() % 1000;
		cn_map_insert(map, &key, &value);
	}

	//Iterate through the entire map and print out key/value pairs
	CNM_ITERATOR it;
	for (cn_map_begin(map, &it); !cn_map_at_end(map, &it); cn_map_next(map, &it)) {
		printf(
			"%d - %d\n",
			cn_map_iterator_key  (&it, int), /* Grab Key   */
			cn_map_iterator_value(&it, int)  /* Grab Value */
		);
	}

	//Now go in reverse
	printf("\nNow let's go in reverse:\n");
	for (cn_map_rbegin(map, &it); !cn_map_at_rend(map, &it); cn_map_prev(map, &it)) {
		printf(
			"%d - %d\n",
			cn_map_iterator_key  (&it, int),  /* Grab Key   */
			cn_map_iterator_value(&it, int)   /* Grab Value */
		);
	}

	//Cleanup
	cn_map_free(map);
}
