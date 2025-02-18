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

class ratioCombo
{
public:
	int indexX_1, indexY_1, indexZ_1;
	int indexX_2, indexY_2, indexZ_2;
};


int factorial(int n)
{
	if (n > 1)
		return n * factorial(n - 1);
	else
		return 1;
}


int main()
{
	ifstream inputFile;
	string filenameRoot;

	string inputFileName;
	string outputFileName;
	string fileNumString;
	
	int numOfFiles, numOfIndices;

	int numOfDataPoints, simulationNum;
	float valueX, spacingValueX, valueY, spacingValueY;
	float energyX, energyY;

	aspectInfo*** lengthData;

	string units;
	char lineBreak;
	bool errorSignal = false;

	int numIndicesToPlot;
	int numCombinations;
	int* indexResponse;

	float*** aspectRatioValues;

	int indexLocation;
	int comboCount;
	ratioCombo* comboValues;

	// Number of files in directory to sort through
	cout << "How many files would you like to process (AKA outer loop number - starts from 0)? ";
	cin >> numOfFiles;
	cout << "\n";

	// Name of file to extract data from
	cout << "What is the root name of your file (filename after number)? ";
	cin >> filenameRoot;
	cout << "\n";

	// Number of indices in file
	cout << "How many indices are in your file? ";
	cin >> numOfIndices;
	cout << "\n";

	// Number of data points to plot on graph
	cout << "How many data points would you like to plot (AKA inner loop number - starts from 0)? ";
	cin >> numOfDataPoints;
	cout << "\n";

	// Starting value for x axis
	cout << "What is your starting energy value on the x-axis (kcal/mol)? ";
	cin >> valueX;
	cout << "\n";

	// Spacing of x axis parameter
	cout << "What is your energy spacing for the x-axis (kcal/mol)? ";
	cin >> spacingValueX;
	cout << "\n";

	// Starting value for y axis
	cout << "What is your starting energy value on the y-axis (kcal/mol)? ";
	cin >> valueY;
	cout << "\n";

	// Spacing of y axis parameter
	cout << "What is your energy spacing for the y-axis (kcal/mol)? ";
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
			errorSignal = false;
			
			// Reading in file data - simulation number
			inputFile >> simulationNum;
//			cout << "Simulation number: " << simulationNum << "\n";
			
			inputFile.get(lineBreak);

//			cout << "Line break 1: " << lineBreak << "\n";
			inputFile.get(lineBreak);
//			cout << "Line break 2: " << lineBreak << "\n";
			inputFile.unget();

			if (lineBreak == '\n')
			{
				errorSignal = true;
				cout << "ERROR.Missing data point ? \n";
			}


			for (int kk = 0; kk < numOfIndices; kk++)
			{
				// Reading in file data - miller indices and length

				if (errorSignal)
				{
					for (int ll = 0; ll < numOfIndices; ll++)
					{
						lengthData[ii][jj][ll].indexX = 0;
						lengthData[ii][jj][ll].indexY = 0;
						lengthData[ii][jj][ll].indexZ = 0;
						lengthData[ii][jj][ll].length = 0;
					}
					break;
				}

				inputFile >> lengthData[ii][jj][kk].indexX;
				inputFile >> lengthData[ii][jj][kk].indexY;
				inputFile >> lengthData[ii][jj][kk].indexZ;
				inputFile >> lengthData[ii][jj][kk].length;
				inputFile >> units;

				// Account for missing data points
				/*if (lengthData[ii][jj][kk].indexX > 1 || lengthData[ii][jj][kk].indexX < -1)
				{
					cout << "ERROR. Missing data point? \n";
					cout << lengthData[ii][jj][kk].indexX << " Read in \n";

					inputFile.unget();

					errorSignal = true;
					for (int ll = 0; ll < numOfIndices; ll++)
					{
						lengthData[ii][jj][ll].indexX = 0;
						lengthData[ii][jj][ll].indexY = 0;
						lengthData[ii][jj][ll].indexZ = 0;
						lengthData[ii][jj][ll].length = 0;
					}
					break;
				}*/

			}
		}

		inputFile.close();
	}

	// Debug Length array
