#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../cn_cmp.h"
#include "../cn_map.h"

/*
 * gen_string
 *
 * Description:
 *     Generates a random string of "size" and puts it into "buffer".
 */

void gen_string(char* buffer, size_t size) {
	unsigned int i, r;

	for (i = 0; i < size - 1; i++) {
		switch (rand() % 3) {
			case 0:
				//Lower-case
				buffer[i] = 'a' + (rand() % ('z' - 'a' + 1));
				break;
			case 1:
				//Upper-case
				buffer[i] = 'A' + (rand() % ('Z' - 'A' + 1));
				break;
			case 2:
				//Numerics
				buffer[i] = '0' + (rand() % ('9' - '0' + 1));
				break;
		}
	}

	//Null terminate
	buffer[size - 1] = 0;
}

/*
 * destruct_key
 *
 * Description:
 *     Custom Destructor to free the c-string upon node destruction. This demos
 *     that CN_Maps can take custom destructors to handle complex datatypes as
 *     keys or values.
 */

void destruct_key(CNM_NODE *node) {
	if (*(char**)node->key != NULL)
		free(*(char**)node->key);
}

main() {
	//Set up the CN_Map
	CN_MAP map = cn_map_init(char*, int, cn_cmp_cstr);

	//Give our CN_Map a custom destructor function to free the c-string upon
	//deletion.
	cn_map_set_func_destructor(map, destruct_key);

	char *key = (char *) malloc(20);
	int  value;

	int i;
	for (i = 0; i <= 100; i++) {
		//Generate a key of random characters
		gen_string(key, 19);

		//If key already is in the map, don't insert.
		CNM_ITERATOR it;
		cn_map_find(map, &it, &key);
		if (!cn_map_at_end(map, &it))
			continue;

		//Set some value and insert into the CN_Map
		value = rand() % 1000;
		char *tmp = strdup(key);
		printf("Inserting \"%s\"\n", tmp);
		cn_map_insert(map, &tmp, &value);
	}

	//We don't need the key anymore.
	free(key);

	//Iterate through the entire map and print out key/value pairs
	CNM_ITERATOR it;
	for (cn_map_begin(map, &it); !cn_map_at_end(map, &it); cn_map_next(map, &it)) {
		printf(
			"%s - %d\n",
			cn_map_iterator_key  (&it, char*),  /* Grab Key   */
			cn_map_iterator_value(&it, int  )   /* Grab Value */
		);
	}

	//Now go in reverse
	printf("\nNow let's go in reverse:\n");
	for (cn_map_rbegin(map, &it); !cn_map_at_rend(map, &it); cn_map_prev(map, &it)) {
		printf(
			"%s - %d\n",
			cn_map_iterator_key  (&it, char*),  /* Grab Key   */
			cn_map_iterator_value(&it, int  )   /* Grab Value */
		);
	}

	//Cleanup
	cn_map_free(map);
}
