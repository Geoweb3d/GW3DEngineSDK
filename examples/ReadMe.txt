Geoweb3d Engine Examples:

* 3DTiles *
This example demonstrates the loading of 3DTile dataset.
These datasets are typically of a significant size disk-wise
and as such the example does not provides a dataset, this
will have to be provided by the user. The user provided
dataset should be placed in the the SDK example 'media'
directory or any a user created sub-directory of the 'media'
directory. The path to this dataset needs to be provided in
\example source code( the call to LoadDataset on line 164 of
Example_3D_Tiles.cpp) and the example rebuilt. Currently, the
\example only displays the basic world-wide imagery.

* Adding User Vector Data *
This example demonstrates the adding of feature(s) to a layer
of a user created data source. The example create 6 vector
layers in a user data source and add 3 features to one of the
created vector layer. The example produces no visible output
and instead display the result of each operation in the
console window then exit.

* Animation Playback *
This example demonstrates the playback of animation data
loaded from Geoweb3D Tour files. The example loads 3 tour
files which are used to create a set of animation path
objects. Three groups of entities( depicted by billoards with
numbers corresponding to each groups) are created and each is
assigned an animation path. Every frame, the position of each
entity is updated from data retrieved from the animation
path. The result is each group of entities will move along
the predefined path that was assigned to the tour.

* BillboardProperties *
This example demonstrates the different properties that can
be assigned to a billboard representation. The example
creates a few textured billboards with labels and displays an
onscreen menu to allow for modifying the different billboard
properties presented. Selecting a feature( billboard) via the
mouse will highlight the feature by displaying a selection
rectangle around the feature. The selected feature id as well
as the intersection distance ( distance from the camera to
the intersection point) is displayed in the console window.

* Billboards *
This example demonstrates how to create simple billboard
representation and billboard line segment intersection test.
The example creates a number of textured billoards, each
displaying one of the number[1-6]. Selecting a billboard by
clicking with the mouse( left-button) will display the
following information in the console window:
\-Feature Id
\-Texture Number[1-6]
\-Line segment intersection test distance( if the billboard
was hit/selected).

* Callback Datasource *
This example demonstrates the usage of the vector pipeline
callback. The example registers a vector pipeline callback
handler whose methods are invoked to provide vector features on
request. It also loads an ESRI shapefile which is used as the
feature data source. The shapefile is displayed when the
\example runs, and selecting a feature will highlight the
feature Red and displays the feature id and name in the
console window.

* CDB_CommonDatabase *
This example demonstrates the loading of a CDB dataset. These
datasets are typically of a significant size disk-wise and as
such the example does not provides a dataset, this will have
to be provided by the user. The user provided dataset should
be placed in the the SDK example 'media' directory or any a
user created sub-directory of the 'media' directory. The path
to this dataset needs to be provided in example source code(
the call to
driver.lock()-\>get_VectorDataSourceCollection()-\>open on
line 529 of MiniEngine.cpp) and the example rebuilt.
Currently, the example only displays Bing-Hybrid imagery.

* ClientOpenGL2DRendering *
This example demonstrates using custom(client/user) OpenGL
rendering alongside Geoweb3d Engine SDK rendering. The
\example creates 3 features which are represented as
spheres.Each frame the spheres position are update and they
are redrawn via the Geoweb3D Engine SDK. The example also
displays 2 textured quads that are drawn via OpenGL. One of
the quad modifies the texture coordinate used to give a
horizontal scrolling effect.

* Colored_Lines *
This example demonstrates the colored line representation.
The example loads a raster and then creates a line the extent
of the diagonal of the raster. It will then cycle through a
few properties of the line representation such as the color,
width. The current properties are displayed in the console
window.

* Contours *
This example demonstrates the generation of vector data from
a raster data source. The example uses elevation data to
generate a vector layer, in this case the elevation contours.
The generated vector layer is then rasterized (red contours)
and used as an imagery source, which is represented as an
imagery overlay. The example displays the final overlayed on
top of the 'base' imagery layer.

