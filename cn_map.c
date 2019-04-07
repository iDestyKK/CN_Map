/*
 * CN_Map Library
 *
 * Version 1.0.0 (Last Updated: 2019-04-07)
 *
 * Description:
 *     C++ Maps for C library. Implements the data structure with a struct and
 *     Red-Black Trees. Any data type can be stored in a CN_Map, just like C++
 *     Maps. CN_Maps require specification of two file types. One for the key,
 *     and another for what data type the tree node will store. It will also
 *     require a comparison function to sort the tree.
 *
 *     It is highly recommended to use this along with the CN_Cmp library, as
 *     if contains comparison functions for C data types to use in the CN_Map.
 *
 * Author:
 *     Clara Nguyen (@iDestyKK)
 *
 * For documentation and Details on every function in this library, visit:
 *     http://docs.claranguyen.me/lib.php?id=cnds/cn_map
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cn_map.h"

// ----------------------------------------------------------------------------
// Constructor                                                             {{{1
// ----------------------------------------------------------------------------

/*
 * new_cn_map
 *
 * Description:
 *     Sets up a brand new, blank CN_Map for use. The size of the node elements
 *     is determined by what types are thrown in. "s1" is the size of the key
 *     elements in bytes, while "s2" is the size of the value elements in
 *     bytes.
 *
 *     Since this is also a tree data structure, a comparison function is also
 *     required to be passed in. A destruct function is optional and must be
 *     added in through another function.
 */

CN_MAP new_cn_map(CNM_UINT s1, CNM_UINT s2, CNC_COMP(*cmp)(void *, void *)) {
	CN_MAP obj = (CN_MAP) malloc(sizeof(struct cn_map));

	//Set all pointers to NULL
	obj->head  = NULL;

	//Set up all default properties
	obj->key_size  = s1;
	obj->elem_size = s2;
	obj->size      = 0;

	//Function pointers
	obj->func_compare = cmp;
	obj->func_destruct = NULL;

	//Dummy variables
	obj->it_end.prev = NULL;
	obj->it_end.node = NULL;
	obj->it_least.prev = NULL;
	obj->it_least.node = NULL;
	obj->it_most.prev = NULL;
	obj->it_most.node = NULL;

	return obj;
}

// ----------------------------------------------------------------------------
// Function Pointer Management                                             {{{1
// ----------------------------------------------------------------------------

/*
 * cn_map_set_func_comparison
 *
 * Description:
 *     Sets the comparison function for the tree. Generally this is only called
 *     during the tree's construction and is not intended for other uses. But,
 *     I'll make it available to the user.
 */

void cn_map_set_func_comparison(CN_MAP obj, CNC_COMP (*cmp)(void *, void *)) {
	obj->func_compare = cmp;
}

/*
 * cn_map_set_func_destructor
 *
 * Description:
 *     Sets the comparison function for the tree. Generally this is only called
 *     during the tree's construction and is not intended for other uses. But,
 *     I'll make it available to the user.
 */

void cn_map_set_func_destructor(CN_MAP obj, void (*dest)(CNM_NODE*)) {
	obj->func_destruct = dest;
}

// ----------------------------------------------------------------------------
// Add                                                                     {{{1
// ----------------------------------------------------------------------------

/*
 * cn_map_insert
 *
 * Description:
 *     Inserts a key/value pair into the CN_Map. The value can be blank. If so,
 *     it is filled with 0's, as defined in "__cn_map_create_node".
 *
 * Complexity:
 *     O(N lg N)
 */