/*	for (int ii = 0; ii < numOfFiles; ii++)
	{
		for (int jj = 0; jj < numOfDataPoints; jj++)
		{
			for (int kk = 0; kk < numOfIndices; kk++)
			{
				cout << ii << " x " << jj << " x " << kk << " = " << lengthData[ii][jj][kk].length << "\n";
			}
		}
	}*/

	// Allocate number of answers depending on how many the user wants to plot

	cout << "How many ratios do you want to plot? (Maximum of " << numOfIndices << ")\n";
	cin >> numIndicesToPlot;
	indexResponse = new int[numIndicesToPlot];

	//Calculate number of combinations of index ratios to plot

	numCombinations = factorial(numIndicesToPlot) / factorial(numIndicesToPlot - 2);

	comboValues = new ratioCombo[numCombinations];

	cout << "Number of combinations to plot = " << numCombinations << "\n";

	// Loop through number of indices and number from 1 - numOfIndices;

	cout << "Which ratios are you interested in plotting? \n";

	for (int ii = 0; ii < numOfDataPoints; ii++)
	{
		if (lengthData[0][ii][0].indexX != 0 || lengthData[0][ii][0].indexY != 0 || lengthData[0][ii][0].indexZ != 0)
		{
			for (int jj = 0; jj < numOfIndices; jj++)
				cout << jj + 1 << ".) " << lengthData[0][ii][jj].indexX << " " << lengthData[0][ii][jj].indexY << " " << lengthData[0][ii][jj].indexZ << "\n";
			
			indexLocation = ii;
			break;
		}

	}

	//Store user response in array, subtract 1 as arrays start at 0 and options start at 1

	for (int ii = 0; ii < numIndicesToPlot; ii++)
	{
		cin >> indexResponse[ii];
		indexResponse[ii]--;
		cout << "\n";
	}

	// Allocate memory for storing requested aspect ratios

	aspectRatioValues = new float**[numOfFiles];
	for (int ii = 0; ii < numOfFiles; ii++)
	{
		aspectRatioValues[ii] = new float*[numOfDataPoints];

		for (int jj = 0; jj < numOfDataPoints; jj++)
		{
			aspectRatioValues[ii][jj] = new float[numCombinations];
		}
	}

	cout << "Aspect ratio array constructed with dimensions of: " << numOfFiles << " " << numOfDataPoints << " " << numCombinations << "\n";

	// Calculate and save aspect ratios

	for (int ii = 0; ii < numOfFiles; ii++)
	{
		for (int jj = 0; jj < numOfDataPoints; jj++)
		{
			comboCount = 0;

			for (int kk = 0; kk < numIndicesToPlot; kk++)
			{
				for (int ll = 0; ll < numIndicesToPlot; ll++)
				{
					if (kk != ll) 
					{
						comboValues[comboCount].indexX_1 = lengthData[0][indexLocation][indexResponse[kk]].indexX;
						comboValues[comboCount].indexY_1 = lengthData[0][indexLocation][indexResponse[kk]].indexY;
						comboValues[comboCount].indexZ_1 = lengthData[0][indexLocation][indexResponse[kk]].indexZ;
						comboValues[comboCount].indexX_2 = lengthData[0][indexLocation][indexResponse[ll]].indexX;
						comboValues[comboCount].indexY_2 = lengthData[0][indexLocation][indexResponse[ll]].indexY;
						comboValues[comboCount].indexZ_2 = lengthData[0][indexLocation][indexResponse[ll]].indexZ;

//						Debug combinations array
//						cout << "File num " << ii << " Data point " << jj << " : ";
//						cout << comboValues[comboCount].indexX_1 << " ";
//						cout << comboValues[comboCount].indexY_1 << " ";
//						cout << comboValues[comboCount].indexZ_1 << " / ";
//						cout << comboValues[comboCount].indexX_2 << " ";
//						cout << comboValues[comboCount].indexY_2 << " ";
//						cout << comboValues[comboCount].indexZ_2 << " = ";
//						cout << lengthData[ii][jj][indexResponse[kk]].length / lengthData[ii][jj][indexResponse[ll]].length << "\n";
						
//						cout << "File num " << ii << " Data point " << jj << " : ";
//						cout << lengthData[ii][jj][indexResponse[kk]].length / lengthData[ii][jj][indexResponse[ll]].length << "\n";

						if (lengthData[ii][jj][indexResponse[kk]].length != 0 && lengthData[ii][jj][indexResponse[ll]].length != 0)
							aspectRatioValues[ii][jj][comboCount] = lengthData[ii][jj][indexResponse[kk]].length / lengthData[ii][jj][indexResponse[ll]].length;
						else
							aspectRatioValues[ii][jj][comboCount] = 0;

//						cout << "Aspect ratio calculated and saved to : " << ii << "  " << jj << " " << comboCount
//							<< " = " << aspectRatioValues[ii][jj][comboCount] << "\n";

						comboCount++;
					}
				}
			}

		}
	}


	// Debug aspect ratio array
