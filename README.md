# ESP32 Lightstrip with Website

This project controls a programmable LED light strip (default: 144 LEDs, configurable via `NUM_PIXELS`) and exposes the controls through a built-in web interface on the ESP32.

## Project layout

- `src/main.cpp` contains the scene logic and control loop.
- `src/WebPage.h` builds the web UI.
- `src/NVMData.h` and `src/DynamicData.h` store settings and scene data.
- The light strip is driven with FastLED.

## Scene list

The scene number is the value sent through the web interface or query string, for example:

`/change?scene=0&red=0&green=0&blue=0&waittime=2&brightness=150`

The implemented scenes are:

0. `lightStripTrafficAdvisor` — moving orange-red traffic-advisor pattern across the strip
1. `twoPhaseLights` — alternating red/blue half-strip phases
2. `warpCoreMode` — four moving color points cycling through the strip with custom RGB values
3. `twoPhaseLightCustomColor` — custom-color alternating blocks with black gaps
4. `FullOnBlueBlackFlasher` — blue/black flashing pattern split across the strip
5. `FullOnCustomBlackFlasher` — custom color/black flashing pattern split across the strip
6. `allOn` — all LEDs on in the selected custom color
7. `RandomFlashingLights` — random LED flashes in the selected custom color
8. `strobo` — full white strobe on/off effect
9. `BlinkingLights` — full-strip blink using the selected custom color
10. `IndicatorRight` — right turn indicator in orange-red
11. `IndicatorLeft` — left turn indicator in orange-red
12. `HazardLight` — hazard flashing using orange-red on left and right sections
13. `IndicatorRightHeadlight` — right indicator with white headlight state
14. `IndicatorLeftHeadlight` — left indicator with white headlight state
15. `HazardHeadLight` — hazard flashing with orange-red and white
16. `HeadlightNormal` — constant white headlight
17. `FarlightsHeadLight` — constant brighter white headlight
18. `HazardLightFast` — faster hazard flashing pattern
19. `SmoothHueChange` — smooth hue sweep across the strip

## Off / unknown scene behavior

Any scene number outside the valid range above, or an unrecognized value, falls through to the default action, which turns the LEDs off.

This is used as a safe fallback and prevents LEDs from remaining stuck on at the wrong color.

## Notes

- `brightness` and `waittime` are adjustable in the app and affect the scene behavior.
- `red`, `green`, and `blue` values are used for custom-color scenes.
- The strip length is currently set to 144 LEDs by default, but can be changed in `src/main.cpp` by editing `NUM_PIXELS`.
