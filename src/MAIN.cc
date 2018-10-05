#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <cstring>
#include <unistd.h>
#define BUFFER_SIZE 250
using namespace std; 

int main(int argc, char ** argv)
{
	char init_filename[BUFFER_SIZE];
	char  query_filename[BUFFER_SIZE];
	char unitest_results[BUFFER_SIZE];
	if(argc <5 || argc >7)
	{
		cout<<"Invalid arguments "<<endl;
		return 1;
	}
	for(int i =1 ;i<argc;i++)
	{
		if(!strcmp(argv[i],"-i") )
		{
			strcpy (init_filename,argv[i+1]);
			i++;
		}
		else if(!strcmp(argv[i] , "-q") )
		{
			strcpy(query_filename , argv[i+1]);
			i++;
		}
		else if (!strcmp(argv[i],"-untest" ) )
		{
			strcpy(unitest_results,argv[i+1]);
			i++;
		}
		else 
		{
			cout<<"Unknown flag"<<endl;
			return 1;
		}

	}
	ifstream init_file(init_filename);		//open file to read
	string line;

	std::getline(init_file, line);
	if(line.compare("DYNAMIC")==0)
	{
		cerr<<"DYNAMIC version \n\n";
		execl("dynamic","./dynamic",argv[1] ,argv[2],argv[3],argv[4] ,NULL);
	}
	else 
	{
		cerr<<"STATIC version \n\n";
		execl("static","./static",argv[1] ,argv[2],argv[3],argv[4] ,NULL);

	}


}

