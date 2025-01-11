# Esp32_Lightstrip_with_website
 This allows you to control a 144 (value adjustable) lights Lightstrip with different scenes as well as a website for easy access to the ESP32!


## This is using the some liberaries:
These can be found in the src folder and everything is a liberary except the "main.cpp" wich is the main file. 
Feel free to alter the webpages etc to your liking just make sure not to break anything!

# The way its set up:
- Name of the scene (notes) (number of scene on the website)

## These are the scene we have:
- Two phase lights with a traffic advisor (blue and red flasher) (going to the right) (0)
- traffic advisor, without the bleu and red lights (going to the right) (1)
- two phase lights (red and blue, without the traffic advisor) (2)
- warp core mode (custom color and wait time) (3)
- lighting (makes some LEDs a custom color) (4)
- Full Lighting (makes 100 LEDs (in a row) a custom color) (5)
- FullLightFlash (flashes the light (70 in a row) every 110 executions) (6)
- FillSolid80 (makes 80 LEDs in a row light up, custom color) (7)
- twoPhaseLightsCustomColor (makes the two phase lights light up in the color and jump between two spaces (black on unused)) (8)
- everything over 8 if "off" (and that is also used as a transition to avoid LEDs getting stuck on)
