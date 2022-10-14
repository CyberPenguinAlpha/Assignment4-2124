#include <stdlib.h>
#include <stdio.h>

#include "tree.h"

/*TODO: Notate if you used a partner and the runtime of your implemented algorithm
Partner: Xander Wilson
Big O Time Complexity of below algorithm: O(logn)
Big O Space Complexity of below algorithm: O(n)
*/

TNode* nextInOrder(TNode*);

int main(){

	int i = 0, j = 0;
	int input[2][10] = {{2,3,4,5,6,7,8,12,14,15},
	                    {0,2,3,5,6,7,9,10,15,20}};
	int k[2] = {3,8};
	
	Tree* t;
	for(i = 0; i < 2; i++){
		t = createTree();
		for(j = 0; j < 10; j++){
			insertTreeBalanced(t,input[i][j]);
		}
		
		TNode* curNode = t->root;
		while (curNode->pLeft != NULL) curNode = curNode->pLeft; //traverse to "index 0"                        in an avl, should be O(logn) plus or minus 1
		
		for(j = 0; j < k[i]; j++) curNode = nextInOrder(curNode); //inorder traverse to desired node            O(klogn), where k is the desired index.
		printf("%d\n", curNode->data);
		
		freeTree(t);
	}

	return 0;
}

TNode* nextInOrder(TNode* curNode){
	
	if(curNode->pRight != NULL){
		
		curNode = curNode->pRight; //right once                                                                 O(1)
		while (curNode->pLeft != NULL) curNode = curNode->pLeft; //left all the way down                        O(logn)
		
		return curNode;
	}

	int old = curNode->data;
	
	do{ // traverse to youngest ancestor larger than original node	                                            O(logn)
		if(curNode->pParent == NULL) return NULL; //checks if we're at root
		curNode = curNode->pParent; 
	} while (curNode->data < old);
	
	return curNode;
}