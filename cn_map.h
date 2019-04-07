/*
 * CN_Map Library
 *
 * Version 1.0.0 (Last Updated: 2019-03-13)
 *
 * Description:
 *     C++ Maps for C library. Implements the data structure with a struct and
 *     Red-Black Trees. Any data type can be stored in a CN_Map, just like C++
 *     Maps. CN_Maps require specification of two file types. One for the key,
 *     and another for what data type the tree node will store. It will also
 *     require a comparison function to sort the tree.
 *
 *     It is highly recommended to use this along with the CN_Comp library, as
 *     if contains comparison functions for C data types to use in the CN_Map.
 *
 * Author:
 *     Clara Nguyen (@iDestyKK)
 *
 * For documentation and Details on every function in this library, visit:
 *     http://docs.claranguyen.me/lib.php?id=cnds/cn_map
 */

#ifndef __CN_MAP__
#define __CN_MAP__

// ----------------------------------------------------------------------------
// Typedefs/Enums                                                          {{{1
// ----------------------------------------------------------------------------

typedef unsigned int       CNM_UINT;
typedef unsigned long long CNM_U64;
typedef unsigned char      CNM_BYTE;

typedef enum cnm_colour {
	CNM_RED,
	CNM_BLACK,
	CNM_DOUBLE_BLACK
} CNM_COLOUR;

//If CN_Comp hasn't been included, we still need to define the type.
#ifndef __CN_COMP__
	typedef int CNC_COMP;
#endif

// ----------------------------------------------------------------------------
// Structs                                                                 {{{1
// ----------------------------------------------------------------------------

/*
 * Node Struct
 *
 * Stores the key, data, and values of each element in the tree. This is the
 * main basis of the entire tree aside from the root struct.
 */

typedef struct cnm_node {
	void *key;
	void *data;

	struct cnm_node *left, *right, *up;
	CNM_COLOUR colour;
} CNM_NODE;

/*
 * Iterator Struct
 *
 * Helps with traversal of the CN_Map. There will be helper functions that can
 * be used with CNM_ITERATORs to iterate through the interior of the structure
 * without having to look at the implementation.
 */

typedef struct cnm_iterator {
	struct cnm_node *prev, *node;
	CNM_UINT count;
} CNM_ITERATOR;

/*
 * CN_Map Main Struct
 *
 * Stores access to the head node, as well as the first and last nodes. Keeps
 * track of all aspects of the tree. All CN_Map utility functions require a
 * pointer to this struct.
 */

typedef struct cn_map {
	/* Pointers to elements */
	struct cnm_node *head;

	/* Properties */
	CNM_UINT key_size;
	CNM_UINT elem_size;
	CNM_UINT size;

	/* Dummy variables */
	CNM_ITERATOR it_end, it_most, it_least;

	/* Function Pointers */
	CNC_COMP (*func_compare )(void *, void *);
	void     (*func_destruct)(CNM_NODE *);
} *CN_MAP;

//For you C++ people...
typedef CN_MAP MAP;

// ----------------------------------------------------------------------------
// Public Functions                                                        {{{1
// ----------------------------------------------------------------------------

//Constructor
CN_MAP       new_cn_map (CNM_UINT, CNM_UINT, CNC_COMP(*)(void *, void *));

//Function Pointer Management
void         cn_map_set_func_comparison(CN_MAP, CNC_COMP(*)(void *, void *));
void         cn_map_set_func_destructor(CN_MAP, void(*)(CNM_NODE *));

//Add Functions
CNM_UINT     cn_map_insert             (CN_MAP, void*, void*);

//Get Functions
void         cn_map_find               (CN_MAP, CNM_ITERATOR *, void*);
CNM_UINT     cn_map_size               (CN_MAP);
CNM_BYTE     cn_map_empty              (CN_MAP);
CNM_UINT     cn_map_key_size           (CN_MAP);
CNM_UINT     cn_map_value_size         (CN_MAP);

//Iteration
void         cn_map_begin              (CN_MAP, CNM_ITERATOR *);
void         cn_map_end                (CN_MAP, CNM_ITERATOR *);
void         cn_map_rbegin             (CN_MAP, CNM_ITERATOR *);
void         cn_map_rend               (CN_MAP, CNM_ITERATOR *);
void         cn_map_next               (CN_MAP, CNM_ITERATOR *);
void         cn_map_prev               (CN_MAP, CNM_ITERATOR *);
void         __cn_map_prev             (CN_MAP, CNM_ITERATOR *);

CNM_BYTE     cn_map_at_begin           (CN_MAP, CNM_ITERATOR *);
CNM_BYTE     cn_map_at_end             (CN_MAP, CNM_ITERATOR *);
CNM_BYTE     cn_map_at_rbegin          (CN_MAP, CNM_ITERATOR *);
CNM_BYTE     cn_map_at_rend            (CN_MAP, CNM_ITERATOR *);

//Remove Functions
void      cn_map_erase                 (CN_MAP, CNM_ITERATOR *);
void      cn_map_clear                 (CN_MAP);

//Cleanup/Destructor
void      cn_map_free                  (CN_MAP);

// ----------------------------------------------------------------------------
// Private/Implementation Helper Functions                                 {{{1
// ----------------------------------------------------------------------------

CNM_NODE *__cn_map_create_node (void*, void*, CNM_UINT, CNM_UINT);
void      __cn_map_free_node   (CN_MAP, CNM_NODE *);
void      __cn_map_fix_colours (CN_MAP, CNM_NODE *);
void      __cn_map_delete_fixup(CN_MAP, CNM_NODE *, CNM_NODE *, CNM_BYTE,
                                CNM_NODE *);

void      __cn_map_l_l(CN_MAP, CNM_NODE *, CNM_NODE *, CNM_NODE *, CNM_NODE *);
void      __cn_map_l_r(CN_MAP, CNM_NODE *, CNM_NODE *, CNM_NODE *, CNM_NODE *);
void      __cn_map_r_l(CN_MAP, CNM_NODE *, CNM_NODE *, CNM_NODE *, CNM_NODE *);
void      __cn_map_r_r(CN_MAP, CNM_NODE *, CNM_NODE *, CNM_NODE *, CNM_NODE *);

CNM_NODE *__cn_map_rotate_left (CN_MAP, CNM_NODE *);
CNM_NODE *__cn_map_rotate_right(CN_MAP, CNM_NODE *);

CNM_UINT  __cn_map_bst_delete  (CN_MAP, CNM_NODE *, CNM_NODE *);
void      __cn_map_clear_nested(CN_MAP, CNM_NODE *);

void      __cn_map_calibrate   (CN_MAP);

// ----------------------------------------------------------------------------
// Macro Functions                                                         {{{1
// ----------------------------------------------------------------------------

#define cn_map_init(key_type, elem_type, __func) \
	new_cn_map(sizeof(key_type), sizeof(elem_type), __func)

#define cn_map_iterator_key(it, type) \
	(*(type*)(it)->node->key)

#define cn_map_iterator_value(it, type) \
	(*(type*)(it)->node->data)

#define cn_map_traverse(map, pit) \
	for ( \
		 cn_map_begin  (map, pit); \
		!cn_map_at_end (map, pit); \
		 cn_map_next   (map, pit)  \
	)

#define cn_map_rtraverse(map, pit) \
	for ( \
		 cn_map_rbegin  (map, pit); \
		!cn_map_at_rend (map, pit); \
		 cn_map_prev    (map, pit)  \
	)

#endif
