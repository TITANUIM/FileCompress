#include <iostream>
using namespace std;
#include<time.h>
#include<assert.h>
#include<vector>

#include "compress.h"
#include "Huffman.h"
#include "Heap.h"

int main()
{
   // define a count to count the  compress time and uncompress time 
	double t1;
	double t2;

	FileComparess f;

	//f.Compress("input.txt"); // small file
	f.Comparess("Input.BIG"); // more bigger file
	t1 = clock(); // Count function 
	printf("Compress time: %f s \n", t1 / CLOCKS_PER_SEC);
	cout << endl;

	//f.UnCompress("input.txt"); 
	f.UnComparess("Input.BIG");
	t2 = clock();
	printf("UnCompress time: %f s \n", t2 / CLOCKS_PER_SEC);

	system("pause");
	return 0;
}


