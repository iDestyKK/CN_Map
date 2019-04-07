/*
 * Example where a struct is the key to a CN_Map instance. This requires the
 * programmer to manually specify a comparison function so the CN_Map would
 * know how to sort the tree when comparing instances of "struct foo".
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../cn_cmp.h"
#include "../cn_map.h"

//A custom struct
struct foo {
	double bar;
	int id;
};

//Comparison function to handle sorting "foo" if it were a key
CNC_COMP compare_foo(void *lhs, void *rhs) {
	struct foo *a = lhs;
	struct foo *b = rhs;

	if (a->id < b->id)
		return CN_CMP_LESS;
	else
	if (a->id > b->id)
		return CN_CMP_GREATER;
	else
		return CN_CMP_EQUAL;
}

main() {
	CN_MAP map = cn_map_init(struct foo, int, compare_foo);

	//Make a key and value
	struct foo key;
	int        value;

	key.id  = 2;
	key.bar = 2.5f;
	value = 2;

	//Insert into the map
	cn_map_insert(map, &key, &value);

	//Make some changes
	key.id  = 1;
	key.bar = 19.2f;
	value   = 4;

	//Insert new struct into map
	cn_map_insert(map, &key, &value);

	//Iterate through the map and print out what's in it.
	CNM_ITERATOR it;
	
	//cn_map_traverse(map, &it) { ... }
	for (cn_map_begin(map, &it); !cn_map_at_end(map, &it); cn_map_next(map, &it)) {
		//Get the key
		struct foo *ptr = &cn_map_iterator_key(&it, struct foo);

		//Print struct key and integer value
		printf(
			"ID: %d / BAR: %lg / VALUE: %d\n",
			ptr->id,
			ptr->bar,
			cn_map_iterator_value(&it, int)
		);
	}

	cn_map_free(map);
}
