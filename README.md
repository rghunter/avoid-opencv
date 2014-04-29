#COLA Optical COLision Avoidance
### Ryan G. Hunter <ryan.g.hunter@gmail.com>

COLA is a monocular collision avoidance algorithim orignally developed at the Boston University Intelligent Mechatronics Lab. The algorithim calculates the time unitl collision by characterizing the rate of
change of a point with respect the the center point of the camera. Simply put, given a camera with a fixed focal length, we can model how we expect a fixed point to move across an optical plane as we approach
it. Based on the rate at wich the point approaches the nodal, we can calculate the time of impact (The time when the point reaches the pinhole of the camera).

##Using COLA

To build COLA, you will need OpenCV: http://opencv.org built and installed. Once you have that simply run make to compile the COLA runtime and associated unit tests (COLA_Test).

Checkout the pdf in this directory for detailed API documentation.