CNM_UINT cn_map_insert(CN_MAP obj, void *key, void *value) {
	//Copy the key and value into a new node and prepare it to put into tree.
	CNM_NODE *new_node = __cn_map_create_node(
		key,
		value,
		obj->key_size,
		obj->elem_size
	);

	obj->size++;

	if (obj->head == NULL) {
		//Just insert the node in as the new head.
		obj->head = new_node;
		obj->head->colour = CNM_BLACK;
		return 1;
	}

	//Traverse the tree until we hit the end or find a side that is NULL
	CNM_NODE *cur = obj->head;
	CNM_NODE *target;
	CNC_COMP res;

	while (1) {
		res = obj->func_compare(new_node->key, cur->key);

		//If the key matches something else, we can't insert
		if (res == 0) {
			__cn_map_free_node(obj, new_node);
			return 0;
		}
		else {
			//If not null, traverse.
			//If null, set.
			if (res < 0) {
				if (cur->left == NULL) {
					cur->left = new_node;
					new_node->up = cur;
					__cn_map_fix_colours(obj, new_node);
					break;
				}
				else
					cur = cur->left;
			}
			else {
				if (cur->right == NULL) {
					cur->right = new_node;
					new_node->up = cur;
					__cn_map_fix_colours(obj, new_node);
					break;
				}
				else
					cur = cur->right;
			}
		}
	}

	__cn_map_calibrate(obj);

	//Insertion complete.
	return 1;
}

// ----------------------------------------------------------------------------
// Get Functions                                                           {{{1
// ----------------------------------------------------------------------------

void cn_map_find(CN_MAP obj, CNM_ITERATOR *it, void *key) {
	//End the search instantly if there's nothing.
	if (obj->head == NULL) {
		it->node = it->prev = NULL;
		return;
	}

	//Basically a repeat of insert
	CNM_NODE *cur = obj->head;
	CNM_NODE *target;
	CNC_COMP res;
	
	//Binary Search
	while (1) {
		res = obj->func_compare(key, cur->key);

		//If the key matches, we hit our target
		if (res == 0) {
			break;
		}
		else
		if (res < 0) {
			if (cur->left == NULL) {
				cur = NULL;
				break;
			}
			else
				cur = cur->left;
		}
		else {
			if (cur->right == NULL) {
				cur = NULL;
				break;
			}
			else
				cur = cur->right;
		}
	}

	if (cur != NULL) {
		it->node = cur;

		//Generate a "prev" too
		CNM_ITERATOR tmp;
		tmp = *it;
		__cn_map_prev(obj, &tmp);
		it->prev = tmp.node;
	}
	else {
		it->node = NULL;
	}
}

CNM_UINT cn_map_size(CN_MAP obj) {
	return obj->size;
}

CNM_BYTE cn_map_empty(CN_MAP obj) {
	return (obj->size == 0);
}

CNM_UINT cn_map_key_size(CN_MAP obj) {
	return obj->key_size;
}

CNM_UINT cn_map_value_size(CN_MAP obj) {
	return obj->elem_size;
}

// ----------------------------------------------------------------------------
// Iteration Functions                                                     {{{1
// ----------------------------------------------------------------------------

/*
 * cn_map_begin
 *
 * Description:
 *     Creates and returns a struct that contains a pointer to the key/value
 *     pair at the beginning of the map.
 */

void cn_map_begin(CN_MAP obj, CNM_ITERATOR *it) {
	//If there is nothing, return a blank iterator.
	if (obj->size == 0) {
		*it = obj->it_end;
		return;
	}

	//Traverse down to the left until we can't anymore.
	it->node = obj->head;
	while (it->node->left != NULL)
		it->node = it->node->left;

	//Mark the previous node as the parent
	it->prev = it->node->up;
}

/*
 * cn_map_end
 *
 * Description:
 *     Creates and returns a struct that contains a pointer to the key/value
 *     pair at the end of the map (NULL, in this case).
 */

void cn_map_end(CN_MAP obj, CNM_ITERATOR *it) {
	*it = obj->it_end;
}

/*
 * cn_map_rbegin
 *
 * Description:
 *     Creates and returns a struct that contains a pointer to the key/value
 *     pair at the reverse beginning of the map.
 */

void cn_map_rbegin(CN_MAP obj, CNM_ITERATOR *it) {
	//If there is nothing, return a blank iterator.
	if (obj->size == 0) {
		*it = obj->it_end;
		return;
	}

	//Traverse down to the right until we can't anymore.
	it->node = obj->head;
	while (it->node->right != NULL)
		it->node = it->node->right;

	//Mark the previous node as the parent
	it->prev = it->node->up;
}

