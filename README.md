# Esp32_Lightstrip_with_website
 This allows you to control a 144 (value adjustable) lights Lightstrip with different scenes as well as a website for easy access to the ESP32!


## This is using the some liberaries:
These can be found in the src folder and everything is a liberary except the "main.cpp" wich is the main file. 
Feel free to alter the webpages etc to your liking just make sure not to break anything!

# The way its set up:
- Name of the scene (notes) (number of scene on the website)

###  These are the scene we have:
0. trafficAdvisorWithFLashers ((blue and red flasher) (going to the right)) 
1. lightStripTrafficAdvisor (without the flahsers in teh middle this time, going to the right) 
2. twoPhaseLights (red and blue, without the traffic advisor) 
3. warpCoreMode (custom color and wait time) 
4. lighting (makes some LEDs a custom color) 
5. twoPhaseLightsCustomColor (makes the two phase lights light up in the color and jump between two spaces (black on unused)) 
6. randomFlashingLights (alternates between random spaces (0-143) and lights them in a preset color. Cusom waittime is also implimented!)
7. FullOnBlueBlackFlasher (Alternates between red and Blue over 77 pixels each)
8. FullOnCustomBlackFlasher (same as the one before but it takes a custom color)
9. strobo (turns on and off (white) in the waittime provided)

### everything over *the last number of the cases* will turn it "off" (and that is also used as a transition to avoid LEDs getting stuck on)