* Display_Devices *
This example enumerates the display devices in the system and 
test the following capabilities:
\-Is the device capable of running Geoweb3d SDK.
\-Is the device capable of stereoscopic rendering
The example outputs the display information in the console
window of the application.

* Draped_Lines *
This example demonstrates the Draped Line representation. The
drape line will follow the contour ( drape over ) the
underlying terrain. This example is similar to the
Colored_Line example with the exception that the line is
draped over the terrain. The current camera view for this
\example does not allow for adjustment so the view is fixed.

* DrapedLines *
This example further demonstrates the Draped Line
representation by creating 3 textured drape lines that are
displayed in the view. Additionally, the properties of the
displayed draped lines can be modified. See the 'Help'
options in the console window by pressing the 'H' key.

* DrapedRings *
This example demonstrates the Draped Ring representation by
creating a few textured and untextured drape rings that are
displayed in the view. Additionally, the properties of the
displayed draped rings can be modified. See the 'Help'
options in the console window by pressing the 'H' key.

* Elevation *
This example demonstrates loading a raster data source and
creating an elevation representation from the loaded data
source. The elevation data loaded is used as the elevation
data for the terrain vertices.

* Extruded_Path *
This example demonstrates the Extruded Path representation.
The example loads and represent an imagery datasource and
creates an extruded path object that spans the diagonal of
the raster datasource. It then cycles through and change
various properties of the displayed extruded path.
Additionally, the properties of the displayed extruded path
can be modified.. See the 'Help' options in the console
window by pressing the 'H' key.

* Extruded_Textured_Polygons *
This example demonstrates the Extruded Polygon extended
representation. The example load an ESRI shapefile containing
building footprints( 2D ), which are then represented as
extruded polygons giving them a 3-D appearance. Each feature
is then textured and colored based on an extrusion height
range. The color is use to modulate the texture color, giving
the buildings their final appearance.

* Extruded Path *
This example demonstrates the Extruded Path representation in
drape mode. The example creates 4 extruded path ( 3-textured
and 1-untextured) that are draped over the underlying
terrain.

* Flight_Path *
This example demonstrates a number of feature
representations, namely:
\-Model representation for the aircraft.
\-Color Line representation for the flight path.
\-Billboard representation for the aircraft location.
The example displayes a moving entity( aircraft) flying along
a predefined path.

* Geocoding *
This example demonstrates the geocoding( geographich search)
feature of the Goeweb3d SDK. The example uses the console to
allow the user to specify the search location. When the user
enters a search location, the search results are displayed.
Selecting a search result( numeric key ) will center the user
on that location. The default search location used by the
\example is Washington DC.

* Icons *
This example demonstrates using client OpenGL drawing to
display 'icon' like features. The texture used for the icons
are a 'procedural' checker-board texture.

* Identify *
This example demonstrates the Geoweb3d SDK line segment
intersection testing capability. The example loads a
shapefile containg a number of features( 'generic building' )
and allow the user to select a feature by left-clicking it.
Whenever a feature is selected, a line segment query test is
executed. If the query result in a feature hit, then details
about the feature is displayed in the console window.

* ImageOverlay *
This example demonstrates the imagery overlay feature of the
Geoweb3d SDK. The examples loads a raster( red contours ) of
a previously rasterized vector source and overlays it on top
on the base imagery as well as another loaded imagery source.

* Imagery *
This example demonstrates loading a raster datasource and
representing one of its layers as "Imagery". The example
loads a raster ( DC_1ft_demo.ecw ) and then represent the
single layer in the data source as "Imagery". The result view
shows the datasource being represented as imagery.

* ImageryServiceConfiguration *
This example demonstrates how to configure and load Web
Services layer(s). The example outputs a list of Web Service
layers in the console and the user is required to selected a
given layer via the console. This is accomplished by entering
the number in the square brackets of the text 'Layer[x]'. The
layer is then represented as "Imagery" and is displayed as
the base map.

* LargeExample *
This example demonstrates a number of features of the
Geoweb3d SDK, namely:

