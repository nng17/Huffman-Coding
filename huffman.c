/* Nathan Ng
 * Professor Atkinson
 * Friday Lab 2:15
 * File: huffman.c
 * this program uses Huffman's method to make trees for efficient compression
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "tree.h"
#include "pack.h"
#define size 257

//functions
void insertMin();
struct tree *deleteMin();
void printBit(struct tree *root);

int count[size]; 
struct tree *leaves[size]; //array of leaves
struct tree *heap[size]; //heap array
int length = 0;

/* Function: Main
 * this function reads in the text file and starts the Huffman process
 */

int main(int argc, char *argv[])
{
	FILE *fp1;
	int c, i,j,k,l;
	int characters = 0; //amount of total characters
	int check = 0; //character check
	
	if(argc != 3) //check if there are sufficient arguments
	{
		printf("Missing file\n");
		return -1;
	}

	fp1 = fopen(argv[1], "r"); //opens the file to read
	if (fp1 == NULL) //checking if file is able to read
	{
		printf("Error\n");
		return -1;
	}

	for (i = 0; i < size; i++) //initializes each element in array count to 0
		count[i] = 0;
	
	while((c = getc(fp1)) != EOF) //reads in each character from the file into c and incrememnts the amount of characters
	{
		characters++;
		count[c]++;
	}
	
	for (j = 0; j < size; j++)
		check += count[j]; //checks if the value of the root is the same as the amount of total characters in the file

	if (check != characters) //prints out error if the characters read does not match the total characters
	{
		printf("There has been an error in the counting\n");
		return -1;
	}

	for (k = 0; k < size; k++)
	{
		if (count[k] > 0) //if there are elements in the array
		{
			leaves[k] = createTree(count[k], NULL, NULL); //creating a tree with the element in count
			insertMin(leaves[k]);
		}
		else
			leaves[k] = NULL;
	}

	leaves[size-1] = createTree(0, NULL, NULL);
	insertMin(leaves[size-1]);

	while(length > 1)
	{
		//making the priority queue to forget about the leaves. set pointers from the queue to NULL

		struct tree *min1 = deleteMin();
		struct tree *min2 = deleteMin();
		struct tree *root = createTree(getData(min1) + getData(min2), min1, min2);
		insertMin(root);
	}

	for(l = 0; l < size; l++)
	{
		if(count[l] > 0)
		{
			if(isprint(l) != 0) //if the character can print
			{
				printf("  '%c' : %d ", l, getData(leaves[l]));
			}
			else //if not, print the three digit octal value
			{
				printf(" '0%o': %d ", l, getData(leaves[l]));
			}
			//print out the encodings and a new line

			printBit(leaves[l]);
			printf("\n"); 
		}
	}
	printf(" '%o' : %d ", size-1, getData(leaves[size-1]));
   	printBit(leaves[size-1]);
	printf("\n");
	
	//call pack function
	pack(argv[1], argv[2], leaves);

	destroyTree(heap[0]); //destroy all the trees
	fclose(fp1);
	return 0;

}

/* Function deleteMin
 * has O(nlogn)
 * this function returns the minimum value and reheaps if necessary
 */

struct tree *deleteMin()
{
	int i = 0;
	int child;
	struct tree *min = heap[0];
	struct tree *x = heap[--(length)];
	
	while (i*2+1 < length)
	{
		child = i*2+1;
		if (child + 1 < length && getData(heap[child]) > getData(heap[child+1]))
		{
			child++;
		}
		if (getData(x) > getData(heap[child]))
		{
			heap[i] = heap[child];
			i = child;
		}
		else
			break;
	}
	heap[i] = x;
	return min;
}

/* Function insertMin
 * has O(nlogn)
 * this function inserts a tree into the priority queue, and makes sure the occurences of the parent is less than the tree inserted
 */

void insertMin(struct tree *root)
{
	int i;
	i = (length)++;

	while (i > 0 && getData(heap[(i-1) / 2]) > getData(root))
	{
		heap[i] = heap[(i-1)/2];
		i = (i-1)/2;
	}
	heap[i] = root;
}

/* Function printBit
 * has O(nlogn)
 * this function prints the encoding 
 */

void printBit(struct tree *root)
{
	if(getParent(root) != NULL) //print as long as the root is not NULL
	{
		printBit(getParent(root)); //print the parent's encoding
		if(getLeft(getParent(root)) == root) //print itself
			printf("0");
		else
			printf("1");
	}
}