/*
 * cn_map_rend
 *
 * Description:
 *     Creates and returns a struct that contains a pointer to the key/value
 *     pair at the reverse end of the map (NULL, in this case).
 */

void cn_map_rend(CN_MAP obj, CNM_ITERATOR *it) {
	*it = obj->it_end;
}

/*
 * cn_map_next
 *
 * Description:
 *     Advances the iterator to the next available key/value pair.
 */

void cn_map_next(CN_MAP obj, CNM_ITERATOR *it) {
	if (it->node == NULL) {
		//Nice try
		it->prev = NULL;
		return;
	}
	else
	if (it->node == obj->it_most.node) {
		//We have hit the end.
		it->prev = NULL; //it->node;
		it->node = NULL;
	}
	else
	if (it->node->right != NULL && it->prev != it->node->right) {
		//There is a right child. Go to the right, and then as far left as
		//possible.
		it->prev = it->node;
		it->node = it->node->right;

		while (it->node->left != NULL)
			it->node = it->node->left;
	}
	else {
		//Go up. How much depends on what "prev" is.
		it->prev = it->node;
		it->node = it->node->up;

		//Keep going up until we can't anymore.
		while (it->prev == it->node->right) {
			it->prev = it->node;
			it->node = it->node->up;
		}
	}
}

/*
 * cn_map_prev
 *
 * Description:
 *     Advances the iterator to the previous available key/value pair.
 */

void cn_map_prev(CN_MAP obj, CNM_ITERATOR *it) {
	__cn_map_prev(obj, it);

	//Compute "prev"
	CNM_ITERATOR tmp;
	tmp = *it;
	__cn_map_prev(obj, &tmp);
	it->prev = tmp.node;
}

void __cn_map_prev(CN_MAP obj, CNM_ITERATOR *it) {
	if (it->node == NULL) {
		//Nice try
		it->prev = NULL;
		return;
	}
	else
	if (it->node == obj->it_least.node) {
		//We have hit the end.
		it->prev = NULL; //it->node;
		it->node = NULL;
	}
	else
	//To the left, and then as far right as possible.
	if (it->node->left != NULL) {
		it->node = it->node->left;

		while (it->node->right != NULL)
			it->node = it->node->right;
	}
	else {
		//Keep going up until there is a left child
		it->prev = it->node;
		it->node = it->node->up;

		if (it->node == NULL)
			return;

		//Well... okay
		while (
			it->node->up   != NULL &&
			it->node->left != NULL &&
			it->node->left == it->prev
		) {
			it->prev = it->node;
			it->node = it->node->up;
		}
	}
}

/*
 * cn_map_at_begin
 *
 * Description:
 *     Returns true if at the the rend of the CN_Map
 */

CNM_BYTE cn_map_at_begin(CN_MAP obj, CNM_ITERATOR *it) {
	return (it->node == obj->it_least.node);
}

/*
 * cn_map_at_end
 *
 * Description:
 *     Returns true if at the the rend of the CN_Map
 */

CNM_BYTE cn_map_at_end(CN_MAP obj, CNM_ITERATOR *it) {
	return (it->node == NULL);
}

/*
 * cn_map_at_rbegin
 *
 * Description:
 *     Returns true if at the the rend of the CN_Map
 */

CNM_BYTE cn_map_at_rbegin(CN_MAP obj, CNM_ITERATOR *it) {
	return (it->node == obj->it_most.node);
}

/*
 * cn_map_at_rend
 *
 * Description:
 *     Returns true if at the the rend of the CN_Map
 */

CNM_BYTE cn_map_at_rend(CN_MAP obj, CNM_ITERATOR *it) {
	return (it->node == NULL);
}

// ----------------------------------------------------------------------------
// Remove Functions                                                        {{{1
// ----------------------------------------------------------------------------