\-Vector data loading and representation. The example loads
an ESRI shape file which is then represented as "Colored
Lines".

\-Model representation. A model is loaded and position
positioned via a uniform distribution at various location in
the scene.

\-Line segment intersection testing. Clicking on any of the
feature or the model will display feature specific
information in the console window.

* Line_Representations *
This example demonstrates creating a representing a feature
using the "Colored Line" representation. The example creates
a vertical line in the center of view.

* Message_Proc_Hook *
This example demonstrates how to register for and handle
windows event via a callback. The example will display a
message in the console window whenever the spacebar is
pressed.

* MovingSpecificFeatures *
This example demonstrates dynamically updating user created
features. The example creates 3 features which are
represented as textured "Billboards" using the default place
mark icon image. Each frame a feature is selected and its
position updated.

* Moving_Representations *
This example demonstrates dynamically updating user created
features. The example creates 10000 features represented as
"Models", whose position and rotation are dynamically updated
each frame.

* MovingModel *
This example demonstrates dynamically updated a feature
represented as a "Model". The example create a feature and
represent it as a model. When the user select the model and
pan the view, the model position is updated to match the
camera position.

* MovingProjectedTexture *
This example demonstrates the use of the "ProjectedImage"
image representation. The example loads an ESRI shapefile
which is represented as "Extruded Polygons", as well as a
feature that is represented as a "ProjectedImage". The
project image when drawn in the scene will appears to be
'projected' onto the scene geometry.

* MovingTexturedDrapeRing *
This example demonstrates the usage of the "DrapedRing"
representation. The example creates a single feature which is
then represented as a textured "DrapedRing". The position of
the ring can be updated in 2 modes selected by using the
number keys '1' or '2'. Mode 1 repositions the draped ring
based on the location of the viewer( camera ). Mode 2 will
position the draped ring where the view direction intersects
the ground.

* Multiple_Cameras *
This example demonstrates using the Geoweb3d SDK to create
multiple views within a single window.

* Navigation *
This example demonstrates using the Geoweb3d SDK to create
multiple views within a single window, with the ability to
navigate via the mouse within each view. Each individual view
can be selected and interacted with via the mouse.

* PointCloud *
This example demonstrates the loading of a Point Cloud
dataset. These datasets are typically of a significant size
disk-wise and as such the example does not provides a
dataset, this will have to be provided by the user. The user
provided dataset should be placedcin the the SDK example
'media' directory or any a user created sub-directory of the
'media' directory. The path to this dataset needs to be
provided in example source code( the call to
get_VectorDataSourceCollection()-\>open on line 421 of
MiniEngine.cpp) and the example rebuilt. Currently, the
\example only displays the basic world-wide imagery.

* Poster_Capture *
This example demonstrates the screen capture functionality of
the Geoweb3d SDK. While running, the example periodically
captures the screen and save it to the following location
"C:/temp/" on disk.

* SceneFile *
This example demonstrates the loading of a Geoweb3d Scene
File as well as the airport lighting system feature of the
Geoweb3d SDK. The examples uses the following numeric key/key
combinations

to modified the light/atmospheric properties:
[A] - Toggles Fog Activity.
[7] - Decrease Haze Visibility.
[Alt-7] - Increase Haze Transition.
[9] - Increase Haze Visibility.
[Alt-9] - Decrease Haze Transition.
[4] - Decrease Fog Visibility.
[Alt-4] - Increase Fog Transition.
[6] - Increase Fog Visibility.
[Alt-6] - Decrease Fog Transition.
[8] - Increase Fog Height.
[5] - Decrease Fog Height.
[PgUp] - Increment Time Of Day by 1hr.
[PgDn] - Decrement Time Of Day by 1hr.

SceneFile_Saving \<FAILED\>

* Slope_Analysis *
This example demonstrates the Slope Analysis feature of the
Geoweb3d SDK. The example creates a slope analysis analytic
object and displays the result in the view. The result
displayed is colored to show the different slope ranges.

Smoke_Representations \<FAILED\>

