#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../cn_cmp.h"
#include "../cn_map.h"

main() {
	CN_MAP map = cn_map_init(int, int, cn_cmp_int);

	int key   = 0;
	int value = 0;

	//Insert keys 0-11 (inclusively)
	while (key != 12) {
		cn_map_insert(map, &key, &value);

		key++;
		value++;
	}

	//Iterate through and print out all key/value pairs.
	CNM_ITERATOR it;

	cn_map_traverse(map, &it) {
		printf(
			"%d -> %d\n",
			cn_map_iterator_key  (&it, int),
			cn_map_iterator_value(&it, int)
		);
	}
}
