/**
 * mc2js / minecraft2javascript
 */
#include <malloc.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

bool parse_coords(char *s[]);
bool sanityCheckCoords(void);
char *getBlock(long x, long y, long z);
char *getMcaFilename(long x, long z);
char *readMcaFile(char *filename);
void showhelp(void);

struct Coords_struct
{
	long start_x;
	long start_y;
	long start_z;
	long end_x;
	long end_y;
	long end_z;
	long viewer_x;
	long viewer_y;
	long viewer_z;
	long direction;
} coords;

int main(int argc, char *argv[])
{
	long x, y, z; /* For looping */
	char *block;
	if (argc != 4)
	{
		showhelp();
		return 1;
	}
	if (!parse_coords(argv))
	{
		fprintf(stderr, "Error/Problem with command-line arguments\n");
		showhelp();
		return 1;
	}

	if (!sanityCheckCoords())
	{
		fprintf(stderr, "Error/Coordinate issue. Viewer most likely not within bounding cube.\n");
		showhelp();
	}
	for (y = coords.start_y; y <= coords.end_y; y++)
	{
		for (x = coords.start_x; x <= coords.end_x; x++)
		{
			for (z = coords.start_z; z <= coords.end_z; z++)
			{
				block = getBlock(x, y, z);
			}
		}
	}
	return 0;
}

/**
 * showhelp()
 * Shows the help if we didn't get enough parameters
 */
void showhelp(void)
{
	char *help = "\
Usage: mc2js [s_xyz] [e_xyz] [v_xyzd]\n\n\
Mandatory arguments:\n\
  s_xyz    Comma-separated X,Y,Z coordinates of where to start extracting world data\n\
  e_xyz    Comma-separated X,Y,Z coordinates of where to finish extracting world data\n\
  v_xyzd   Comma-separated X,Y,Z coordinates of where to finish extracting world data including direction\n\
           Direction is a number between 0-360 for degrees\n\n\
Example:\n\
  mc2js 1,1,1 9,9,9 5,5,5,0\n\
";
	printf("%s", help);
	return;
}

/**
 * parse_coords()
 * parses command-line arguments into numerical things
 */
bool parse_coords(char *s[])
{
	char **endptr;
	char *token = "\0";

	/* s[1] is going to go to coords.start_[xyz] */
	token = strtok(s[1], ",");
	if (token == NULL) { return false; }
	coords.start_x = strtol(token, &endptr, 10);
	token = strtok(NULL, ",");
	if (token == NULL) { return false; }
	coords.start_y = strtol(token, &endptr, 10);
	token = strtok(NULL, ",");
	if (token == NULL) { return false; }
	coords.start_z = strtol(token, &endptr, 10);

	/* s[2] is going to go to coords.end_[xyz] */
	token = strtok(s[2], ",");
	if (token == NULL) { return false; }
	coords.end_x = strtol(token, &endptr, 10);
	token = strtok(NULL, ",");
	if (token == NULL) { return false; }
	coords.end_y = strtol(token, &endptr, 10);
	token = strtok(NULL, ",");
	if (token == NULL) { return false; }
	coords.end_z = strtol(token, &endptr, 10);

	/* s[3] is going to go to coords.viewer_[xyzd] */
	token = strtok(s[3], ",");
	if (token == NULL) { return false; }
	coords.viewer_x = strtol(token, &endptr, 10);
	token = strtok(NULL, ",");
	if (token == NULL) { return false; }
	coords.viewer_y = strtol(token, &endptr, 10);
	token = strtok(NULL, ",");
	if (token == NULL) { return false; }
	coords.viewer_z = strtol(token, &endptr, 10);
	token = strtok(NULL, ",");
	if (token == NULL) { return false; }
	coords.direction = strtol(token, &endptr, 10);
	if ((coords.direction < 0) || (coords.direction > 360))
	{
		return false;
	}
	return true;
}

/*
 * sanityCheckCoords()
 * Make sure coords look good, start is < and end is > (looping) and viewer exists between start and finish
 */
bool sanityCheckCoords(void)
{
	long tmp;
	if (coords.start_x > coords.end_x)
	{
		tmp = coords.start_x;
		coords.start_x = coords.end_x;
		coords.end_x = tmp;
	}
	if ((coords.start_x > coords.viewer_x) || (coords.viewer_x > coords.end_x))
	{
		return false;
	}
	if (coords.start_y > coords.end_y)
	{
		tmp = coords.start_y;
		coords.start_y = coords.end_y;
		coords.end_y = tmp;
	}
	if ((coords.start_y > coords.viewer_y) || (coords.viewer_y > coords.end_y))
	{
		return false;
	}
	if (coords.start_z > coords.end_z)
	{
		tmp = coords.start_z;
		coords.start_z = coords.end_z;
		coords.end_z = tmp;
	}
	if ((coords.start_z > coords.viewer_z) || (coords.viewer_z > coords.end_z))
	{
		return false;
	}
	return true;
}

char *getBlock(long x, long y, long z)
{
	/* Based on x/z we need to be able to calculate the mca file to read */
	char *mcaFilename;
	char *mcaFile;
	mcaFilename = getMcaFilename(x, z);
	mcaFile = readMcaFile(mcaFilename);
	if (mcaFile == NULL)
	{
		fprintf(stderr, "Error/%s not found for block (%d,%d,%d)\n", mcaFilename, x, y, z);
		return NULL;
	}

	/* We have a file. Lets see if the chunk data exists within the file */


	/* Clear up some mallocs() */
	free(mcaFile);
	free(mcaFilename);
	return "\0";
}

/**
 * getMcaFilename()
 * Take x and z world coordinates and return back a mca filename to open up
 */
char *getMcaFilename(long x, long z)
{
	/* A chunk is 16x16x16 */
	char *regionFilename = malloc(20); /* r.-xxxxx.-zzzzz.mcr\0 = 20 */
	double chunkX = floor((double)x / 16.0);
	double chunkZ = floor((double)z / 16.0);
	long regionX = (long)floor(chunkX / 32.0);
	long regionZ = (long)floor(chunkZ / 32.0);
	sprintf(regionFilename, "r.%d.%d.mcr", regionX, regionZ);
	return regionFilename;
}

/**
 * readMcaFile()
 * Reads /technically/ any file into memory, assuming we have free memory
 * We really want a .mca file in the same directory this script is run in
 */
char *readMcaFile(char *filename)
{
	FILE *fp;
	long file_length = 0;
	long fread_elements = 0;
	char *file_contents;

	fp = fopen(filename, "rb");
	if (fp == NULL)
	{
		return NULL;
	}
	fseek(fp, 0L, SEEK_END);
	file_length = ftell(fp);
	rewind(fp);
	file_contents = malloc(file_length);
	if (file_contents == NULL)
	{
		return NULL;
	}

	fread_elements = fread(file_contents, file_length, 1, fp);
	fclose(fp);
	if (fread_elements != 1)
	{
		return NULL;
	}
	return file_contents;
}