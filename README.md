# Atlantis

Underwater is a first-person RPG game where the character is transported in an underwater ruined city for an experiment.
They are trapped in an air bubble along with a tiny robot that is controlled by the devs. Through this companion, the player 
receives instructions on how to escape this experiment.

The current version only allows for movement in the set scene.The player can swim in all directions and explore the map 
made of two parts: an old part where it once was a great city and wild part where they will find only aquatic life. In the 
left upper corner of the screen, the player can see the HUD where the companion will be able to talk to the character, give them 
tasks and guide them along their journey. From the settings menu, that can be accessed using the Setting button in the upper 
part of the HUD, the player can also adjust the speed of their character's movement, the sensitivity of the mouse rotation 
and adjust the visibility level of the scene or toggle on/off the animation.

Pictures/intro.png

# Dependencies used

OpenGl with GLFW 3.2.1 
ImGUI

# How to run

From VisualStudio 2022 run with debugging on x86.

# Commands

W			-	swim forward

S			-	swim back

A			-	swim left

D			-	swim right

Q			-	swim up

E			-	swim down

Z			-	roll left

C			-	roll right

SPACE			-	toggle cursor on/off 

# Future Implementations

~  Story System  ~

To implement the Story system, we plan on adding an NPC (non-player character) which the players can approach in order 
to receive various tasks from.These tasks will be stored into a quest log that can either be brought up by pressing an additional 
key (ex. “L”) or have it displayed at all times in the top right of the screen.

To check for the completion of these quests, we will use event flags that verify the progression of a player, 
updated in real time as they fulfill objectives.

The current plan for the game was to add 5 quests, with three of them being Fetch Quests 
(Quests meant to find various objects and bring them to the NPC that tick the event flag whenever the items are delivered to 
the NPC)and 2 of them being Exploration Quests (Taking the player to explore a certain area by adding the completion flag 
in a certain area).
