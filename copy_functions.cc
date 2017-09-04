
#include "copy_functions.h"

void cp_recurs(DIR* origin_ptr,char* origin,char* dest,int flag,verbose* v)
{
	DIR* sub_ptr;
	DIR* dest_ptr;
	int fd_from,fd_to;
	char buf[4096];
	struct 	dirent *direntp;

	struct stat statbuf;
	if(stat(origin,&statbuf)==1)
	{
		perror("Failed to get file status");
		exit(2);
	}
	mkdir(dest,statbuf.st_mode);
	dest_ptr=opendir(dest);

	while ( ( direntp=readdir(origin_ptr) ) != NULL )
	{
		//enswmatwsh tou onomatos arxeiou sto mexri twra path gia thn dhmiourgia tou neou path
		string str_path(origin);
		str_path=str_path+"/"+direntp->d_name;
		char* filepath=new char[str_path.length()+1];
		strcpy(filepath,str_path.c_str());

		string str_path_d(dest);
		str_path_d=str_path_d+"/"+direntp->d_name;
		char* dest_path=new char[str_path_d.length()+1];
		strcpy(dest_path,str_path_d.c_str());

		cout<<direntp->d_name<<endl;
		if(flag==1)
			v->count++;

		//den antigrafontai kathws dhmiourgountai automata
		string dname(direntp->d_name);
		if(dname=="." || dname=="..")
		{
			delete [] filepath;
			delete [] dest_path;
			continue;
		}

		struct stat mybuf;
		if(stat(filepath,&mybuf)==1)
		{
			perror("Failed to get file status");
			exit(3);
		}

		if ((mybuf.st_mode & S_IFMT) == S_IFDIR )
		{
			if((sub_ptr=opendir(filepath))==NULL)
			{
				perror("Cannot open file");
				exit(4);
			}
			cp_recurs(sub_ptr,filepath,dest_path,flag,v);
		}
		else
		{
			ssize_t nread;
			fd_from=open(filepath, O_RDONLY);
			fd_to=open(dest_path,O_WRONLY|O_CREAT|O_EXCL,mybuf.st_mode);
			while(nread=read(fd_from,buf,sizeof(buf)),nread>0)
			{
				if(nread==0)
				{
					close(fd_to);
					close(fd_from);
					break;
				}
				char* out_ptr=buf;
				ssize_t nwritten;
				nwritten=write(fd_to,out_ptr,nread);

				if(flag==1)
				{
					int temp;
					temp=v->byte_count;
					temp=temp+nwritten;
					v->byte_count=temp;
				}
			}
			if(flag==1)
				v->count_copied++;
		}
		delete [] filepath;
		delete [] dest_path;
	}

	if(closedir(origin_ptr)==-1)
	{
		perror("closedir origin");
		exit(5);
	}
	if(closedir(dest_ptr)==-1)
	{
		perror("closedir dest");
		exit(6);
	}
}

void rap_cp(DIR* origin_ptr,char* origin,char* dest,int flag,verbose* v)
{
	clean_target(origin_ptr,origin,dest,flag);
	cp_replace(origin_ptr,origin,dest,flag,v);
}

void clean_target(DIR* origin_ptr,char* origin,char* dest,int flag)
{
	DIR* dest_ptr;
	DIR* sub_ptr;
	dest_ptr=opendir(dest);
	struct 	dirent *direntp;

	//elegxos apo to target sto source gia arxeia poy den uparxoyn sto source
	while ( ( direntp=readdir(dest_ptr) ) != NULL )
	{
		//enswmatwsh tou onomatos arxeiou sto mexri twra path gia thn dhmiourgia tou neou path
		string str_path(origin);
		str_path=str_path+"/"+direntp->d_name;
		char* filepath=new char[str_path.length()+1];
		strcpy(filepath,str_path.c_str());

		string str_path_d(dest);
		str_path_d=str_path_d+"/"+direntp->d_name;
		char* dest_path=new char[str_path_d.length()+1];
		strcpy(dest_path,str_path_d.c_str());

		string dname(direntp->d_name);
		if(dname=="." || dname=="..")
		{
			delete [] filepath;
			delete [] dest_path;
			continue;
		}

		struct stat origin_stat;
		struct stat dest_stat;
		if(stat(filepath,&origin_stat)==1)
		{
			perror("Failed to get file status");
			exit(7);
		}
		if(stat(dest_path,&dest_stat)==1)
		{
			perror("Failed to get file status");
			exit(8);
		}

		if(access(filepath,F_OK)==0)
		{
			if(((origin_stat.st_mode & S_IFMT) == S_IFDIR) && ((dest_stat.st_mode & S_IFMT)==S_IFDIR))
			{
				if((sub_ptr=opendir(filepath))==NULL)
				{
					perror("Cannot open file");
					exit(9);
				}
				clean_target(sub_ptr,filepath,dest_path,flag);
			}
			else if(((origin_stat.st_mode & S_IFMT) == S_IFREG) && ((dest_stat.st_mode & S_IFMT)==S_IFREG))
			{
				delete [] filepath;
				delete [] dest_path;
				continue;
			}
			else
			{
				if((dest_stat.st_mode & S_IFMT)==S_IFREG)
					remove(dest_path);
				else
					remove(dest_ptr,dest_path);
				cout<<"deleted "<<direntp->d_name<<endl;
			}
		}
		else
		{
			if((dest_stat.st_mode & S_IFMT)==S_IFREG)
				remove(dest_path);
			else
				remove(dest_ptr,dest_path);
			cout<<"deleted "<<direntp->d_name<<endl;
		}
		delete [] filepath;
		delete [] dest_path;
	}
	if(closedir(origin_ptr)==-1)
	{
		perror("closedir origin");
		exit(10);
	}
	if(closedir(dest_ptr)==-1)
	{
		perror("closedir dest");
		exit(11);
	}
}

