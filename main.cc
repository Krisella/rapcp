#include <time.h>
#include "copy_functions.h"

int main(int argc,char* argv[])
{
	double bytes_sec=0;
	double tstart,tstop,ttime;
	tstart=(double)clock()/CLOCKS_PER_SEC;
	int flag_v=0;
	int i=1;
	char* origin_name;
	char* dest_name;
	DIR* origin_ptr;
	DIR* dest_ptr;
	while(i<argc)
	{
		string argument(argv[i]);
		if(argument=="-v")
		{
			flag_v=1;
			i++;
		}
		else if(argument=="-l")
		{
			cout<<"The links mode is not available"<<endl;
			cout<<"Program will continue in regular mode"<<endl;
			i++;
		}
		else
		{
			origin_name=argv[i];
			dest_name=argv[i+1];
			i=i+2;
		}
	}
	struct stat originbuf;
	struct stat destbuf;
	if(stat(origin_name,&originbuf)==1)
	{
		perror("Could not get file status");
		exit(15);
	}
	if(stat(dest_name,&destbuf)==1)
	{
		perror("Could not get file status");
		exit(16);
	}
	if((originbuf.st_mode & S_IFMT)==S_IFREG)
	{
		cout<<"Source file not a directory"<<endl;
		return 1;
	}
	if((origin_ptr=opendir(origin_name))==NULL)
	{
		perror("Cannot open source file");
		exit(1);
	}

	verbose v;
	v.byte_count=0;
	v.count=0;
	v.count_copied=0;

	if((dest_ptr=opendir(dest_name))==NULL)
	{
		cout<<"Created directory at "<<dest_name<<endl;
		cp_recurs(origin_ptr,origin_name,dest_name,flag_v,&v);
	}
	else
	{
		closedir(dest_ptr);
		rap_cp(origin_ptr,origin_name,dest_name,flag_v,&v);
	}

	tstop=(double)clock()/CLOCKS_PER_SEC;
	ttime=tstop-tstart;
	if(flag_v==1)
	{
		bytes_sec=(double)v.byte_count/(double)ttime;
		cout<<"There are "<<v.count<<" files/directories in the hierarchy"<<endl;
		cout<<"number of entities copied is "<<v.count_copied<<endl;
		cout<<"copied "<<v.byte_count<<" bytes in "<<ttime<<"sec";
		if(ttime!=0.00)
			cout<<" at "<<fixed<<bytes_sec<<" bytes/sec"<<endl;
		else
			cout<<endl;
	}
}



