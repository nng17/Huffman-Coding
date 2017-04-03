/* Nathan Ng
 * Professor Atkinson
 * Friday Lab 2:15
 *
 * File: tree.c
 * this program creates and uses a tree to create, destroy, edit, and retrieve data.
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>
#include "tree.h"

struct tree 
{
	int data;
	struct tree *left; //pointer to the left
	struct tree *right; //pointer to the right
	struct tree *parent; //pointer to the parent
};

/* Function: Detach
 * has O(1)
 * this function check if the left or right subtree has a parent and then detaches it
 */

void detach(struct tree *root)
{
	assert(root != NULL);
	if(root->parent != NULL) 
	{
		if (root->parent->left == root)
			root->parent->left = NULL; //set the left's pointer to NULL
		else
			root->parent->right = NULL; //set the right's pointer to NULL
	}
}

/* Function: createTree
 * has O(1)
 * this function creates a tree by allocating space. assigns "data" as the root and gives it a left subtree indicated by "left" and a right subtree indicated by "right". Function also detaches the left and right parent trees if they are there
 */

struct tree *createTree(int data, struct tree *left, struct tree *right)
{
	struct tree *root;
	root = malloc(sizeof(struct tree)); //allocates space for the tree
	assert(root != NULL); //checks if the allocation worked
	
	// initializes the tree's attributes

	root->data = data; 
	root->right = right;
	root->left = left;
	root->parent = NULL;

	//making sure the right subtree's only parent is root

	if(right != NULL){
		detach(right);
		root->right->parent = root;
	}

	//making sure the left subtree's only parent is root

	if(left != NULL){
		detach(left);
		root->left->parent = root;
	}
	
	return root;
}

/* Function: destroyTree
 * has O(nlogn)
 * this function deallocates memory for the entire subtree pointed by root
 */

void destroyTree(struct tree *root)
{
	if (root != NULL) //check if there is something to delete
	{
		//deallocate root's subtrees
		destroyTree(root->right);
		destroyTree(root->left);

		//deallocates root
		free(root); 
	}
}

/* Function: getData
 * has O(1)
 * this function returns the data in the root of the subtree pointed by the root
 */

int getData(struct tree *root)
{
	assert(root != NULL); //makes sure the root is not NULL
	return root->data;
}

/* Function getLeft
 * has O(1)
 * this function returns the left subtree pointed by root
 */

struct tree *getLeft(struct tree *root)
{
	assert(root != NULL); //makes sure root is not NULL
	return root->left;
}

/* Function getRight
 * has O(1)
 * this function returns the right subtree pointed by root
 */

struct tree *getRight(struct tree *root)
{
	assert(root != NULL); //make sure root is not NULL
	return root->right;
}

/* Function: getParent
 * has O(1)
 * this function returns the parent tree pointed by root
 */

struct tree *getParent(struct tree *root)
{
	assert(root != NULL);
	return root->parent;
}

/* Function: setLeft
 * has O(1)
 * this function updates the left subtree of root
 */

void setLeft(struct tree *root, struct tree *left)
{
	assert(root != NULL && left != NULL);

	//makes sure left has no connections with its existing parents

	if(root->left != NULL)
		root->left->parent = NULL;
	root->left = left;
	detach(left);

	//establishes new parent for root

	root->left->parent = root;
	
}

/* Function: setRight
 * has O(1)
 * this function updates the right subtree of root
 */

void setRight(struct tree *root, struct tree *right)
{
	assert(root != NULL && right != NULL);

	//makes sure right has no connections with its existing parents
	
	if(root->right != NULL)
		root->right->parent = NULL;
	root->right = right;
	detach(right);

	//establishes new parent for root
	
	root->right->parent = root;
}

