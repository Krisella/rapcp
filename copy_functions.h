#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include "verbose.h"
using namespace std;

//h aplh cp -r
void cp_recurs(DIR*,char*,char*,int,verbose*);

//h cp me elegxous apto source sto target
void cp_replace(DIR*,char*,char*,int,verbose*);

//elegxei poia arxeia/fakeloi den uparxoun sto source kai ta diagrafei apto target
void clean_target(DIR*,char*,char*,int);

//sunolika h rapcp
void rap_cp(DIR*,char*,char*,int,verbose*);

//diagrafei mia oloklhrh ierarxia katalogwn kai fakelwn katw apo enan katalogo
void remove(DIR*,char*);
