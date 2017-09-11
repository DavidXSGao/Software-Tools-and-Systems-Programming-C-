#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <float.h>
#include "worker.h"

int main(int argc, char **argv) {
	
	char ch;
	char path[PATHLENGTH];
	char *startdir = ".";
    char *image_file = NULL;

	while((ch = getopt(argc, argv, "d:")) != -1) {
		switch (ch) {
			case 'd':
			startdir = optarg;
			break;
			default:
			fprintf(stderr, "Usage: queryone [-d DIRECTORY_NAME] FILE_NAME\n");
			exit(1);
		}
	}

        if (optind != argc-1) {
	     fprintf(stderr, "Usage: queryone [-d DIRECTORY_NAME] FILE_NAME\n");
        } else
             image_file = argv[optind];

	// Open the directory provided by the user (or current working directory)
	
	DIR *dirp;
	if((dirp = opendir(startdir)) == NULL) {
		perror("opendir");
		exit(1);
	} 
	DIR *dirp2;
	if((dirp2 = opendir(startdir)) == NULL) {
		perror("opendir");
		exit(1);
	} 
	
	/* For each entry in the directory, eliminate . and .., and check
	* to make sure that the entry is a directory, then call run_worker
	* to process the image files contained in the directory.
	*/
		
	struct dirent *dp;
	struct dirent *dp2;
	Image * initial_image;
	CompRecord baby_Rec;
	strcpy(baby_Rec.filename, "");        
	baby_Rec.distance = FLT_MAX;
	initial_image = read_image(image_file);
	int dirnum = 0;
	while ((dp2 = readdir(dirp2)) != NULL){
		if(strcmp(dp2->d_name, ".") == 0 || 
		    strcmp(dp2->d_name, "..") == 0 ||
		    strcmp(dp2->d_name, ".svn") == 0){
			continue;
		}
		strncpy(path, startdir, PATHLENGTH);
		strncat(path, "/", PATHLENGTH - strlen(path) - 1);
		strncat(path, dp2->d_name, PATHLENGTH - strlen(path) - 1);
		struct stat sbuf;
		if(stat(path, &sbuf) == -1) {
			//This should only fail if we got the path wrong
			// or we don't have permissions on this entry.
			perror("stat");
			exit(1);
		} 
		if (S_ISDIR(sbuf.st_mode)){
		   dirnum++;
		}
	}
	int pipe_fd[dirnum][2];
	int j = 0;
	while((dp = readdir(dirp)) != NULL && j!=dirnum) {
		
		if(strcmp(dp->d_name, ".") == 0 || 
		    strcmp(dp->d_name, "..") == 0 ||
		    strcmp(dp->d_name, ".svn") == 0){
			continue;
		}
		strncpy(path, startdir, PATHLENGTH);
		strncat(path, "/", PATHLENGTH - strlen(path) - 1);
		strncat(path, dp->d_name, PATHLENGTH - strlen(path) - 1);
		
		struct stat sbuf;
		if(stat(path, &sbuf) == -1) {
			//This should only fail if we got the path wrong
			// or we don't have permissions on this entry.
			perror("stat");
			exit(1);
		} 
		
		
		if ((pipe(pipe_fd[j])) == -1) {
			perror("pipe");
			exit(1);
		}
		int result = fork();
		if (result < 0){
			perror("fork");
			exit(1);
		}else if (result == 0){
			//child
			if (close(pipe_fd[j][0]) == -1) {
				
				perror("close reading end from inside child");
			}
			int child_no;
			for (child_no = 1; child_no < j; child_no++) {
				if (close(pipe_fd[child_no][0]) == -1) {
					perror("close reading ends of previously forked children");
				}
			}

			// Only call process_dir if it is a directory
			// Otherwise ignore it.
			if(S_ISDIR(sbuf.st_mode)) {
				printf("Processing all images in directory: %s \n", path);
				baby_Rec = process_dir(path, initial_image, pipe_fd[j][1]);
				if (write(pipe_fd[j][1], &baby_Rec, sizeof(CompRecord)) != sizeof(CompRecord)) {
					perror("write from child to pipe");
				}
				if (close(pipe_fd[j][1]) == -1) {
					perror("close pipe after writing");
				}
			}
			// exit so I don’t fork my own children on next loop iteration
			exit(0);
		}else{
			//single stay at home dad
			if (close(pipe_fd[j][1]) == -1) {
				perror("close writing end of pipe in parent");
			}
		}
		if(S_ISDIR(sbuf.st_mode)) {
			j++;
		}
	}
	CompRecord CRec;
	strcpy(CRec.filename, "");        
	CRec.distance = FLT_MAX;
	CompRecord curr_CRec;	
				
	for (int i=0; i< dirnum; i++) {
		if (read(pipe_fd[i][0], &curr_CRec, sizeof(CompRecord)) == -1) {
			perror("reading from pipe from a child");
		}
		//fprintf(stderr, "I just read a %d from child %d\n", contribution, i);
		if (CRec.distance < 0 || curr_CRec.distance < CRec.distance){
			strcpy(CRec.filename, curr_CRec.filename);
			CRec.distance = curr_CRec.distance;
		}
	}
    printf("The most similar image is %s with a distance of %f\n", CRec.filename, CRec.distance);
	
	return 0;
}