Leyla Mammadova
Intro to Game Programming: Assignment 1
Objectives:
•	Hack the example_invaderers and change the game dynamics
•	Setting up objects by setting a CSV or similar txt file
•	Write your own fragment shader

Ghost Blocks!
YouTube link:
https://www.youtube.com/watch?v=kxJkdnniKGo

Gameplay:
Help this little ghost pass through the obstacles! Don’t let yourself stuck! As soon as you won’t be seen by camera the game is OVER.

Controls:
Use WASD to move the ghost. 
Press “P” to pause the game. Press “P” again to proceed the game.
Press LEFT SHIFT to restart the game.

General changes:
What I did first it changed the whole concept of the game. Now there is no invaders, there is only one player(ghost). 
Before that score was counting your hits on invaders, now score is showing how much time(seconds) you stayed alive. To implement that score is counting frames. 
The player before that could only go from left to right, I’ve added WASD key with ASCII values, so now player can move in any direction. Also, player collides with borders and obstacles. 
To make a sense of that the borders are going down actually the camera with borders is going up. The obstacles are represented by reading a txt file. To set up the obstacles instead of adding each one separately, I’ve use a txt file (“borders.txt”).
 Added game rules in the beginning so player would understand the objectives of the game.
There should be a failure in the game so if you stuck and you are lost from the camera ghost collides with a bottom border which you don’t see. 

Fragment Shader:
I’ve added an invert_shader to the octet and another render method. This shader colour the sprites to invert colour. But since I didn’t want it to apply to every sprite added a Boolean function invert that allowed me to change only selected sprites. 
  with invert_shader                                                                 with invert_shader applied only on obstacles












Sound and images:
Changed gifs so it would fit to the concept of the game. I do now own any of this pictures.
Added creepy version of music by M83 “Midnight City”.

Problem solving:
Problem: ship didn’t collide with all obstacles
Solution: added for loop for collision
Problem: render function was for texture_shader
Solution: added another render function for invert_shader and Boolean function
Problem: camera didn’t move up
Solution: added camera function in to simulate function so it would be called more than once
Problem : reading the whole file (not only 1 line) and convert string to char
Solution :  http://stackoverflow.com/questions/13035674/how-to-read-line-by-line-or-a-whole-text-file-at-once  
http://www.cplusplus.com/forum/general/100714/


