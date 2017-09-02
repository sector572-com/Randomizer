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
Version:	1.1.0
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

#define _APP_NAME_ "randomizer"
#define _VERSION_ "v1.1.0"

using namespace std;

void usage(char* command);

int main(int argc, char* argv[])
{
	int rtn = 0;
	int iterations = 1;
	int arg = 0;
	int error = 0;
	vector<int> lines;
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
				#ifdef _DEBUG_
				cout << "Iteration: " << (i + 1) << endl;
				#endif

				for(int r = lines.size() - 1; r > 0; r--)
				{
					int j = rand() % r;

					if(j > -1)
					{
						int val1 = lines[r];
						int val2 = lines[j];

						lines[r] = val2;
						lines[j] = val1;
					}
				}
			}

			inFile.clear();

			ofstream outFile(outputFileName, ofstream::out);

			if(outFile)
			{
				for(int i = 0; i < lines.size(); i++)
				{
					int p = lines[i];

					string line;
					inFile.seekg(p);
					getline(inFile, line);
					outFile << line << endl;
				}

				outFile.close();
			}
		}

		inFile.close();
		inFile.clear();

		#ifdef _DEBUG_
		cout << "Processing finished." << endl;
		#endif
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
	cout << _APP_NAME_ << " " << _VERSION_ << endl << endl;
	cout << "Usage: " << endl;
	cout << command << " -f <file name> -o <output file name> "
	     << "[-i <iterations>]" << endl;
}
