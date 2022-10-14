#ifndef _tree_h
#define _tree_h
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

typedef struct TNode
{
    /* Data for every TNode */
    struct TNode* pLeft;    /* left child (NULL for leaf) */
    struct TNode* pRight;   /* right child (NULL for leaf) */
    struct TNode* pParent;  /* parent TNode (NULL for root) */

    /* AVL data */
    int height;             /* max number of nodes on path from this node down to a leaf of the tree */
    int data;             /* pointer to the data stored in the node, leaves contain no valid data */

}  TNode;

typedef struct Tree
{
    TNode* root;            /* the root of this tree (it will be a NULL if the tree is empty) */
}  Tree;

/**********  Functions for creating/freeing a tree **********/
Tree *createTree( );
Tree *createTreeFromTNode( TNode* root );
void freeTree( Tree* t );
void freeTreeContents( TNode *root);

/**********  Functions for creating/linking TNodes tree **********/
TNode* createTNode( );
void attachChildNodes( TNode* root, TNode* left, TNode* right );

/**********  Functions for searching an AVL tree **********/
TNode* searchTree( Tree *t, int tData );
TNode* searchTreeRec( TNode *root, int tData );

/**********  Functions for inserting/removing from an AVL tree **********/
TNode* insertNode( TNode *root, TNode* newNode );
void insertTree( Tree* t, int tData );
void insertTreeBalanced( Tree* t, int tData );
TNode* removeTree( Tree* t, int key );

/**********  Functions for debugging an AVL tree **********/
void printTree( TNode* root );
void checkAVLTree( TNode* root );
int getBalance(TNode* x);

/**********  Functions for printing an tree **********/
void printTreeByType( Tree* t, TNode* root, int depth );

#endif
