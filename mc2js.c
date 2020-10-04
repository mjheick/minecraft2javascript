/**
 * mc2js / minecraft2javascript
 *
 * https://minecraft.gamepedia.com/Anvil_file_format
 * https://minecraft.gamepedia.com/Region_file_format
 * https://minecraft.gamepedia.com/Chunk_format
 * https://minecraft.gamepedia.com/NBT_format
 * https://web.archive.org/web/20110723210920/http://www.minecraft.net/docs/NBT.txt
 */
#include <malloc.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

bool parse_coords(char *s[]);
bool sanityCheckCoords(void);
char *getBlock(long x, long y, long z);
char *getChunk(char *file_contents, long x, long z);
char *getChunkBytes(char *mcaData, long offset, long count);
char *getMcaFilename(long x, long z);
char *readMcaFile(char *filename);
long bytes2num(char *data, long cnt);
void showhelp(void);

/* Our coordinates that we're going to be working with, in real-world units */
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
} _coords;

/* We shouldn't need more than 4 files loaded in memory at /any/ time */
#define MCAFILEMAX 4

struct MCAFiles
{
	char *filename;
	long file_length;
	char *file_contents;
	bool loaded;
} _mcafile[MCAFILEMAX];

int main(int argc, char *argv[])
{
	long x, y, z; /* For looping */
	char *block;
	if ((sizeof x) < 8)
	{
		fprintf(stderr, "Error/Need to run on 64-bit system\n");
		return 1;
	}
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

	/* Initialize mcafile[] */
	for (x = 0; x < MCAFILEMAX; x++)
	{
		_mcafile[x].loaded = false;
	}

	/* Loop through our coordinates and build our stuff */
	for (y = _coords.start_y; y <= _coords.end_y; y++)
	{
		for (x = _coords.start_x; x <= _coords.end_x; x++)
		{
			for (z = _coords.start_z; z <= _coords.end_z; z++)
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

	/* s[1] is going to go to _coords.start_[xyz] */
	token = strtok(s[1], ",");
	if (token == NULL) { return false; }
	_coords.start_x = strtol(token, &endptr, 10);
	token = strtok(NULL, ",");
	if (token == NULL) { return false; }
	_coords.start_y = strtol(token, &endptr, 10);
	token = strtok(NULL, ",");
	if (token == NULL) { return false; }
	_coords.start_z = strtol(token, &endptr, 10);

	/* s[2] is going to go to _coords.end_[xyz] */
	token = strtok(s[2], ",");
	if (token == NULL) { return false; }
	_coords.end_x = strtol(token, &endptr, 10);
	token = strtok(NULL, ",");
	if (token == NULL) { return false; }
	_coords.end_y = strtol(token, &endptr, 10);
	token = strtok(NULL, ",");
	if (token == NULL) { return false; }
	_coords.end_z = strtol(token, &endptr, 10);

	/* s[3] is going to go to _coords.viewer_[xyzd] */
	token = strtok(s[3], ",");
	if (token == NULL) { return false; }
	_coords.viewer_x = strtol(token, &endptr, 10);
	token = strtok(NULL, ",");
	if (token == NULL) { return false; }
	_coords.viewer_y = strtol(token, &endptr, 10);
	token = strtok(NULL, ",");
	if (token == NULL) { return false; }
	_coords.viewer_z = strtol(token, &endptr, 10);
	token = strtok(NULL, ",");
	if (token == NULL) { return false; }
	_coords.direction = strtol(token, &endptr, 10);
	if ((_coords.direction < 0) || (_coords.direction > 360))
	{
		return false;
	}
	return true;
}

/*
 * sanityCheckCoords()
 * Make sure _coords look good, start is < and end is > (looping) and viewer exists between start and finish
 */
bool sanityCheckCoords(void)
{
	long tmp;
	if (_coords.start_x > _coords.end_x)
	{
		tmp = _coords.start_x;
		_coords.start_x = _coords.end_x;
		_coords.end_x = tmp;
	}
	if ((_coords.start_x > _coords.viewer_x) || (_coords.viewer_x > _coords.end_x))
	{
		return false;
	}
	if (_coords.start_y > _coords.end_y)
	{
		tmp = _coords.start_y;
		_coords.start_y = _coords.end_y;
		_coords.end_y = tmp;
	}
	if ((_coords.start_y > _coords.viewer_y) || (_coords.viewer_y > _coords.end_y))
	{
		return false;
	}
	if (_coords.start_z > _coords.end_z)
	{
		tmp = _coords.start_z;
		_coords.start_z = _coords.end_z;
		_coords.end_z = tmp;
	}
	if ((_coords.start_z > _coords.viewer_z) || (_coords.viewer_z > _coords.end_z))
	{
		return false;
	}
	return true;
}

char *getBlock(long x, long y, long z)
{
	char *mcaFilename;
	char *mcaFile;
	char *chunkData;
	mcaFilename = getMcaFilename(x, z); /* Based on x/z we need to be able to calculate the mca file to read */
	mcaFile = readMcaFile(mcaFilename); /* Read the file */
	if (mcaFile == NULL)
	{
		fprintf(stderr, "Error/%s not found for block (%d,%d,%d)\n", mcaFilename, x, y, z);
		return NULL;
	}

	/**
	 * We have a file. Lets see if the chunk data exists within the file.
	 * It'll be an X/Z search, then a Y search within the chunk.
	 */
	chunkData = getChunk(mcaFile, x, z); /* This will translate world x/z into specific chunk coords in mcaFile */
	if (chunkData == NULL)
	{
		fprintf(stderr, "Error/Chunk could not be found in %s for x=%d,z=%d\n", mcaFilename, x, z);
		return NULL;
	}
	return "\0";
}

/**
 * getMcaFilename()
 * Take x and z world coordinates and return back a mca filename to open up
 */
char *getMcaFilename(long x, long z)
{
	/* A chunk is 16x16x16 */
	char *regionFilename = malloc(20); /* r.-xxxxx.-zzzzz.mca\0 = 20 */
	long regionX = (long)(floor((double)x / 512.0));
	long regionZ = (long)(floor((double)z / 512.0));
	sprintf(regionFilename, "r.%d.%d.mca", regionX, regionZ);
	return regionFilename;
}

/**
 * readMcaFile()
 * Reads /technically/ any file into memory, assuming we have free memory
 * We really want a .mca file in the same directory this script is run in
 */
char *readMcaFile(char *filename)
{
	/* We have a global struct to cache loaded files */
	int f;
	FILE *fp;
	long file_length = 0;
	long fread_elements = 0;
	char *file_contents;

	for (f = 0; f < MCAFILEMAX; f++)
	{
		if (_mcafile[f].loaded)
		{
			if (strcmp(_mcafile[f].filename, filename) == 0)
			{
				return _mcafile[f].file_contents;
			}
		}
	}

	fp = fopen(filename, "rb");
	if (fp == NULL)
	{
		fprintf(stderr, "Error/failed to open %s for binary reading\n", filename);
		return NULL;
	}
	fseek(fp, 0L, SEEK_END);
	file_length = ftell(fp);
	rewind(fp);
	file_contents = malloc(file_length);
	if (file_contents == NULL)
	{
		fprintf(stderr, "Error/Could not malloc() %d bytes of memory for loading %s\n", file_length, filename);
		return NULL;
	}

	fread_elements = fread(file_contents, file_length, 1, fp);
	fclose(fp);
	if (fread_elements != 1)
	{
		fprintf(stderr, "Error/Had an issue with reading all the data from %s into memory\n", filename);
		return NULL;
	}

	/* cache this in _mcafile */
	for (f = 0; f < MCAFILEMAX; f++)
	{
		if (!_mcafile[f].loaded)
		{
			_mcafile[f].filename = filename;
			_mcafile[f].file_length = file_length;
			_mcafile[f].loaded = true;
			_mcafile[f].file_contents = file_contents;
			return _mcafile[f].file_contents;
		}
	}
	/* If we got here, we couldn't cache this file in memory. return contents anyways */
	fprintf(stderr, "Error/Had an issue with caching %s, maybe increase MCAFILEMAX(=%d now) and recompile\n", filename, MCAFILEMAX);
	return file_contents;
}

/**
 * Read chunk location and determine where the offset of the chunk data is going to be
 * If chunk doesn't exist, return 0, otherwise, return offset
 */
char *getChunk(char *file_contents, long x, long z)
{
	/* translate x and z to chunk coordinates, 0 >= [x|z] > 31 */
	long chunkX, chunkZ, chunkOffset;
	long loopX, loopZ;
	long chunkDataStart, chunkDataLength;
	long chunkLength, chunkCompression;
	char *chunkData;
	while (x > 511) { x = x - 512; } while (x < -511) { x = x + 512; }
	while (z > 511) { z = z - 512; } while (z < 511) { z = z + 512; }
	chunkX = (long)(floor(x / 16)); chunkZ = (long)(floor(z / 16));
	/* We have to go through the entire file and find out where this chunk data is */
	for (loopZ = 0; loopZ < 31; loopZ++)
	{
		for (loopX = 0; loopX < 31; loopX++)
		{
			chunkOffset = (4 * ((loopX % 32) + (loopZ % 32) * 32)); /* This is the location in the NBT data to find this chunk information */
			chunkDataStart = bytes2num(getChunkBytes(file_contents, chunkOffset, 3), 3) * 4096;
			chunkDataLength = bytes2num(getChunkBytes(file_contents, chunkOffset + 3, 1), 1) * 4096;
			if ((chunkDataStart == 0) && (chunkDataLength == 0))
			{
				continue;
			}
			chunkLength = bytes2num(getChunkBytes(file_contents, chunkDataStart, 4), 4); /* 4 bytes: length of chunk */
			chunkCompression = bytes2num(getChunkBytes(file_contents, chunkDataStart + 4, 1), 1); /* 1 byte: compression of data */
			chunkData = getChunkBytes(file_contents, chunkDataStart + 5, chunkLength); /* chunkData should already be malloc()'d */
			if (chunkCompression == 2) /* gzinflate */
			{
				
			}
			else
			{
				fprintf(stderr, "Error/chunk[%d,%d] is using unsupported compression=%d\n", loopX, loopZ, chunkCompression);
				continue;
			}
		}
	}
	return NULL;
}

/**
 * Extract count bytes from char* starting at offset
 */
char *getChunkBytes(char *mcaData, long offset, long count)
{
	if (count <= 0)
	{
		return NULL;
	}
	char *chunkData = malloc(count);
	mcaData = mcaData + offset;
	memcpy(chunkData, mcaData, count);
	return chunkData;
}

/**
 * Convert a series of cnt bytes into a usable number, big-endian
 */
long bytes2num(char *data, long cnt)
{
	long num = 0, byte, x;
	for (x = 0; x < cnt; x++)
	{
		num <<= 8;
		byte = 0;
		if ((data[x] & 0x80) == 0x80) { byte += 128; }
		if ((data[x] & 0x40) == 0x40) { byte += 64; }
		if ((data[x] & 0x20) == 0x20) { byte += 32; }
		if ((data[x] & 0x10) == 0x10) { byte += 16; }
		if ((data[x] & 0x08) == 0x08) { byte += 8; }
		if ((data[x] & 0x04) == 0x04) { byte += 4; }
		if ((data[x] & 0x02) == 0x02) { byte += 2; }
		if ((data[x] & 0x01) == 0x01) { byte += 1; }
		num |= byte;
	}
	return num;
}