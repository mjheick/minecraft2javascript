/**
 * mc2js / minecraft2javascript
 */
#include <stdio.h>
#include <string.h>

void showhelp(void);
void parse_coords(char *s[]);

struct Coords_struct {
	signed long start_x;
	signed long start_y;
	signed long start_z;
	signed long end_x;
	signed long end_y;
	signed long end_z;
	signed long viewer_x;
	signed long viewer_y;
	signed long viewer_z;
	unsigned int direction;
} coords;

int main(int argc, char *argv[])
{
	/**
	 * I need 3 parameters before we parse them out
	 * - x,y,z of the start of the world to extract
	 * - x,y,z of the end of the world to extract
	 * - x,y,z,[d[nesw]|degrees] location of where to put the viewer, either with a cardinal direction or with a degree
	 */
	if (argc != 4)
	{
		showhelp();
		return 1;
	}
	parse_coords(argv);
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
           Direction can be NESW or a number between 0-360 for degrees\n\n\
Example:\n\
  mc2js 1,1,1 9,9,9 5,5,5,n\n\
  mc2js 1,1,1 9,9,9 5,5,5,0\n\
";
	printf("%s", help);
	return;
}

/**
 * parse_coords / parses command-line arguments into numerical things
 */
void parse_coords(char *s[])
{
	/* strtok() https://www.tutorialspoint.com/c_standard_library/c_function_strtok.htm */
	printf("%s\n", s[1]);
	printf("%s\n", s[2]);
	printf("%s\n", s[3]);
	return;
}