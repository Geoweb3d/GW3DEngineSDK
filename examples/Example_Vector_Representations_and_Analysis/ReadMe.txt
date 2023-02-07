
Example:  Vector Representations and Analysis

This example shows how to create graphic representations of vector data layers, and how to analyze the 3D scene.
Several vector layers are loaded and different 3D representations are created.
The location of the mouse pointer on the terrain is queried every frame.
A line segment intersection analysis is performed every frame to detect what features/representations/layers
are between the camera and the mouse position on the terrain, and on a mouse click the feature is highlighted 
and the results are printed to the console.
The camera moves toward the location of the mouse pointer on a double-click.
Use arrow keys to navigate, D / Ctrl+D to rotate, and Q / Ctrl+Q to pitch, W to move upward, X to move downward.
Press V to insert (or remove) a viewshed, and E to report the visible surfaces in the viewshed
Press S to switch into an anaglyph stereo mode.
Press P to save a high-resolution screenshots and thumbnails to disk.


Requires data from the Geoweb3d DC sample project that can be downloaded here:
http://www.geoweb3d.com/download/sample-projects/
Change the 'root_path' variable to the installed location of the DC project.