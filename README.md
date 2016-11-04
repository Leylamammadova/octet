#Leyla Mammadova
#Intro to Game Programming: Assignment 1
#Objectives:
•	Hack the example_invaderers and change the game dynamics

•	Setting up objects by setting a CSV or similar txt file

•	Write your own fragment shader

#Ghost Blocks!

#YouTube link:

https://www.youtube.com/watch?v=kxJkdnniKGo

#Gameplay:
Help this little ghost pass through the obstacles! Don’t let yourself stuck! As soon as you won’t be seen by camera the game is OVER.

#Controls:
Use WASD to move the ghost. 

Press “P” to pause the game. Press “P” again to proceed the game.

Press LEFT SHIFT to restart the game.

#General changes:
What I did first it changed the whole concept of the game. Now there is no invaders, there is only one player(ghost). 
Before that score was counting your hits on invaders, now score is showing how much time(seconds) you stayed alive. To implement that score is counting frames. 

The player before that could only go from left to right, I’ve added WASD key with ASCII values, so now player can move in any direction. Also, player collides with borders and obstacles. 

To make a sense of that the borders are going down actually the camera with borders are going up. The obstacles are represented by reading a txt file. To set up the obstacles instead of adding each one separately, I’ve use a txt file (“borders.txt”).
 Added game rules in the beginning so player would understand the objectives of the game.
 
There should be a failure in the game so if you stuck and you are lost from the camera ghost collides with a bottom border which you don’t see. 


#Fragment Shader:
I’ve added an invert_shader to the octet and another render method. 
This shader colour the sprites to invert colour. But since I didn’t want it to apply to every sprite I created a boolean function invert that allowed me to change only selected sprites. 

example of applying this shader to all sprites and only for blocks

<p align="center">
  <img src="https://github.com/Leylamammadova/octet/blob/master/invertshader.jpg?raw=true" width="350"/>
  <img src="https://github.com/Leylamammadova/octet/blob/master/shader.jpg?raw=true" width="350"/>
</p>


#Sound and images:
Changed gifs so thay would fit to the concept of the game. I do now own any of this pictures.
Added creepy version of music by M83 “Midnight City”.

#Problem solving:

Problem: ship didn’t collide with all obstacles

Solution: added for loop for collision

Problem: render function was for texture_shader

Solution: added another render function for invert_shader and boolean function

Problem: camera didn’t move up

Solution: added camera function in to simulate function so it would be called more than once

Problem : reading the whole file (not only 1 line) and convert string to char

Solution :  http://stackoverflow.com/questions/13035674/how-to-read-line-by-line-or-a-whole-text-file-at-once  

http://www.cplusplus.com/forum/general/100714/


#Tools and Middleware: Assignment 1

#YouTube link:

https://www.youtube.com/watch?v=n0m3O7l4h14


#Objectives:

•	Building a dynamic structure like a rope bridge that uses 
               spring constraints.

#Bullet Physics

Bullet is a physics engine which simulates collision detection, soft and rigid body dynamics. One of the ways that I’ve used to learn its library is by using bullet3 solution (from github) and Example Browser. Example Browser represent all bullet physics possibilities.

#Adding Rigid Bodies

To detect constraints at first we should add a rigid body. For this I used mesh instance in visual_scene class which contains parameters of btRigidBody.

#btPoint2PointConstraint

The point to point constraint, limits the translation of two rigidbodies. A chain of rigidbodies can be connected using this constraint. I used this constraint to connect planks.
The point-to-point constraint uses the following constructors:
btPoint2PointConstraint(btRigidBody& rbAbtRigidBody& rbB, const btVector3& pivotInA, const btVector3& pivotInB);

#btGeneric6DofSpringConstraint
With this constraint, I’ve connected two middle planks. This generic constraint can emulate a variety of standard constraints, by configuring each of the 6 degrees of freedom. I’ve limited the rotation of this planks by setting angular lower and upper limit.

#btHingeConstraint

To demonstrate this type of constraint I’ve added a sphere which moves from one side of the bank to another. I’ve limited the movement by decreasing the pivot.

#btDiscreteDynamicsWorld
To call world from visual scene I’ve added  get_world() function  to visual_scene.

#Improvements

I didn’t use a csv (or another similar txt file) to add objects, because of the small size of the bridge and variety of constraints that I’m using. 



#What this assignments mean 


Now reading someone else sourse code and working in a new framework is not that confusing as it seemed in the begining. Since I've had a decent break in programming I have a positive attitude for this course and how it can affect on my posibilities and bring me closer to my dream job! 


#Thanks 
Thanks to Robert (links for reading a csv file) and Witek (explaining mesh instance) .

