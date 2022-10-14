#include "tree.h"

/**********  Helper functions for removing from an AVL tree **********/
TNode* removeNextInorder( TNode** pRoot );

/**********  Helper functions for balancing an AVL tree **********/
void updateHeights(TNode* root);
void rebalanceTree(Tree* t, TNode* x);
void rightRotate(Tree* t, TNode* root);
void leftRotate(Tree* t, TNode* root);

TNode* getTallerSubTree(TNode* root);
bool isSameSignBalance(TNode* x, TNode* z);
int subTreeHeight(TNode* root);

/* createTree
 * input: none
 * output: a pointer to a Tree (this is malloc-ed so must be freed eventually!)
 *
 * Creates a new empty Tree and returns a pointer to it.
 */
Tree *createTree( )
{
    Tree* t = (Tree*)malloc( sizeof(Tree) );
    t->root = NULL;

    return t;
}

Tree *createTreeFromTNode( TNode* root )
{
    Tree* t = (Tree*)malloc( sizeof(Tree) );
    t->root = root;

    return t;
}

/* attachChildNodes
 * input: three pointers to a TNodes
 * output: none
 *
 * Sets root's left and right children to the specified nodes
 */
void attachChildNodes( TNode* root, TNode* left, TNode* right ){
    root->pLeft = left;
    root->pRight = right;
    if( left!=NULL )
        root->pLeft->pParent = root;
    if( right!=NULL )
        root->pRight->pParent = root;
    updateHeights(root);
}

/* createTNode
 * input: none
 * output: TNode*
 *
 * Malloc and returns a new empty TNode
 */
TNode* createTNode( ){
    TNode* newNode = (TNode*)malloc( sizeof(TNode) );
    newNode->height = 1;
    newNode->pParent = NULL;
    attachChildNodes( newNode, NULL, NULL );
    return newNode;
}

void freeTreeContents( TNode *root)
{
    if(root==NULL)
        return;

    freeTreeContents(root->pLeft);
    freeTreeContents(root->pRight);

    free(root);
}

/* freeTree and freeTreeContents
 * input: a pointer to a Tree
 * output: none
 *
 * frees the given Tree and all of Data elements
 */
void freeTree( Tree *t )
{
    freeTreeContents(t->root);
    free(t);
}


/**********  Functions for searching an AVL tree **********/

/* searchTree and searchTreeRec
 * input: a pointer to a Tree, a Data* tData
 * output: a pointer to the TNode that contains tData or, if no such node exists, NULL
 *
 * Finds and returns a pointer to the TNode that contains tData or, if no such node exists, it returns a NULL
 */
TNode* searchTree( Tree *t, int tData )
{
    return searchTreeRec( t->root, tData );
}

TNode* searchTreeRec( TNode *root, int tData )
{
    if( root==NULL || tData == root->data)
        return root;
    else if( tData < root->data)
        return searchTreeRec( root->pLeft, tData );
    else /* compareData( tData, root->data ) > 0 */
        return searchTreeRec( root->pRight, tData );
}


/**********  Functions for inserting/removing from an AVL tree **********/

/* insertNode
 * input: a pointer to a TNode, a Data*
 * output: none
 *
 * Stores the passed Data* in the given tree, does not rebalance tree
 */
TNode* insertNode( TNode *root, TNode* newNode )
{
    if( root==NULL )
        return newNode;
    if( newNode->data == root->data ){
        free( newNode );
        return root;
    }
    else if(  newNode->data < root->data ){
        root->pLeft = insertNode( root->pLeft, newNode );
        if( root->pLeft!=NULL )
          root->pLeft->pParent = root;
        return root;
    }
    else{ /* compareData( newNode->data, root->data ) > 0 */
        root->pRight = insertNode( root->pRight, newNode );
        if( root->pRight!=NULL )
          root->pRight->pParent = root;
        return root;
    }
}

/* insertTree
 * input: a pointer to a Tree, a Data*
 * output: none
 *
 * Stores the passed Data* into the Tree following BST order, Does not rebalance tree
 */
void insertTree( Tree *t, int tData )
{
    TNode* newNode = createTNode( );
    newNode->data = tData;
    t->root = insertNode( t->root, newNode );
    updateHeights(newNode);
}

/* insertTreeBalanced
 * input: a pointer to a Tree, a Data*
 * output: none
 *
 * Stores the passed Data* into the Tree following BST order and rebalances the tree
 */
void insertTreeBalanced( Tree *t, int tData )
{
    TNode* newNode = createTNode( );
    newNode->data = tData;
    t->root = insertNode( t->root, newNode );
    updateHeights(newNode);
    rebalanceTree( t, newNode );
}