/*
 * cn_map_erase
 *
 * Description:
 *     Removes a node from the CN_Map. It performs a BST delete, and then
 *     reorders the tree so that it remains balanced.
 */

void cn_map_erase(CN_MAP obj, CNM_ITERATOR *it) {
	CNM_UINT    result;
	CNM_NODE   *x, *y, *new_node;
	CNM_NODE   *node, *target, *t_sibling, *t_parent, *double_blk, *x_parent;
	CNM_BYTE    c;
	CNM_COLOUR  uc, vc;
	CNM_BYTE    res_case;

	node = it->node;

	//If it is the head, and the size is 1, just delete it.
	if (obj->size == 1 && node == obj->head) {
		__cn_map_free_node(obj, node);
		obj->head = NULL;
		obj->size--;
		return;
	}

	//Determine what the target is
	c  = 0;
	c |= (node->left  != NULL) << 0x0;
	c |= (node->right != NULL) << 0x1;
	
	//Determine the case
	switch (c) {
		case 0x0:
			//Leaf node (this should be impossible)
			target = node;
			break;

		case 0x1:
			//Has left child
			target = node->left;
			break;

		case 0x2:
			//Has right child
			target = node->right;
			break;

		case 0x3:
			//Has 2 children
			target = node->left;
			while (target->right != NULL)
				target = target->right;
			break;
	}

	//Grab the parent and sibling to the target node.
	t_parent = target->up;

	if (t_parent == NULL)
		t_sibling = target;
	else
		t_sibling = (t_parent->left == target)
			? t_parent->right
			: t_parent->left;

	//Initially there is no Double Black
	double_blk = NULL;

	if (node->left == NULL || node->right == NULL)
		y = node;
	else 
		y = target;

	if (y->left != NULL)
		x = y->left;
	else
		x = y->right;

	if (x != NULL)
		x->up = y->up;

	x_parent = y->up;

	CNM_BYTE y_is_left = 0;
	if (y->up == NULL) {
		obj->head = x;
	}
	else {
		if (y == y->up->left) {
			y->up->left = x;
			y_is_left = 1;
		}
		else
			y->up->right = x;
	}

	if (y != node) {
		//Move pointers over
		if (obj->func_destruct != NULL)
			obj->func_destruct(node);

		free(node->key);
		free(node->data);

		node->key  = y->key;
		node->data = y->data;

		y->key = y->data = NULL;
	}

	if (y->colour == CNM_BLACK) {
		//Make a blank node if null
		if (x == NULL) {
			double_blk = __cn_map_create_node(
				NULL, NULL, obj->key_size, obj->elem_size
			);

			x = double_blk;

			if (target->up->left == NULL)
				target->up->left = x;
			else
				target->up->right = x;

			x->up = target->up;
			x->colour = CNM_BLACK;
		}

		//Let's fix the tree up
		__cn_map_delete_fixup(
			obj,
			x,
			x_parent,
			y_is_left,
			y
		);

		//Clean up Double Black
		if (double_blk != NULL) {
			if (double_blk->up != NULL) {
				if (double_blk->up->left == double_blk)
					double_blk->up->left = NULL;
				else
					double_blk->up->right = NULL;
			}

			__cn_map_free_node(obj, double_blk);
		}
	}

	obj->size--;

	__cn_map_free_node(obj, y);
	__cn_map_calibrate(obj);
}

/*
 * cn_map_clear
 *
 * Description:
 *     Deletes all nodes in the graph. This is done by calling erase on the
 *     head node until the tree is emptied.
 */

void cn_map_clear(CN_MAP obj) {
	//Aggressively delete by recursion.
	if (obj->head != NULL)
		__cn_map_clear_nested(obj, obj->head);

	//Reset stats
	obj->size = 0;
	obj->head = NULL;
}

/*
 * cn_map_free
 *
 * Description:
 *     Frees the CN_Map from memory. Deletes all nodes. Call this when you are
 *     done using the data structure.
 */

