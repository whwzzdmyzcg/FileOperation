/*
 * =====================================================================================
 *
 *       Filename:  FileOperation.cc
 *
 *    Description:  命令行界面操作选择
 *
 *        Version:  1.0
 *        Created:  10/20/2012 12:31:22 PM
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  Rachel 
 *   Organization:  
 *
 * =====================================================================================
 */
#include <unistd.h>//fork
#include <sys/types.h>//fork,waitpid
#include <sys/wait.h>//waitpid
#include <stdio.h>//fprintf
#include <stdlib.h>//exit
#include <errno.h>//perror

#include <iostream>

#define MAX 128

using std::cout;
using std::cin;
using std::endl;
using std::string;

int main(int argc, char **argv)
{
	pid_t	pid;
	int selection;
	bool flag = true;

	char *dir = NULL;
	char *suffix = NULL;
	char *target = NULL;

	dir = (char*)malloc(sizeof(char)*MAX);
	suffix = (char*)malloc(sizeof(char)*MAX);
	target = (char*)malloc(sizeof(char)*MAX);

	while (flag)
	{
		cout << "Please select an operation" << endl << "0 for Exiting" << endl << "1 for Extracting File Names From A Directory" << endl
			 << "2 for Deleting The Same Entry Of A File" << endl << "3 for Sorting The Entries Of A File" << endl;
		cin >> selection;

		switch(selection)
		{
			case 0:
				flag = false;
				break;
			case 1:
				cout << "Directory:";
				cin >> dir;
				cout << "Suffix:";
				cin >> suffix;
				cout << "Target Filename:";
				cin >> target;
				if ((pid = fork()) < 0)
					goto err_handle;
				else if (pid == 0) // child process
				{
					if (execl("GatherFilename", "GatherFilename", dir, suffix, target, (char *)0) < 0)	            
						goto err_handle;
				}
				// parent process
				if (waitpid(-1, NULL, 0) < 0)
					goto err_handle;
				break;
			case 2:
				cout << "Filename:";
				cin >> target;
				if ((pid = fork()) < 0)
					goto err_handle;
				else if (pid == 0) // child process
				{
					if (execl("DelRepetition", "DelRepetition", target, (char *)0) < 0)	            
					{
#ifndef NDEBUG
						cout << "execl" << endl;
#endif
						goto err_handle;
					}
				}
				// parent process
				if (waitpid(-1, NULL, 0) < 0)
					goto err_handle;
				break;
			case 3:
				cout << "Filename:";
				cin >> target;
				if ((pid = fork()) < 0)
					goto err_handle;
				else if (pid == 0) // child process
				{
					if (execl("Sort", "Sort", target, (char *)0) < 0)	            
						goto err_handle;
				}
				// parent process
				if (waitpid(-1, NULL, 0) < 0)
					goto err_handle;
				break;
			default:
				break;
		}
	}

	free(dir);
	free(suffix);
	free(target);
	return 0;

err_handle:
#ifndef NDEBUG
	cout << "err_handle" << endl;
#endif

	free(dir);
	free(suffix);
	free(target);
	perror(argv[0]);
	return -1;
}