* Spotlight_Representations *
This example demonstrates one of the lighting feature of the
Geoweb3d SDK. The example loads an ESRI shape file and the
represent each feature loaded using the "SpotLight"
representation.

* Stereo *
This example demonstrates the anaglyphic stereo rendering
feature of the Geoweb3d SDK.

* Stereo_GenericHMD *
This example demonstrates the stereoscopic rendering
capabilities of the Geowweb3d SDK using a Head Mounted
Device.
To get started:
Install Steam and SteamVR
Download and install Steam :
https://store.steampowered.com/about/
\Note: You will need to create an account with Steam
Through Steam install SteamVR : http://steam://run/250820
Update your graphics drivers to the latest version:

NVIDIA Drivers :
http://www.nvidia.com/Download/index.aspx?lang=en-us

AMD Drivers : http://support.amd.com/en-us/download
Make sure your computer is VR Ready :
http://store.steampowered.com/app/323910/

Install Hardware Specific Software. (Instructions are usually
packaged with the device)
HTC Vive : https://www.vive.com/us/setup/
Oculus Rift : https://www.oculus.com/setup/

Launch SteamVR
You can do this by clicking the VR button that appears in the
top right corner of the

Steam client.
You can also launch SteamVR from the Steam library under
Tools.

The SteamVR Status window will show up. Follow the prompts to
finish installing your Device.

* Stereo_Oculus *
This example demonstrates the stereoscopic rendering
capabilities of the Geowweb3d SDK using an Oculus Rift HMD.
\NOTE, an Oculus Rift HMD is required for this example.

* Stereo_UserHMD *
This example demonstrates the stereoscopic rendering
capabilities of the Geowweb3d SDK using an OpenVR HMD. NOTE,
an OpenVR capable HMD is required for this example.

* System_Utilities *
This example demonstrates several utility feature provided by
the Geoweb3d SDK. The information queried for each utility is
displayed in the console window.

* TAKRepresentation *
This example demonstrates the Geoweb3d SDK TAK integration
feature. The example allow the user to connect to a specified
TAK server. Once a server connection is established and
Cursor On Target messages are received for server contacts,
these will be displayed in the view with their appropriated
team color as well as their callsign. The commandline options
for the example are as follows are also printed to the
console window:

TAKRepresentation.exe \<server ip\> \<server port\>
\<callsign\> \<user_name\> \<password\>

* TerrainAvoidance *
This example demonstrates the Terrain Avoidance analytic
feature of the Geoweb3d SDK. The example creates a terrain
avoidance analytic object and displays the result in the
view. The result displayed is colored to show the different
'risk/danger' area based on the location of the analytic
object.

* Textured_Representations *
This example demonstrates the creation of a number of
representations supported by the Geoweb3d SDK. Each of the
created representation is assign a texture to also
demonstrate its support for texturing of the geometry.

* TexturedDrapedLines *
This example demonstrates the ability of the "Draped Line"
representation to support texturing of the geometry. The
\example creates a draped line and apply an "arrow" texture
to the representation.

* Vector_Representations_and_Analysis *
This example showcase a number of capabilities of the
Geoweb3D SDK, namely:
Vector data loading and representation.
Frustum queries.
Atmospheric effects in the form of precipitation.

* ViewSheds *
This example demonstrates the Viewshed analytic feature of
the Geoweb3d SDK. The example creates a Viewshed analytic
object and displays the result in the view. The result
displayed is colored to show the different 'risk/danger' area
based on the location of the analytic object.

* Water_And_Clouds *
This examples demonstrates how to use the Water
representation of the Geoweb3d SDK.

* Weather *
This example demonstrates loading and displaying weather data
from a number of different online feeds. The view is updated
in realtime as the new weather data is obtained from the
feed. The example have a built-in 'Help' menu that is
displayed in the console window whenever the 'H' key is
pressed.

* Windowing *
This exampled demonstrates the windowing capabilities of the
Geoweb3d SDK. The example cycles through the creation of
multiple windows, w/o multiple cameras per view in each
window.