void cn_map_free(CN_MAP obj) {
	//Free all nodes
	cn_map_clear(obj);

	//Free the map itself
	free(obj);
}

// ----------------------------------------------------------------------------
// Private/Implementation Helper Functions                                 {{{1
// ----------------------------------------------------------------------------

/*
 * __cn_map_create_node
 *
 * Description:
 *     Creates a node to be attached in the CN_Map internal tree structure.
 */

CNM_NODE *__cn_map_create_node(
	void     *key,
	void     *value,
	CNM_UINT  ksize,
	CNM_UINT  vsize
) {
	CNM_NODE *node = (CNM_NODE *) malloc(sizeof(struct cnm_node));

	//Allocate memory for the keys and values.
	node->key  = (void *) malloc(ksize);
	node->data = (void *) malloc(vsize);

	//Setup the pointers
	node->left  = NULL;
	node->right = NULL;
	node->up    = NULL;

	//Set the colour to black by default
	node->colour = CNM_RED;

	/*
	 * Copy over the key and values
	 *
	 * If the parameter passed in is NULL, make the element blank instead of
	 * a segfault.
	 */
	if (key == NULL)
		memset(node->key , 0  , ksize);
	else
		memcpy(node->key , key, ksize);

	if (value == NULL)
		memset(node->data, 0    , vsize);
	else
		memcpy(node->data, value, vsize);

	return node;
}

void __cn_map_free_node(CN_MAP obj, CNM_NODE *node) {
	//Call the destructor... if it exists.
	if (obj->func_destruct != NULL)
		obj->func_destruct(node);

	if (node->key  != NULL) free(node->key);
	if (node->data != NULL) free(node->data);

	free(node);
}

void __cn_map_fix_colours(CN_MAP obj, CNM_NODE *node) {
	//If root, set the colour to black
	if (node == obj->head) {
		node->colour = CNM_BLACK;
		return;
	}

	//If node's parent is black or node is root, back out.
	if (node->up->colour == CNM_BLACK && node->up != obj->head)
		return;

	//Find out who is who
	CNM_NODE *parent      = node->up;
	CNM_NODE *grandparent = parent->up;
	CNM_NODE *uncle;

	if (parent->up == NULL)
		return;

	//Find out the uncle
	if (grandparent->left == parent)
		uncle = grandparent->right;
	else
		uncle = grandparent->left;

	if (uncle != NULL && uncle->colour == CNM_RED) {
		//If the uncle is red...
		//Change colour of parent and uncle to black
		uncle->colour = CNM_BLACK;
		parent->colour = CNM_BLACK;
		
		//Change colour of grandparent to red.
		grandparent->colour = CNM_RED;

		//Call this on the grandparent
		__cn_map_fix_colours(obj, grandparent);
	}
	else
	if (uncle == NULL || uncle->colour == CNM_BLACK) {
		//If the uncle is black...
		if (parent == grandparent->left && node == parent->left)
			__cn_map_l_l(obj, node, parent, grandparent, uncle);
		else
		if (parent == grandparent->left && node == parent->right)
			__cn_map_l_r(obj, node, parent, grandparent, uncle);
		else
		if (parent == grandparent->right && node == parent->left)
			__cn_map_r_l(obj, node, parent, grandparent, uncle);
		else
		if (parent == grandparent->right && node == parent->right)
			__cn_map_r_r(obj, node, parent, grandparent, uncle);
	}
}

/*
 * __cn_map_delete_fixup
 *
 * Description:
 *     Fixes the Red-Black tree post-BST deletion. This may involve multiple
 *     recolours and/or rotations depending on which node was deleted, what
 *     colour it was, and where it was in the tree at the time of deletion.
 *
 *     These fixes occur up and down the path of the tree, and each rotation is
 *     guaranteed constant time. As such, there is a maximum of O(lg n)
 *     operations taking place during the fixup procedure.
 */

