/**
 * mc2js / minecraft2javascript
 */
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

void showhelp(void);
bool parse_coords(char *s[]);

struct Coords_struct
{
	signed long start_x;
	signed long start_y;
	signed long start_z;
	signed long end_x;
	signed long end_y;
	signed long end_z;
	signed long viewer_x;
	signed long viewer_y;
	signed long viewer_z;
	signed long direction;
} coords;

int main(int argc, char *argv[])
{
	/**
	 * I need 3 parameters before we parse them out
	 * - x,y,z of the start of the world to extract
	 * - x,y,z of the end of the world to extract
	 * - x,y,z,degrees location of where to put the viewer with a facing direction in degrees
	 */
	if (argc != 4)
	{
		showhelp();
		return 1;
	}
	if (!parse_coords(argv))
	{
		printf("Error in command-line arguments\n");
		showhelp();
		return 1;
	}
	return 0;
}

/**
 * showhelp / Shows the help if we didn't get enough parameters
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
 * parse_coords / parses command-line arguments into numerical things
 */
bool parse_coords(char *s[])
{
	/* strtok() https://www.tutorialspoint.com/c_standard_library/c_function_strtok.htm */
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