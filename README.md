cBridge

A simple wooden bridge analysis program using SFML and Opencv to process video and highlight stress areas

Dependencies:

 -Opencv 3.10
 -SFML 2.3.2 
 -SFGUI 0.2.3.0

Program processes video data comparing a chosen frame to all others. Fix a camera in place and take a video of a bridge as it is being tested. Choose a specific frame as the "kek frame" to compare to all the others. The program highlights changes based on an adjustable threshold in the GUI. The red areas are areas where the bridge strains most under weight. This can be used to identify high areas of stress and strain. 

-Currently only able to work on live video data. Working on video input and video writing. 

-Future plans include the possibility of point tracking and a static model creation for bridge analysis. User could input the distance from the bridge the camera is and input the static members of the bridge. This would allow for simple analysis of the design. 2D of course.
