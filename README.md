# minecraft2javascript

Lets grab a portion of a minecraft world and allow it to be navigated in a browser

This is specific to Java edition

# Compiling

requires zlib. built and tested with zlib 1.2.11 https://zlib.net

Run make to complile. 'nuff said

# Usage

mc2js start_xyz end_xyz viewer_xyzd

start_xyz - A comma-separated list of starting X, Y, and Z coordinates to extract from the world

end_xyz - A comma-separated list of ending X, Y, and Z coordinates to extract from the world

viewer_xyzd - X, Y, and Z coordinates of where to place the viewer in the generated world. You can also specify a direction, either in NESW or in degrees.

start_xyz and end_xyz are going to be juggled to create a navigatable box to extract the world with. The larger the box, the larger the renderable world will be.

If viewer_xyzd is outside the world, it'll be a real bad experience.

# References

* [https://stackoverflow.com/questions/1484896/php-gzinflate-in-c]
* [https://www.zlib.net/zlib_how.html]
* [https://minecraft.gamepedia.com/Anvil_file_format]
* [https://minecraft.gamepedia.com/Region_file_format]
* [https://minecraft.gamepedia.com/Chunk_format]
* [https://minecraft.gamepedia.com/NBT_format]
* [https://web.archive.org/web/20110723210920/http://www.minecraft.net/docs/NBT.txt]
