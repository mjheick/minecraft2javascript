# minecraft2javascript
Lets grab a portion of a minecraft world and allow it to be navigated in a browser

This is specific to Java edition

# Compiling

There is an included makefile, so just run make to compile

# Usage

mc2js start_xyz end_xyz viewerxyzd

start_xyz - A comma-separated list of starting X, Y, and Z coordinates to extract from the world

end_xyz - A comma-separated list of ending X, Y, and Z coordinates to extract from the world

viewer_xyzd - X, Y, and Z coordinates of where to place the viewer in the generated world. You can also specify a direction, either in NESW or in degrees.

start_xyz and end_xyz are going to be juggled to create a navigatable box to extract the world with. The larger the box, the larger the renderable world will be.

If viewer_xyzd is outside the world, it'll be a real bad experience.


