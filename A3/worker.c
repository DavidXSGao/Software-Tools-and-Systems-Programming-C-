#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <math.h>
#include <float.h>
#include "worker.h"


/*
 * Read an image from a file and create a corresponding 
 * image struct 
 */

Image* read_image(char *filename)
{
        FILE *source_file;
		char head[3];
		int row, col, mcv;
		if ((source_file = fopen(filename, "r")) == NULL){
			fprintf(stderr, "Error opening file\n");
			perror("error: ");
			return NULL;
		}
		
		fscanf(source_file, "%s", head);
		fscanf(source_file, "%d %d", &col, &row);
		fscanf(source_file, "%d", &mcv);
		
		if (strcmp(head, "P3") != 0){
			return (NULL);
		}
		
		Image *image = malloc(sizeof(Image));
		if (image == NULL){
			perror("Malloc\n");
			exit(1);
		}
		
		image->width = col;
		image->height = row;
		image->max_value = mcv;
		image->p = malloc(sizeof(Pixel) * row * col);
		
		
		Pixel *pixies = malloc(row * col * sizeof(Pixel));
		if (pixies == NULL){
			perror("Malloc\n");
			exit(1);
		}
		int counter = 0;
		for (int i = 0; i < 3 * row * col; i+=3){
			int r, g, b;
			fscanf(source_file,"%d\n",&r);
			fscanf(source_file,"%d\n",&g);
			fscanf(source_file,"%d\n",&b);
			pixies[counter].red = r;
			pixies[counter].green = g;
			pixies[counter].blue = b;
			counter ++;
		}
		image->p = pixies;
		fclose(source_file);
	return image;
}

/*
 * Print an image based on the provided Image struct 
 */

void print_image(Image *img){
        printf("P3\n");
        printf("%d %d\n", img->width, img->height);
        printf("%d\n", img->max_value);
       
        for(int i=0; i<img->width*img->height; i++)
           printf("%d %d %d  ", img->p[i].red, img->p[i].green, img->p[i].blue);
        printf("\n");
}

/*
 * Compute the Euclidian distance between two pixels 
 */
float eucl_distance (Pixel p1, Pixel p2) {
        return sqrt( pow(p1.red - p2.red,2 ) + pow( p1.blue - p2.blue, 2) + pow(p1.green - p2.green, 2));
}

/*
 * Compute the average Euclidian distance between the pixels 
 * in the image provided by img1 and the image contained in
 * the file filename
 */

float compare_images(Image *img1, char *filename) {
	float toteuclo = 0;
	Image* compimage = read_image(filename);
	int magicalindex = 0;
	if (img1->width != compimage->width || img1->height != compimage->height){
		return FLT_MAX;
	}else{
		float euclo;
		while(magicalindex != (img1->height)*(img1->width)){
			euclo = eucl_distance(img1->p[magicalindex], compimage->p[magicalindex]);
			toteuclo += euclo;
			magicalindex ++;
		}
	}
	
    return (toteuclo/((img1->height)*(img1->width)));
}

/* process all files in one directory and find most similar image among them
* - open the directory and find all files in it 
* - for each file read the image in it 
* - compare the image read to the image passed as parameter 
* - keep track of the image that is most similar 
* - write a struct CompRecord with the info for the most similar image to out_fd
*/
CompRecord process_dir(char *dirname, Image *img, int out_fd){

    CompRecord CRec;
	DIR *derp;
	char path[PATHLENGTH];
	char fname[PATHLENGTH];
	float smallest = FLT_MAX;
	
	if((derp = opendir(dirname)) == NULL) {
		perror("opendir");
		exit(1);
	}

	struct dirent *dp;
	while((dp = readdir(derp)) != NULL) {

		float euklidlen = 0;

		if(strcmp(dp->d_name, ".") == 0 || 
		   strcmp(dp->d_name, "..") == 0 ||
		   strcmp(dp->d_name, ".svn") == 0){
			continue;
		}
		
		strncpy(path, dirname, PATHLENGTH);
		strncat(path, "/", PATHLENGTH - strlen(path) - 1);
		strncat(path, dp->d_name, PATHLENGTH - strlen(path) - 1);
		
		struct stat sbuf;
		if(stat(path, &sbuf) == -1) {
			//This should only fail if we got the path wrong
			// or we don't have permissions on this entry.
			perror("stat");
			exit(1);
		} 

		// Only call process_dir if it is a directory
		// Otherwise ignore it.
		char img_path[PATHLENGTH];
		strcpy(img_path, dirname);
		strcat(img_path, "/");
		strcat(img_path, dp->d_name);
		
		
		if(S_ISREG(sbuf.st_mode)) {
			euklidlen = compare_images(img, img_path);
			if (euklidlen < smallest){
				smallest = euklidlen;
				strcpy(fname, dp->d_name);
			}
		}
		
	}
	strcpy(CRec.filename, fname);
	CRec.distance = smallest;

    return CRec;
}