/*	for (int ii = 0; ii < numOfFiles; ii++)
	{
		for (int jj = 0; jj < numOfDataPoints; jj++)
		{
			for (int kk = 0; kk < numCombinations; kk++)
			{
				cout << ii << " x " << jj << " x " << kk << " = " << aspectRatioValues[ii][jj][kk] << "\n";
			}
		}
	}*/


	// Plot 2D first
	// Processed (variable 2 increasing) first
	for (int ii = 0; ii < numOfFiles; ii++)
	{
		fileNumString = to_string(ii);
		fileNumString.append("_");
		outputFileName = filenameRoot;
		outputFileName.erase(outputFileName.end() - 4, outputFileName.end());
		outputFileName = outputFileName.insert(0, fileNumString);
		outputFileName = outputFileName.append("_processed.csv");
		ofstream outputFile(outputFileName);

		outputFile << "Energy (kcal/mol)";
		
		for (int jj = 0; jj < numCombinations; jj++)
		{
			outputFile << " , ";

			outputFile << "[" << comboValues[jj].indexX_1
				<< " " << comboValues[jj].indexY_1
				<< " " << comboValues[jj].indexZ_1
				<< "] / [" << comboValues[jj].indexX_2
				<< " " << comboValues[jj].indexY_2
				<< " " << comboValues[jj].indexZ_2
				<< "]";

		}

		outputFile << "\n";

		energyY = valueY;
		for (int jj = 0; jj < numOfDataPoints; jj++)
		{
			outputFile << energyY << " , ";

			for (int kk = 0; kk < numCombinations; kk++)
			{
				outputFile << aspectRatioValues[ii][jj][kk];
					if (kk < numCombinations - 1)
						outputFile << " , ";
			}
			
			outputFile << "\n";
			energyY += spacingValueY;
		}
		outputFile.close();
	}
	
	// Transposed (variable 1 increasing) second

	for (int ii = 0; ii < numOfDataPoints; ii++)
	{
		fileNumString = to_string(ii);
		fileNumString.append("_");
		outputFileName = filenameRoot;
		outputFileName.erase(outputFileName.end() - 4, outputFileName.end());
		outputFileName = outputFileName.insert(0, fileNumString);
		outputFileName = outputFileName.append("_transposed.csv");
		ofstream outputFile(outputFileName);

		outputFile << "Energy (kcal/mol)";

		for (int jj = 0; jj < numCombinations; jj++)
		{
			outputFile << " , ";

			outputFile << "[" << comboValues[jj].indexX_1
				<< " " << comboValues[jj].indexY_1
				<< " " << comboValues[jj].indexZ_1
				<< "] / [" << comboValues[jj].indexX_2
				<< " " << comboValues[jj].indexY_2
				<< " " << comboValues[jj].indexZ_2
				<< "]";

		}

		outputFile << "\n";

		energyX = valueX;
		for (int jj = 0; jj < numOfFiles; jj++)
		{
			outputFile << energyX << " , ";

			for (int kk = 0; kk < numCombinations; kk++)
			{
				outputFile << aspectRatioValues[jj][ii][kk];

				if (kk < numCombinations - 1)
					outputFile << " , ";
			}

			outputFile << "\n";
			energyX += spacingValueX;
		}
		outputFile.close();
	}

	// 3D plots
	// 1 per combination
	for (int ii = 0; ii < numCombinations; ii++)
	{
		outputFileName = "3d_";

		outputFileName.append("(");
		outputFileName.append(to_string(comboValues[ii].indexX_1));
		outputFileName.append(to_string(comboValues[ii].indexY_1));
		outputFileName.append(to_string(comboValues[ii].indexZ_1));
		outputFileName.append(")");
		outputFileName.append("-");
		outputFileName.append("(");
		outputFileName.append(to_string(comboValues[ii].indexX_2));
		outputFileName.append(to_string(comboValues[ii].indexY_2));
		outputFileName.append(to_string(comboValues[ii].indexZ_2));
		outputFileName.append(")");


		outputFileName = outputFileName.append("_plot.csv");
		ofstream outputFile(outputFileName);

		energyX = valueX;
		outputFile << ",";
		outputFile << "Outer energy (kcal/mol)";
		outputFile << "\n";
		outputFile << ",,";

		for (int jj = 0; jj < numOfFiles; jj++)
		{
			outputFile << energyX;
			energyX += spacingValueX;

			if (jj < numOfFiles - 1)
				outputFile << " , ";
		}
		outputFile << "\n";
		outputFile << "Inner energy (kcal/mol)";
		energyY = valueY;

		for (int jj = 0; jj < numOfDataPoints; jj++)
		{
			outputFile << ",";
			outputFile << energyY;

			for (int kk = 0; kk < numOfFiles; kk++)
			{
				outputFile << ",";
				outputFile << aspectRatioValues[kk][jj][ii];
			}

			outputFile << "\n";
			energyY += spacingValueY;
		}

		outputFile.close();
	}

	for (int ii = 0; ii < numOfFiles; ii++)
	{
		for (int jj = 0; jj < numOfDataPoints; jj++)
		{
			delete[] lengthData[ii][jj];
		}

		delete[] lengthData[ii];
	}
	delete[] lengthData;
	delete[] indexResponse;
	delete[] comboValues;

	for (int ii = 0; ii < numOfFiles; ii++)
	{
		for (int jj = 0; jj < numOfDataPoints; jj++)
		{
			delete[] aspectRatioValues[ii][jj];
		}

		delete[] aspectRatioValues[ii];
	}
	delete[] aspectRatioValues;

	
	return 0;
}

