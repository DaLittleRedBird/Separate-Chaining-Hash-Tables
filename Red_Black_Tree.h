#ifndef RED_BLACK_TREE_MAP
#define RED_BLACK_TREE_MAP

/*
    Red-Black trees are considered the main benchmark for fast hashing, so it makes sence to define their methods here and now.
The time it takes to insert, delete or lookup a key in a red black tree is O(log(n)), so any hash table whose lookup time is O(n)
will not be implemented because they are often too slow compared to RB Trees.
*/

enum COLOR { RED = 0, BLACK = 1 };
typedef char DIRECTION;

struct RED_BLACK_NODE {
    COLOR color;                   //0 is red, 1 is black
    unsigned int key;
    struct RED_BLACK_NODE* parent;
    union {
        struct {
			struct RED_BLACK_NODE* left;
			struct RED_BLACK_NODE* right;
		};
		struct RED_BLACK_NODE* child[2];
    }
}

struct RED_BLACK_TREE {
    struct RED_BLACK_NODE* root;
}

static DIRECTION direction(const RED_BLACK_NODE* N) { return N == N->parent->right ? 1 : 0; }    //0 is left, 1 is right

RED_BLACK_NODE* rotate_subtree(RED_BLACK_TREE* tree, RED_BLACK_NODE* sub, DIRECTION dir) {
	RED_BLACK_NODE* sub_parent = sub->parent;
	RED_BLACK_NODE* new_root = sub->child[1 - dir]; // 1 - dir is the opposite direction
	RED_BLACK_NODE* new_child = new_root->child[dir];

	sub->child[1 - dir] = new_child;

	if (new_child) { new_child->parent = sub; }

	new_root->child[dir] = sub;

	new_root->parent = sub_parent;
	sub->parent = new_root;
	if (sub_parent) { sub_parent->child[sub == sub_parent->right] = new_root; } else { tree->root = new_root; }

	return new_root;
}

void insert(RED_BLACK_TREE* tree, RED_BLACK_NODE* node, RED_BLACK_NODE* parent, DIRECTION dir) {
	node->color = RED;          //0 is red, 1 is black
	node->parent = parent;

	if (!parent) {
		tree->root = node;
		return;
	}

	parent->child[dir] = node;

	// rebalance the tree 
	do {
		// Case #1
		if (parent->color == BLACK) { return; }
	    RED_BLACK_NODE* grandparent = parent->parent;

		if (!grandparent) {
			// Case #4
			parent->color = BLACK;
			return;
		}

		dir = direction(parent);
		Node* uncle = grandparent->child[1 - dir];
		if (!uncle || uncle->color == BLACK) {
			if (node == parent->child[1 - dir]) {
				// Case #5
				rotate_subtree(tree, parent, dir);
				node = parent;
				parent = grandparent->child[dir];
			}

			// Case #6
			rotate_subtree(tree, grandparent, 1 - dir);
			parent->color = BLACK;
			grandparent->color = RED;
			return;
		}
	
		// Case #2
		parent->color = BLACK;
		uncle->color = BLACK;
		grandparent->color = RED;
		node = grandparent;

	} while (parent = node->parent);

	// Case #3
	return;
}

void remove(RED_BLACK_TREE* tree, RED_BLACK_NODE* node) {
	RED_BLACK_NODE* parent = node->parent;

	RED_BLACK_NODE* sibling;
	RED_BLACK_NODE* close_nephew;
	RED_BLACK_NODE* distant_nephew;

	DIRECTION dir = direction(node);

	parent->child[dir] = NULL;
	goto start_balance;

	do {
		dir = direction(node);
start_balance:
		sibling = parent->child[1 - dir];
		distant_nephew = sibling->child[1 - dir];
		close_nephew = sibling->child[dir];
		if (sibling->color == RED) {
			// Case #3
			rotate_subtree(tree, parent, dir);
			parent->color = RED;
			sibling->color = BLACK;
			sibling = close_nephew;

			distant_nephew = sibling->child[1 - dir];
			if (distant_nephew && distant_nephew->color == RED) {
				goto case_6;
            }
			close_nephew = sibling->child[dir];
			if (close_nephew && close_nephew->color == RED) {
				goto case_5;
            }

			// Case #4
			sibling->color = RED;
			parent->color = BLACK;
			return;
		}

		if (distant_nephew && distant_nephew->color == RED) { goto case_6; }

		if (close_nephew && close_nephew->color == RED) { goto case_5; }

		if (parent->color == RED) {
			// Case #4
			sibling->color = RED;
			parent->color = BLACK;
			return;
		}

		// Case #1
		if (!parent) { return; }

		// Case #2
		sibling->color = RED;
		node = parent;

	} while (parent = node->parent);

case_5:

	rotate_subtree(tree, sibling, 1 - dir);
	sibling->color = RED;
	close_nephew->color = BLACK;
	distant_nephew = sibling;
	sibling = close_nephew;

case_6:

	rotate_subtree(tree, parent, dir);
	sibling->color = parent->color;
	parent->color = BLACK;
	distant_nephew->color = BLACK;
	return;
}

#endif
