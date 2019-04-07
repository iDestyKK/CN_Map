/*
 * CN_Map Example - Insertion and Deletion
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../cn_cmp.h"
#include "../cn_map.h"

main() {
	CN_MAP map = cn_map_init(int, int, cn_cmp_int);

	int key;
	int value;
	int read_in;

	char cmd;
	CNM_ITERATOR it;

	//Read in commands and manipulate the tree
	while (scanf("%c", &cmd) == 1) {
		switch (cmd) {
			case 'I':
				//Insert
				read_in = scanf("%d %d", &key, &value);
				if (read_in != 2)
					fprintf(stderr, "Usage: I key value\n");
				else
					cn_map_insert(map, &key, &value);
				break;

			case 'C':
				//Clear
				cn_map_clear(map);
				break;

			case 'P':
				//Print
				//__cn_map_print_tree(map);
				cn_map_traverse(map, &it) {
					printf(
						"%d - > %d\n",
						cn_map_iterator_key  (&it, int),
						cn_map_iterator_value(&it, int)
					);
				}
				break;

			case 'D':
				//Delete
				read_in = scanf("%d", &key);
				if (read_in != 1)
					fprintf(stderr, "Usage: D key\n");
				else {
					cn_map_find(map, &it, &key);

					if (cn_map_at_end(map, &it))
						fprintf(stderr, "Key \"%d\" isn't in the map.\n", key);
					else
						cn_map_erase(map, &it);
				}
				break;
		}
	}

	cn_map_free(map);
}
