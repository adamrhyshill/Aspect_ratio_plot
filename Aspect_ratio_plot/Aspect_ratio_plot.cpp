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
	string filenameRoot;

	string inputFileName;
	string outputFileName;
	string fileNumString;
	
	int numOfFiles, numOfIndices;

	int numOfDataPoints, fileNum;
	float valueX, spacingValueX, valueY, spacingValueY;
	float energyX, energyY;

	aspectInfo*** lengthData;

	string units;
	string lineBreak;
	bool errorSignal = false;

	//float crystalInfo[3], aspectratios[3];


	// Number of files in directory to sort through
	cout << "How many files would you like to process? ";
	cin >> numOfFiles;
	cout << "\n";

	// Name of file to extract data from
	cout << "What is the root name of your file? ";
	cin >> filenameRoot;
	cout << "\n";

	// Number of indices in file
	cout << "How many indices are in your file? ";
	cin >> numOfIndices;
	cout << "\n";

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

	// Starting value for y axis
	cout << "What is your starting value on the y-axis? ";
	cin >> valueY;
	cout << "\n";

	// Spacing of y axis parameter
	cout << "What is your spacing for the y-axis? ";
	cin >> spacingValueY;
	cout << "\n";

	lengthData = new aspectInfo**[numOfFiles];

	for (int ii = 0; ii < numOfFiles; ii++)
	{
		lengthData[ii] = new aspectInfo*[numOfDataPoints];
		
		for (int jj = 0; jj < numOfDataPoints; jj++)
		{
			lengthData[ii][jj] = new aspectInfo[numOfIndices];
		}
	}

	cout << "Array created with dimensions of: " << numOfFiles << " x " << numOfDataPoints << " x " << numOfIndices << "\n";

	for (int ii = 0; ii < numOfFiles; ii++)
	{
		fileNumString = to_string(ii);
		fileNumString.append("_");
		inputFileName = filenameRoot;
		inputFileName = inputFileName.insert(0, fileNumString);
		inputFile.open(inputFileName, ifstream::binary);

		cout << inputFileName << "\n";

		for (int jj = 0; jj < numOfDataPoints; jj++)
		{
			// Reading in file data - simulation number

			inputFile >> fileNum;
			cout << "File number: " << fileNum << "\n";

			for (int kk = 0; kk < numOfIndices; kk++)
			{
				// Reading in file data - miller indices and length

				inputFile >> lengthData[ii][jj][kk].indexX;

				// Account for missing data points
				if (lengthData[ii][jj][kk].indexX > 1 || lengthData[ii][jj][kk].indexX < -1)
				{
					cout << "ERROR. Missing data point? \n";
					inputFile.unget();
					errorSignal = true;
					break;
				}
				inputFile >> lengthData[ii][jj][kk].indexY;
				inputFile >> lengthData[ii][jj][kk].indexZ;
				inputFile >> lengthData[ii][jj][kk].length;
				inputFile >> units;
			}
		}

		inputFile.close();
	}


	// NOW CALCULATE RATIOS


	/*// Create output file for plotting
	inputFileName.erase(inputFileName.end() - 4, inputFileName.end());
	outputFileName = inputFileName;
	outputFileName.append("_processed.csv");
	cout << outputFileName << "\n";
	ofstream outputFile(outputFileName);

	//WRITE DATA TO PROCESSED FILE

	outputFileName = inputFileName;
	outputFileName.append("_transposed.csv");
	cout << outputFileName << "\n";
	ofstream outputFile2(outputFileName);

	//WRITE DATA TO TRANSPOSED FILE

	// Close input file*/


	for (int ii = 0; ii < numOfFiles; ii++)
	{
		energyY = valueY;
		for (int jj = 0; jj < numOfDataPoints; jj++)
		{
			cout << energyY << " ";
			for (int kk = 0; kk < numOfIndices; kk++)
			{
				cout << lengthData[ii][jj][kk].indexX << " ";
				cout << lengthData[ii][jj][kk].indexY << " ";
				cout << lengthData[ii][jj][kk].indexZ << " ";
				cout << lengthData[ii][jj][kk].length << " ";
			}
			cout << "\n";
			energyY += spacingValueY;
		}

		cout << "\n";
	}


	//--------------------------------------------------------------------------------
	/*

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

	}*/
	
	return 0;
}