/* removeTree
 * input: a pointer to a Tree
 * output: a Data*
 *
 * Remove and returns the TNode with the specified key or NULL if its not in the tree
 */
TNode* removeTree( Tree *t, int key )
{
    int temp;
    TNode *ret;
    TNode *del, *update;
    TNode **parentDelPtr;

    temp = key;
    del = searchTree( t, temp );

    if( del == NULL )
        return NULL;
        
    ret = (TNode*) malloc(sizeof(TNode));
    ret->pParent = del->pParent;
    ret->pLeft = del->pLeft;
    ret->pRight = del->pRight;
    ret->data = del->data;
    ret->height = del->height;

    /* Get previous node's pointer to del */
    if( del->pParent==NULL )
        parentDelPtr = &t->root;    /* del is the root */
    else
        parentDelPtr = del->pParent->pLeft==del ? &del->pParent->pLeft : &del->pParent->pRight; /* check if del if left or right child of its parent */

    /* del has no left child */
    if( del->pLeft==NULL ){
        *parentDelPtr = del->pRight;
        if( del->pRight!=NULL )
            del->pRight->pParent = del->pParent;
        update = del->pParent;
        free( del );
    }

    /* del has no right child */
    else if( del->pRight==NULL ){
        *parentDelPtr = del->pLeft;
        if( del->pLeft!=NULL )
            del->pLeft->pParent = del->pParent;
        update = del->pParent;
        free( del );
    }

    /* del has two children */
    else{
        TNode *next = removeNextInorder( &del->pRight );
        update = next->pParent;
        del->data = next->data;
        free(next);
    }

    /* Update the heights and rebalance around the node update */
    updateHeights(update);
    rebalanceTree(t, update);
    return ret;
}

TNode* removeNextInorder( TNode** pRoot ){
    TNode* temp = *pRoot;

    if( temp->pLeft==NULL ){
        *pRoot = temp->pRight;
        if( temp->pRight!=NULL )
            temp->pRight->pParent = temp->pParent;
    }
    else
        temp = removeNextInorder( &temp->pLeft );

    return temp;
}

int subTreeHeight(TNode* root){
    if( root==NULL )
        return 0;
    return root->height;
}

/* updateHeights
 * input: a pointer to a TNode
 * output: none
 *
 * Recursively recomputes the height of the current node and then recurses on the nodes parent
 */
void updateHeights(TNode* root){
    if( root!=NULL ){
        root->height = subTreeHeight(root->pLeft)>subTreeHeight(root->pRight) ? subTreeHeight(root->pLeft) : subTreeHeight(root->pRight);
        root->height = root->height + 1;
        updateHeights( root->pParent );
    }
}

/* rebalanceTree
 * input: a pointer to a tree and a pointer to TNode
 * output: none
 *
 * Should attempt to rebalance the tree starting at x and going up through the root (i.e., until it reaches null).
 * After this function runs, every node should be balanced (i.e. -2 < balance < 2).
 *
 * For additional help in testing set the parameters PRINT_AVL_TREE and PRINT_AVL_ERRORS to true in the driver
 */
void rebalanceTree(Tree* t, TNode* x){
    while (x != NULL) {
	if (getBalance(x) >= 2  ||  getBalance(x) <= -2) {
		TNode* z = NULL; //have to declare outside of if statement, so scope is within use
		if(subTreeHeight(x->pLeft) > subTreeHeight(x->pRight)) {
			z = x->pLeft;
		}
		else {
			z = x->pRight;
		}
				//LL Rotation
	//x and z are both positive in balance factor so we right rotate about x
	if (getBalance(x) >= 2 && getBalance(z)  >= 0) {
		rightRotate(t, x);
	}
				//LR Rotation
	//x is positive and z is negative in balance factor, left rotate about z, then right rotate about x
	else if (getBalance(x) >= 2 && getBalance(z) <= -1) {
		leftRotate(t, z);
		rightRotate(t, x);
	}
				//RR Rotation
	//x is negative and z is negative in balance factor, so we left rotate about x
	else if(getBalance(x) <= -2 && getBalance(z) <= -1) {
		leftRotate(t, x);
	}
				//RL Rotation
	else if (getBalance(x) <= -2 && getBalance(z) >= 0) {
		rightRotate(t, z);
		leftRotate(t, x);
	}
	}
	if (getBalance(x) >= 2 || getBalance(x) <= -2) {
		x = x;
	}
	else {
		x = x->pParent;
	}
    }
}

