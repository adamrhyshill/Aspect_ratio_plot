// Aspect_ratio_plot.cpp : Defines the entry point for the console application.
//
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

class aspectInfo
{
public:
	int indexX, indexY, indexZ;
	float length;
};

int whichIndex(int x, int y, int z)
{
	if (x == 1 || x == -1)
		return 1;
	else if (y == 1 || y == -1)
		return 2;
	else if (z == 1 || z == -1)
		return 3;
	else
		return 0;
}

int main()
{
	ifstream inputFile;
	string inputFileName;
	string outputFileName;
	int numOfDataPoints, fileNum;
	float valueX, spacingValueX;

	aspectInfo aspectIndex[6];
	string units;
	string lineBreak;
	bool errorSignal = false;

	float crystalInfo[3], aspectratios[3];

	// File name to search for aspect ratio data
	cout << "What file would you like to process? ";
	cin >> inputFileName;
	cout << "\n";
	inputFile.open(inputFileName, ifstream::binary);
	inputFileName.erase(inputFileName.end()-4,inputFileName.end());

	// Create output file for plotting
	outputFileName = inputFileName.append("_processed.csv");
	cout << outputFileName << "\n";
	ofstream outputFile(outputFileName);

	// Number of data points to plot on graph
	cout << "How many data points would you like to plot? ";
	cin >> numOfDataPoints;
	cout << "\n";

	// Starting value for x axis
	cout << "What is your starting value on the x-axis? ";
	cin >> valueX;
	cout << "\n";

	// Spacing of x axis parameter
	cout << "What is your spacing for the x-axis? ";
	cin >> spacingValueX;
	cout << "\n";

	// Column headers
	outputFile << "X axis , a/b ratio , c/a ratio , c/b ratio \n";

	for (int ii = 0; ii < numOfDataPoints; ii++)
	{
		// Reading in file data - simulation number

		inputFile >> fileNum;
		cout << "File number: " << fileNum << "\n";

		for (int jj = 0; jj < 6; jj++)
		{
			// Reading in file data - miller indices and length

			inputFile >> aspectIndex[jj].indexX;
			
			// Account for missing data points
			if (aspectIndex[jj].indexX > 1 || aspectIndex[jj].indexX < -1)
			{
				cout << "ERROR. Missing data point? \n";
				inputFile.unget();
				errorSignal = true;
				break;
			}
			inputFile >> aspectIndex[jj].indexY;
			inputFile >> aspectIndex[jj].indexZ;
			inputFile >> aspectIndex[jj].length;
			inputFile >> units;


			cout << "Index X: " << aspectIndex[jj].indexX << "\n";
			cout << "Index Y: " << aspectIndex[jj].indexY << "\n";
			cout << "Index Z: " << aspectIndex[jj].indexZ << "\n";
			cout << "Index Length: " << aspectIndex[jj].length << "\n";
			cout << "" << units << "\n";

			// Reading in file data - miller indices and length

			if (whichIndex(aspectIndex[jj].indexX, aspectIndex[jj].indexY, aspectIndex[jj].indexZ) == 1)
				crystalInfo[0] = aspectIndex[jj].length;
			else if (whichIndex(aspectIndex[jj].indexX, aspectIndex[jj].indexY, aspectIndex[jj].indexZ) == 2)
				crystalInfo[1] = aspectIndex[jj].length;
			else if (whichIndex(aspectIndex[jj].indexX, aspectIndex[jj].indexY, aspectIndex[jj].indexZ) == 3)
				crystalInfo[2] = aspectIndex[jj].length;
			else
			{
				cout << "ERROR. Is your file format correct? \n";
				return 0;
			}
		}

		if (!errorSignal)
		{
			aspectratios[0] = crystalInfo[0] / crystalInfo[1];
			aspectratios[1] = crystalInfo[2] / crystalInfo[0];
			aspectratios[2] = crystalInfo[2] / crystalInfo[1];

			cout << "Aspect ratios - a/b = " << aspectratios[0] << " c/a = " << aspectratios[1] << " c/b = " << aspectratios[2] << "\n";

			outputFile << valueX << " , " << aspectratios[0] << " , " << aspectratios[1] << " , " << aspectratios[2] << "\n";
		}
		else
			outputFile << valueX << ",  0,  0,  0 \n";

		valueX += spacingValueX;
		errorSignal = false;

	}
	
	return 0;
}

