
#include <iostream>
#include "DobleFor.h"

using namespace std;

int main()
{
	int mat[5][5];
	DobleFor df_input(0, 0, 5, 5);
	int i, j;
	cout << "This program creates and prints a 5x5 matrix where each entry is the sum of its coordinates\n";
	while(df_input.getIndexes(i, j))
	{
		mat[i][j] = i + j; 
	}
	DobleFor df_output(0,0,5,5);
	while(df_output.getIndexes(i, j))
	{
		cout << mat[i][j] << " ";
		if(j == 4) cout << endl;
	}
	
	return 0;
}
