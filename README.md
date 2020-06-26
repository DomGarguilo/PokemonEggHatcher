# PokemonEggHatcher
An arduino script that hatches eggs within the video game Pokemon Shield for the Nintendo Switch

The concept behind the project is to use an Arduino to constantly observe the display of a
Nintendo Switch running the game, ​ Pokémon Shield . ​ The Arduino, and its components, would
also control the game movements of the Nintendo Switch by simulating voltage to a
deconstructed GameCube controller. The deconstructed GameCube controller is connected
through soldered wires attached to the Arduino. This simulates human input on the controller and
allows us to pass scripted movements from the Arduino, through the controller, and into the
game. To monitor in-game events, we use an LDR (light-dependent resistor) that will be put
against the screen to measure certain light values on the screen. This light sensor determines the
brightness of the area on the screen which allows for synchronization of in-game events.

Implementation Details
![Image of Yaktocat]
(https://github.com/DomGarguilo/PokemonEggHatcher/blob/master/images/1.png?raw=true)
Pictured: Arduino and Components of GameCube controller and Printed Circuit Board
To implement our project, we used a deconstructed GameCube controller. The
GameCube controller was taken apart and the Printed Circuit Board (PCB) was extracted. The
main goal is to be able to control the game from the Arduino. To do this, we needed to figure out
how the different components of the GameCube work, and how to control them from the
Arduino. The main components of the controller that are needed for this project are the buttons
and the joystick. The buttons are used to select, advance or otherwise send boolean inputs to the
game. The joystick is used to control the cursor in menus, movements of the character and other
directional inputs. Additionally, we need a way to monitor the in-game events which is where the
LDR comes in.
First, to implement the button presses, we needed to determine how such functionality
normally works on the GameCube controller. Most physical buttons on electronic devices
operate the same way. There is a data line leading to a bridge on the PCB which is normally
“open”. This means that by default, the data line for the button is held at the input voltage which
in this case is 3.3v. When the button is pressed, a conductive pad on the underside of the button
is lowered, closing a gap between the data line at 3.3v and a grounded line completing the circuit
and bringing the data line to ground. This is how the controller knows when a button is pressed.
To simulate the button press from the Arduino, we used a relay module as pictured below.Pictured: Relay modules
Each of the blue boxes depicted in the middle is a separate relay. The relays work by
switching an input line (Common) between two other lines (‘normally open’ which is ​ not
connected to common and ‘normally closed’ which ​ is ​ connected to common) . In our case, we
soldered wires to the spot on the PCB where the button lines are (pictured below).
Pictured: Soldered wires on the Printed Circuit Board
We then fed each of those wires to the common slot on a relay. One relay for each button
line. We left the “normally closed” slot free and put a grounded wire in the “normally open” slot.
When the relay is triggered, the wire coming from the button would be connected to the
“normally open” (grounded) wire completing the circuit and triggering a button press reading
from the controller.
The next part of the implementation was the joystick. A joystick is essentially two
potentiometers. “A potentiometer is a three-terminal resistor with a sliding or rotating contact
that forms an adjustable voltage divider” [2]. This means that depending on how far the joystick
is pressed in each direction, the resistance will increase or decrease one potentiometer for thex-axis (left and right) and one for the y-axis (up and down). After measuring the voltage output
of the joysticks, we found that when pressed all the way to the left and all the way down, the
output lines read 0v each. When pressed all the way to the right and all the way up, the output
lines read 3.3v each. Knowing this, we needed to replicate these readings and feed it back into
the joystick output lines. To do this, we acquired two DACs (pictured below) which allowed us
to output the analog signal that we needed at varying voltages.
Pictured: y-axis and x-axis Digital to Analog Converters
One DAC for the y-axis and one DAC for the x-axis. For example, if we want the game
to register a downward movement, we set the respective DAC to 0v. When no movement is
wanted, we hold both DACs at a constant middle voltage.
The third and final main hardware component is the LDR. The LDR is taped to the screen
displaying the game and measures the brightness and returns a corresponding integer to the
Arduino.
Pictured: Light-Dependent Resistor
This integer value is used in the code to determine when certain in-game events are
taking place. For this application, we used the LDR to determine when dialog boxes appear. This
is helpful because it indicates when an egg is hatching and when it is done hatching. Thisinformation is needed to know when to stop the player from moving and begin the portion of the
script used for egg hatching. To determine if the hatched Pokémon is shiny, we measure the time
in between the point at which the first dialog box appears, signifying the egg hatching animation
has started, and the second dialog box signifying the end of the animation. This time is measured
to determine if the Pokémon is shiny or regular. A shiny Pokémon will result in slightly longer
animation.