/* rightRotate and leftRotate
 * input: a pointer to the Tree, a pointer to a TNode
 * output: none
 *
 * Performs specified rotation around a given node and updates the root of the tree if needed
 */
void rightRotate(Tree* t, TNode* oldRoot){
    TNode *newRoot;

    if( oldRoot==NULL ){
        printf("ERROR - Attempting to do a rightRotate on a NULL Root.");
        return;
    }

    newRoot = oldRoot->pLeft;

    if( newRoot==NULL ){
        printf("ERROR - Attempting to do a rightRotate on a node with no left child (i.e. left child is NULL).");
        return;
    }

    /* check whether oldRoot is the root of the entire tree */
    if( oldRoot->pParent!=NULL ){
        if( oldRoot->pParent->pLeft==oldRoot )
            oldRoot->pParent->pLeft = oldRoot->pLeft;
        else
            oldRoot->pParent->pRight = oldRoot->pLeft;
    }
    else
        t->root = newRoot;
    newRoot->pParent = oldRoot->pParent;

    oldRoot->pLeft = newRoot->pRight;
    if( newRoot->pRight!=NULL )
        newRoot->pRight->pParent = oldRoot;

    oldRoot->pParent = newRoot;
    newRoot->pRight = oldRoot;

    updateHeights( oldRoot );
}

void leftRotate(Tree* t, TNode* oldRoot){
    TNode *newRoot;

    if( oldRoot==NULL ){
        printf("ERROR - Attempting to do a leftRotate on a NULL Root.");
        return;
    }

    newRoot = oldRoot->pRight;

    if( newRoot==NULL ){
        printf("ERROR - Attempting to do a leftRotate on a node with no right child (i.e. right child is NULL).");
        return;
    }

    /* check whether oldRoot is the root of the entire tree */
    if( oldRoot->pParent!=NULL ){
        if( oldRoot->pParent->pRight==oldRoot )
            oldRoot->pParent->pRight = oldRoot->pRight;
        else
            oldRoot->pParent->pLeft = oldRoot->pRight;
    }
    else
        t->root = newRoot;
    newRoot->pParent = oldRoot->pParent;

    oldRoot->pRight = newRoot->pLeft;
    if( newRoot->pLeft!=NULL )
        newRoot->pLeft->pParent = oldRoot;

    oldRoot->pParent = newRoot;
    newRoot->pLeft = oldRoot;

    updateHeights( oldRoot );
}

/* getBalance
 * input: a pointer to a TNode
 * output: none
 *
 * Finds the balance of the given node
 */
int getBalance(TNode* root){
    if( root==NULL )
        return 0;
    return subTreeHeight(root->pLeft) - subTreeHeight(root->pRight);
}



/**********  Functions for debugging an AVL tree **********/

/* printTree
 * input: a pointer to a Tree
 * output: none
 *
 * Prints the contents of the tree below the root node
 */
void printTree( TNode* root ){
    int i;
    if(root!=NULL){
        printTree(root->pLeft);
        for( i=1; i<root->height; i++){
            printf("\t");
        }
        printf("%i\n",root->data);
        printTree(root->pRight);

    }
}

/* checkAVLTree
 * input: a pointer to a Tree
 * output: none
 *
 * Prints error messages if there are any problems with the AVL tree
 */
void checkAVLTree(TNode* root){
    if( root!=NULL ){
        if( getBalance(root)>1 ||  getBalance(root)<-1 )
            printf("ERROR - Node %i had balance %d\n",root->data,getBalance(root) );
        if( root->pLeft!=NULL && root->pLeft->pParent!=root )
            printf("ERROR - Invalid edge at %i-%i\n",root->data,root->pLeft->data );
        if( root->pRight!=NULL && root->pRight->pParent!=root )
            printf("ERROR - Invalid edge at %i-%i\n",root->data,root->pRight->data );

        checkAVLTree(root->pLeft);
        checkAVLTree(root->pRight);
    }
}

/**********  Functions for printing a tree **********/

/* printTreeByType
 * input: a pointer to a Tree
 * output: none
 *
 * Prints the contents of the tree below the root node based on the type of data stored in the tree
 */

void printTreeByType( Tree* t, TNode* root, int depth ){
    int i;
    char c = '+';
    if( root!=NULL ){
        printTreeByType( t, root->pRight, depth+1 );
        for( i=0; i<depth; i++){
            printf("\t");
        }
        if(root->height==1)
            c = '-';

        if( depth == 0 )
            printf("R");
        else if( root->pParent->pLeft == root )
            printf("\\");
        else
            printf("/");


        printf("%i\n",root->data);

 
        printTreeByType( t, root->pLeft, depth+1 );

    }
}