void __cn_map_delete_fixup(
	CN_MAP    obj,
	CNM_NODE *node,
	CNM_NODE *p,
	CNM_BYTE  y_is_left,
	CNM_NODE *y
) {
	CNM_NODE   *w;
	CNM_COLOUR  lc, rc;

	if (node == NULL)
		return;

	while (node != obj->head && node->colour == CNM_BLACK) {
		//If left child
		if (y_is_left) {
			w = p->right;

			if (w->colour == CNM_RED) {
				w->colour = CNM_BLACK;
				p->colour = CNM_RED;
				p = __cn_map_rotate_left(obj, p)->left;
				w = p->right;
			}

			lc = (w->left  == NULL) ? CNM_BLACK : w->left->colour;
			rc = (w->right == NULL) ? CNM_BLACK : w->right->colour;

			if (lc == CNM_BLACK && rc == CNM_BLACK) {
				w->colour = CNM_RED;
				node = node->up;
				p = node->up;

				if (p != NULL)
					y_is_left = (node == p->left);
			}
			else {
				if (rc == CNM_BLACK) {
					w->left->colour = CNM_BLACK;
					w->colour = CNM_RED;
					w = __cn_map_rotate_right(obj, w);
					w = p->right;
				}

				w->colour = p->colour;
				p->colour = CNM_BLACK;

				if (w->right != NULL)
					w->right->colour = CNM_BLACK;

				p = __cn_map_rotate_left(obj, p);
				node = obj->head;
				p = NULL;
			}
		}
		else {
			/* Same except flipped "left" and "right" */
			w = p->left;

			if (w->colour == CNM_RED) {
				w->colour = CNM_BLACK;
				p->colour = CNM_RED;
				p = __cn_map_rotate_right(obj, p)->right;
				w = p->left;
			}

			lc = (w->left  == NULL) ? CNM_BLACK : w->left->colour;
			rc = (w->right == NULL) ? CNM_BLACK : w->right->colour;

			if (lc == CNM_BLACK && rc == CNM_BLACK) {
				w->colour = CNM_RED;
				node = node->up;
				p = node->up;
				if (p != NULL)
					y_is_left = (node == p->left);
			}
			else {
				if (lc == CNM_BLACK) {
					w->right->colour = CNM_BLACK;
					w->colour = CNM_RED;
					w = __cn_map_rotate_left(obj, w);
					w = p->left;
				}

				w->colour = p->colour;
				p->colour = CNM_BLACK;

				if (w->left != NULL)
					w->left->colour = CNM_BLACK;

				p = __cn_map_rotate_right(obj, p);
				node = obj->head;
				p = NULL;
			}
		}
	}

	node->colour = CNM_BLACK;
}

void __cn_map_l_l(
	CN_MAP obj,
	CNM_NODE *node,
	CNM_NODE *parent,
	CNM_NODE *grandparent,
	CNM_NODE *uncle
) {
	//Rotate to the right according to grandparent
	grandparent = __cn_map_rotate_right(obj, grandparent);

	//Swap grandparent and uncle's colours
	CNM_COLOUR c1, c2;
	c1 = grandparent->colour;
	c2 = grandparent->right->colour;

	grandparent->colour = c2;
	grandparent->right->colour = c1;
}

void __cn_map_l_r(
	CN_MAP obj,
	CNM_NODE *node,
	CNM_NODE *parent,
	CNM_NODE *grandparent,
	CNM_NODE *uncle
) {
	//Rotate to the left according to parent
	parent = __cn_map_rotate_left(obj, parent);

	//Refigure out who is who
	node = parent->left;
	grandparent = parent->up;
	uncle = (grandparent->left == parent)
		? grandparent->right
		: grandparent->left;

	//Apply left-left case
	__cn_map_l_l(obj, node, parent, grandparent, uncle);
}

void __cn_map_r_r(
	CN_MAP obj,
	CNM_NODE *node,
	CNM_NODE *parent,
	CNM_NODE *grandparent,
	CNM_NODE *uncle
) {
	//Rotate to the left according to grandparent
	grandparent = __cn_map_rotate_left(obj, grandparent);

	//Swap grandparent and uncle's colours
	CNM_COLOUR c1, c2;
	c1 = grandparent->colour;
	c2 = grandparent->left->colour;

	grandparent->colour = c2;
	grandparent->left->colour = c1;
}