void cp_replace(DIR* origin_ptr,char* origin,char* dest,int flag,verbose* v)
{
	DIR* sub_ptr;
	int fd_from,fd_to;
	char buf[4096];
	struct 	dirent *direntp;
	opendir(dest);

	while ( ( direntp=readdir(origin_ptr) ) != NULL )
	{
		//enswmatwsh tou onomatos arxeiou sto mexri twra path gia thn dhmiourgia tou neou path
		string str_path(origin);
		str_path=str_path+"/"+direntp->d_name;
		char* filepath=new char[str_path.length()+1];
		strcpy(filepath,str_path.c_str());

		string str_path_d(dest);
		str_path_d=str_path_d+"/"+direntp->d_name;
		char* dest_path=new char[str_path_d.length()+1];
		strcpy(dest_path,str_path_d.c_str());

		if(flag==1)
			v->count++;

		string dname(direntp->d_name);
		if(dname=="." || dname=="..")
		{
			delete [] filepath;
			delete [] dest_path;
			continue;
		}

		struct stat originbuf;
		struct stat destbuf;
		if(stat(filepath,&originbuf)==1)
		{
			perror("Failed to get file status");
			exit(12);
		}
		if(stat(dest_path,&destbuf)==1)
		{
			perror("Failed to get file status");
			exit(13);
		}

		if ((originbuf.st_mode & S_IFMT) == S_IFDIR )
		{
			if((sub_ptr=opendir(filepath))==NULL)
			{
				perror("Cannot open file");
				exit(14);
			}
			if(access(dest_path,F_OK)==-1)
			{
				mkdir(dest_path,originbuf.st_mode);
				cout<<"created directory at "<<dest_path<<endl;
			}
			cp_replace(sub_ptr,filepath,dest_path,flag,v);
		}
		else
		{
			if(originbuf.st_size==destbuf.st_size && difftime(originbuf.st_mtim.tv_sec,destbuf.st_mtim.tv_sec)<0)
				{
					delete [] filepath;
					delete [] dest_path;
					continue;
				}
			else
			{
				ssize_t nread;
				fd_from=open(filepath, O_RDONLY);
				fd_to=open(dest_path,O_WRONLY|O_TRUNC|O_CREAT,originbuf.st_mode);
				while(nread=read(fd_from,buf,sizeof(buf)),nread>0)
				{
					if(nread==0)
					{
						close(fd_to);
						close(fd_from);
						break;
					}
					char* out_ptr=buf;
					ssize_t nwritten;
					nwritten=write(fd_to,out_ptr,nread);
					if(flag==1)
						v->byte_count=v->byte_count+nwritten;
				}
				cout<<"copied "<<direntp->d_name<<endl;
				if(flag==1)
					v->count_copied++;
			}
		}
		delete [] filepath;
		delete [] dest_path;
	}
	if(closedir(origin_ptr)==-1)
	{
		perror("closedir origin");
		exit(10);
	}
}

void remove(DIR* dir_ptr,char* dir_name)
{
	struct dirent *direntp;
	DIR* sub_ptr;
	dir_ptr=opendir(dir_name);

	while((direntp=readdir(dir_ptr))!=NULL)
	{
		string str_path(dir_name);
		str_path=str_path+"/"+direntp->d_name;
		char* filepath=new char[str_path.length()+1];
		strcpy(filepath,str_path.c_str());

		struct stat mybuf;
		if(stat(filepath,&mybuf)==1)
		{
			perror("Failed to get file status");
			exit(17);
		}

		string dname(direntp->d_name);
		if(dname=="." || dname=="..")
		{
			delete [] filepath;
			continue;
		}

		if((mybuf.st_mode & S_IFMT)==S_IFREG)
		{
			remove(filepath);
			delete [] filepath;
			continue;
		}
		else
		{
			if((sub_ptr=opendir(filepath))==NULL)
			{
				perror("Cannot open file");
				exit(18);
			}
			remove(sub_ptr,filepath);
		}
		delete [] filepath;
	}
	closedir(dir_ptr);
	remove(dir_name);
}
