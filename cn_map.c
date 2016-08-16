/*
 * CN_Map Library
 *
 * Version 0.0.1 (Last Updated: 2016-08-15)
 *
 * Description:
 *     C++ Maps for C library. Implements the data structure with a struct and
 *     Red-Black Trees. Any data type can be stored in a CN_Map, just like C++
 *     Maps. CN_Maps require specification of two file types. One for the key,
 *     and another for what data type the tree node will store. It will also
 *     require a comparison function to sort the tree.
 *     
 *     It is highly recommended to use this along with the CN_Comp library, as
 *     it contains comparison functions for C data types to use in the CN_Map.
 *
 * Changelog of library is located at the bottom of "cn_map.h".
 *
 * Author:
 *     Clara Van Nguyen
 *
 * For documentation and details on every function in this library, visit:
 * http://web.eecs.utk.edu/~ssmit285/lib/cn_map/index.html
 */

#include "cn_map.h"

//Functions
//Initialiser
CN_MAP new_cn_map(cnm_uint key_size, cnm_uint elem_size, CNC_COMP (*__func)(void*, void*)) {
	CN_MAP map     = (CN_MAP) malloc(sizeof(struct cn_map));
	map->size      = 0;
	map->key_size  = key_size;
	map->elem_size = elem_size;
	map->head      = NULL;
	map->cmpfunc   = __func;

	return map;
}

//Add
void cn_map_insert(CN_MAP map, void* key, void* ptr) {
	//Create the node
	CNM_NODE* node = __cn_map_create_node(key, ptr, map->key_size, map->elem_size);
	
	//Now figure out where to put it in the tree.
	if (map->size == 0) {
		//Well that was easy...
		map->head = node;
		map->size++;
	} else {
		CNM_NODE* cur_node = map->head;
		CNC_COMP  compare;
		while (1) {
			compare = map->cmpfunc(key, cur_node->key);
			if (compare < 0) {
				//LESS THAN
				if (cur_node->left == NULL) {
					cur_node->left = node;
					node->up = cur_node;
					break;
				} else
					cur_node = cur_node->left;
			}
			if (compare == 0) {
				//EQUAL
				__cn_map_free_node(node);
				fprintf(stderr, "WARNING: Duplicate Key\n");
				break;
			}
			if (compare > 0) {
				//GREATER THAN
				if (cur_node->right == NULL) {
					cur_node->right = node;
					node->up = cur_node;
					break;
				} else
					cur_node = cur_node->right;
			}
		}
		if (compare != 0)
			map->size++;
	}

}


//Set

//Modify
void cn_map_clear(CN_MAP map) {
	//TODO: Implement...
}

//Get
CNM_NODE* cn_map_find(CN_MAP map, void* key) {
	if (map->head == NULL)
		return NULL;

	CNM_NODE* cur_node = map->head;
	CNC_COMP  compare;
	while (1) {
		compare = map->cmpfunc(key, cur_node->key);
		if (compare == 0) {
			//We have our match.
			return cur_node;
			break;
		}
		if (compare < 0) {
			if (cur_node->left == NULL)
				return NULL;
			else
				cur_node = cur_node->left;
		}
		if (compare > 0) {
			if (cur_node->right == NULL)
				return NULL;
			else
				cur_node = cur_node->right;
		}
	}
}

cnm_uint cn_map_size(CN_MAP map) {
	return map->size;
}

//Iteration
CNM_ITERATOR* cn_map_begin(CN_MAP map) {
	//Move along the left side of the tree. The left-most node is guaranteed
	//to be the lowest valued number.
	CNM_ITERATOR* iterator = (CNM_ITERATOR *) malloc(sizeof(struct cnm_iterator));
	iterator->count = 0;
	if (map->head == NULL) {
		iterator->node = NULL;
		return iterator;
	}
	CNM_NODE* cur_node = map->head;
	while (1) {
		if (cur_node->left != NULL) {
			iterator->prev = cur_node;
			cur_node = cur_node->left;
		} else {
			iterator->node = cur_node;
			break;
		}
	}

	return iterator;
}

CNM_ITERATOR* cn_map_end(CN_MAP map) {
	return NULL;
}

CNM_ITERATOR* cn_map_rbegin(CN_MAP map) {
	//Move along the right side of the tree. The right-most node is guaranteed
	//to be the highest valued number.
	CNM_ITERATOR* iterator = (CNM_ITERATOR *) malloc(sizeof(struct cnm_iterator));
	iterator->count = 0;
	if (map->head == NULL) {
		iterator->node = NULL;
		return iterator;
	}
	CNM_NODE* cur_node = map->head;
	while (1) {
		if (cur_node->right != NULL) {
			iterator->prev = cur_node;
			cur_node = cur_node->right;
		} else {
			iterator->node = cur_node;
			break;
		}
	}

	return iterator;
}

CNM_ITERATOR* cn_map_rend(CN_MAP map) {
	return NULL;
}

CNM_ITERATOR* cn_map_prev(CN_MAP map, CNM_ITERATOR* iterator) {
	
}

CNM_ITERATOR* cn_map_next(CN_MAP map, CNM_ITERATOR* iterator) {
	//Check if at last node.
	/*if (iterator->prev = iterator->node->up                   &&
	    map->cmpfunc(iterator->node->key, map->head->key) > 0 &&
		iterator->node->right == NULL                         &&
	    iterator->node->left  == NULL) {
		iterator->node = NULL;
		return iterator;
	}*/

	if (iterator->count >= map->size - 1) {
		iterator->node = NULL;
		return iterator;
	} else {
		iterator->count++;
	}

	//Try going back
	if (iterator->node->left == NULL && iterator->prev != iterator->node->left && map->cmpfunc(iterator->node->key, map->head->key) < 0) {
		//printf("As far left as possible\n");
		if (iterator->node->up != NULL) {
			//printf("Can go up\n");
			iterator->prev = iterator->node;
			iterator->node = iterator->node->up;
		} else {
			//printf("At root node. Going to the right\n");
			//Go down the right branch one level and go left as far as possible
			iterator->node = iterator->node->right;
			while (iterator->node->left != NULL) {
				iterator->prev = iterator->node;
				iterator->node = iterator->node->left;
			}
		}
	} else {
		if (iterator->prev == iterator->node->left) {
			//Did you just come from the bottom left? Go to the right and as far
			//left as possible.
			if (iterator->node->right != NULL) {
				iterator->node = iterator->node->right;
				while (iterator->node->left != NULL) {
					iterator->prev = iterator->node;
					iterator->node = iterator->node->left;
				}
			} else {
				//Or... not.
				if (iterator->node->up != NULL) {
					iterator->prev = iterator->node;
					iterator->node = iterator->node->up;
				}
			}
		}
	}
	return iterator;
}

//Destructor
void cn_map_free(CN_MAP map) {
	if (map->size > 0)
		cn_map_clear(map);
	free(map);
}

//Functions you won't use if you are sane
CNM_NODE* __cn_map_create_node(void* key, void* ptr, cnm_uint key_size, cnm_uint elem_size) {
	CNM_NODE* node = (CNM_NODE*) malloc(sizeof(struct cnm_node));
	node->key   = (void*) malloc(key_size );
	node->data  = (void*) malloc(elem_size);
	node->up    = NULL;
	node->left  = NULL;
	node->right = NULL;

	//Copy data over
	memcpy(node->key , key, key_size );
	memcpy(node->data, ptr, elem_size);

	return node;
}

void __cn_map_free_node(CNM_NODE* ptr) {
	if (ptr->key  != NULL) free(ptr->key );
	if (ptr->data != NULL) free(ptr->data);

	free(ptr);
}