void __cn_map_r_l(
	CN_MAP obj,
	CNM_NODE *node,
	CNM_NODE *parent,
	CNM_NODE *grandparent,
	CNM_NODE *uncle
) {
	//Rotate to the right according to parent
	parent = __cn_map_rotate_right(obj, parent);

	//Refigure out who is who
	node = parent->right;
	grandparent = parent->up;
	uncle = (grandparent->left == parent)
		? grandparent->right
		: grandparent->left;

	//Apply right-right case
	__cn_map_r_r(obj, node, parent, grandparent, uncle);
}

/*
 * __cn_map_rotate_left
 *
 * Description:
 *     Performs a left rotation with "node". The following happens (with
 *     respect to "C":
 *
 *         B                C
 *        / \              / \
 *       A   C     =>     B   D
 *            \          /
 *             D        A
 *
 *     Returns the new node pointing in the spot of the original node.
 */

CNM_NODE *__cn_map_rotate_left(CN_MAP obj, CNM_NODE *node) {
	CNM_NODE *top, *r, *rr, *rl, *up;

	top = node;
	r   = node->right;
	rl  = r->left;
	rr  = r->right;
	up  = node->up;

	//Adjust
	r->up = up;
	r->left = node;

	node->right = rl;
	node->up = r;

	if (node->right != NULL)
		node->right->up = node;

	if (up != NULL) {
		if (up->right == node)
			up->right = r;
		else
			up->left = r;
	}

	if (node == obj->head)
		obj->head = r;

	return r;
}

/*
 * __cn_map_rotate_right
 *
 * Description:
 *     Performs a right rotation with "node". The following happens (with
 *     respect to "C":
 *
 *         C                B
 *        / \              / \
 *       B   D     =>     A   C
 *      /                      \
 *     A                        D
 *
 *     Returns the new node pointing in the spot of the original node.
 */

CNM_NODE *__cn_map_rotate_right(CN_MAP obj, CNM_NODE *node) {
	CNM_NODE *top, *l, *ll, *lr, *up;

	top = node;
	l   = node->left;
	lr  = l->right;
	ll  = l->left;
	up  = node->up;

	//Adjust
	l->up = up;
	l->right = node;

	node->left = lr;
	node->up = l;

	if (node->left != NULL)
		node->left->up = node;

	if (up != NULL) {
		if (up->right == node)
			up->right = l;
		else
			up->left = l;
	}

	if (node == obj->head)
		obj->head = l;

	return l;
}

/*
 * __cn_map_clear_nested
 *
 * Description:
 *     Recursive wrapper for deleting nodes in a graph at an accelerated pace.
 *     Skips rotations. Just aggressively goes through all nodes and deletes.
 */

void __cn_map_clear_nested(CN_MAP obj, CNM_NODE *node) {
	//Free children
	if (node->left  != NULL) __cn_map_clear_nested(obj, node->left );
	if (node->right != NULL) __cn_map_clear_nested(obj, node->right);

	//Free self
	__cn_map_free_node(obj, node);
}

/*
 * __cn_map_calibrate
 *
 * Description:
 *     Recalculate the positions of the "least" and "most" iterators in the
 *     tree. This is so iterators know where the beginning and end of the tree
 *     resides.
 */

void __cn_map_calibrate(CN_MAP obj) {
	if (obj->head == NULL) {
		obj->it_least.node = obj->it_most.node = NULL;
		return;
	}

	//Recompute it_least and it_most
	obj->it_least.node = obj->it_most.node = obj->head;

	while (obj->it_least.node->left != NULL)
		obj->it_least.node = obj->it_least.node->left;

	while (obj->it_most.node->right != NULL)
		obj->it_most.node = obj->it_most.node->right;
}
