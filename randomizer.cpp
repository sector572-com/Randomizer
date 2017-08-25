/*
Copyright 2017 Eddie N. <en@sector572.com>

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

Author:		Eddie N. <en@sector572.com>
Date:		2017-08-25
Filename:	randomizer.cpp
Version:	1.0.0
Description:	This program takes a text file as input and reorders the lines
		in a random order outputting the results to a new file.
*/

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <map>
#include <unistd.h>

using namespace std;

void usage(char* command);

int main(int argc, char* argv[])
{
	int rtn = 0;
	int iterations = 1;
	int arg = 0;
	int error = 0;
	vector<int> lines;
	vector<int>* indexes = NULL;
	map<int, int>* usedIndexes = NULL;
	const char* fileName = NULL;
	const char* outputFileName = NULL;
	srand(time(NULL));

	while(!error && (arg = getopt(argc, argv, "f:i:o:")) != -1)
	{
		switch(arg)
		{
			case 'f':
				if(optarg)
				{
					fileName = optarg;
				}

				break;

			case 'i':
				if(optarg)
				{
					iterations = atoi(optarg);
				}

				break;

			case 'o':
				if(optarg)
				{
					outputFileName = optarg;
				}

				break;

			default:
				error = 1;
				break;
		}
	}

	if(fileName == NULL || outputFileName == NULL || iterations < 1)
	{
		error = 1;
	}

	if(error == 0)
	{
		#ifdef _DEBUG_
		cout << "Input file name:  " << fileName << endl;
		cout << "Output file name: " << outputFileName << endl;
		cout << "Iterations:       " << iterations << endl << endl;
		#endif

		ifstream inFile(fileName);

		if(!inFile)
		{
			cerr << "Unable to open the file " << fileName
				<< "." << endl;
			rtn = 1;
		}
		else
		{
			#ifdef _DEBUG_
			cout << "Processing started." << endl;
			#endif

			string line;
			int pos = 0;
			inFile.seekg(0, inFile.end);
			int length = inFile.tellg();
			inFile.seekg(0);

			while(!inFile.eof())
			{
				pos = inFile.tellg();
				if(pos < length)
				{
					lines.push_back(pos);
				}
				getline(inFile, line);
			}

			for(int i = 0; i < iterations; i++)
			{
				if(indexes != NULL)
				{
					delete indexes;
					indexes = NULL;
				}

				#ifdef _DEBUG_
				cout << "Iteration: " << (i + 1) << endl;
				#endif

				usedIndexes = new map<int, int>();
				indexes = new vector<int>();

				for(int k = 0; k < lines.size(); k++)
				{
					int index = -1;
					do
					{
						index = rand() % lines.size();
					}
					while(usedIndexes->find(index)
						 != usedIndexes->end());

					if(index > -1 && index < lines.size())
					{
						usedIndexes->operator[](index) = 0;
						indexes->push_back(index);
					}
				}

				for(int r = 0; r < lines.size(); r++)
				{
					int val1 = lines[r];
					int val2 = lines[indexes->operator[](r)];

					lines[r] = val2;
					lines[indexes->operator[](r)] = val1;
				}

				delete usedIndexes;
				usedIndexes = NULL;
			}

			inFile.clear();


			if(indexes != NULL)
			{
				ofstream outFile(outputFileName, ofstream::out);

				if(outFile)
				{
					for(vector<int>::iterator i = indexes->begin();
						i != indexes->end();
						++i)
					{
						int p = lines[*i];

						string line;
						inFile.seekg(p);
						getline(inFile, line);
						outFile << line << endl;
					}

					outFile.close();
				}

				delete indexes;
				indexes = NULL;
			}

			inFile.close();
			inFile.clear();

			#ifdef _DEBUG_
			cout << "Processing finished." << endl;
			#endif
		}
	}
	else
	{
		usage(argv[0]);
		rtn = 1;
	}

	return rtn;
}

void usage(char* command)
{
	cout << "Usage: " << endl;
	cout << command << " -f <file name> -o <output file name> "
	     << "[-i <iterations>]" << endl;
}
