#include <stdlib.h>
#include <stdio.h>

/*
Partner: Xander Wilson
Big O Time Complexity of below algorithm: O(n+k), where k is the largest number given.
Big O Space Complexity of below algorithm: O(n+k), where k is the largest number given.
*/

int main(){

	int inputArrays[2][10] = {{2,5,6,3,14,7,4,12,15,8},
	                          {0,5,7,10,6,15,20,9,3,2}};
	int targets[2] = {24,30};
	
	
	int max;
	int numVals = 10;
	int numSets = 2;
	
	int* sortedArr = (int*) calloc(numVals, sizeof(int));
	
	int i,j;
	for (i = 0; i < numSets; i++) {
		
		max = inputArrays[i][0];
		
		///// counting sort
		
		for (j = 0; j < numVals; j++) if(inputArrays[i][j] > max) max = inputArrays[i][j]; //iterate through current array to find max val                      O(n)
		int* countArr = (int*) calloc(max + 1, sizeof(int)); //prep second array (which feels like a hash with h(k) = k? anyways.)                              O(k+1), where k is the largest number given
		
		for (j = 0; j < numVals; j++) countArr[inputArrays[i][j]]++; //count occurences in first array                                                          O(n)
		for (j = 1; j < max + 1; j++) countArr[j] += countArr[j-1]; //convert count array into list of indices                                                  O(k+1)
		
		for (j = 0; j < numVals; j++) sortedArr[countArr[inputArrays[i][j]] - 1] = inputArrays[i][j]; //populate final array with sorted data                   O(n)
		
		///// end counting sort
		
		for (j = max + 1; j > 0; j--) countArr[j] -= countArr[j-1]; //re-convert count array back into counts instead of a list of indices                      O(k+1)
		
		for (j = 0; j < (numVals+1)/2-1 ; j++) { //(numVals+1)/2 to normalize behavior in case of odd input, -1 to adjust for 0 indexing.                       O(.5n)
			
			int curNum = sortedArr[j];

			if (curNum == 0 || targets[i] % curNum != 0) continue;
			if (countArr[targets[i]/curNum] == 0) continue;
			
			printf("(%d,%d) ", curNum, targets[i]/curNum);
		}
		
		printf("\n");

		free(countArr);
	}
	
	free(sortedArr);
	
	return 0;
}