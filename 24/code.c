#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

#define MAXPACKAGE	29

typedef struct _Set {
	int		Nums[MAXPACKAGE];
	int		Num;
	int		Sum;
	uint64_t	Product;
} Set;

void PrintSet(Set *S) {
	for(int i=0;i<S->Num;i++) {
		printf("%i,", S->Nums[i]);
	}
	printf("\n");
}

void AddToSet(Set *S, int n) {
	if(S->Num == MAXPACKAGE) {
		printf("Set overflow!\n");
		exit(1);
	}
	if(S->Product == 0) {
		S->Product = n;
	} else {
		S->Product *= n;
	}
	S->Nums[S->Num++]=n;
	S->Sum+=n;
}

void RemoveFromSet(Set *S, int n) {
	int i;
	for(i=0;i<S->Num;i++) {
		if(S->Nums[i] == n) {
			S->Product /= n;
			S->Sum -= n;
			memmove(&S->Nums[i], &S->Nums[i+1], S->Num - i - 1);
			S->Num--;
			return;
		}
	}
}

Set BestSet;

void combinationUtil(int arr[], int data[], int start, int end, int index, int r, int Target) {
	// Current combination is ready to be printed, print it
	if (index == r) {
		Set Working;

		memset(&Working, 0, sizeof(Working));	
		for (int j=0; j<r; j++) {
			AddToSet(&Working, data[j]);
		}

		if(Working.Sum == Target && (!BestSet.Num || BestSet.Product > Working.Product)) {
			memcpy(&BestSet, &Working, sizeof(BestSet));
		}

		return;
	}
    
	// replace index with all possible elements. The condition
	// "end-i+1 >= r-index" makes sure that including one element
	// at index will make a combination with remaining elements
	// at remaining positions
	
	for (int i=start; i<=end && end-i+1 >= r-index; i++) {
		data[index] = arr[i];
		combinationUtil(arr, data, i+1, end, index+1, r, Target);
	}
}

void MakeBestSet(Set *In, Set *Out, int Target) {
	int num;
	int Buffer[ MAXPACKAGE ];

	memset(&BestSet, 0, sizeof(BestSet));
	for(num=1; num < In->Num; num++) {
		//printf("Checking combinations of %i\n", num);
		combinationUtil(In->Nums, Buffer, 0, In->Num-1, 0, num, Target);
		if(BestSet.Num) {
			memcpy(Out, &BestSet, sizeof(*Out));
			return;
		}
	}
}

int main(int argc, char *argv[]) {
	char buffer[128];
	Set AllNums, Out;

	memset(&AllNums, 0, sizeof(AllNums));
	memset(&Out, 0, sizeof(Out));
	while(fgets(buffer, sizeof(buffer), stdin)) {
		int n=atoi(buffer);
		AddToSet(&AllNums, n);
	}
	printf("Total set size is %i\n", AllNums.Num);
	printf("Master set:\n");
	PrintSet(&AllNums);

	MakeBestSet(&AllNums, &Out, AllNums.Sum / 3);
	printf("\nDay 1, total %i, %i packages, %lu entanglement\n", Out.Sum, Out.Num, Out.Product);
	PrintSet(&Out);

	MakeBestSet(&AllNums, &Out, AllNums.Sum / 4);
	printf("\nDay 2, total %i, %i packages, %lu entanglement\n", Out.Sum, Out.Num, Out.Product);
	PrintSet(&Out);

	return 0;
}
