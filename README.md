# TVout DSO

TVout DSO is a simple oscilloscope running on basic arduino cards such as the uno, nano, or pro micro.

## Requierements

- Arduino card, such as an Arduino Uno
- Arduino IDE
- TVout Arduino library (directly installable from the IDE)
- 3 NO buttons + 3 pullup resistors (470 kohm for example)
- 175 ohm, 470 ohm, and 1kohm resistors for composite signal generation
- analog input part with optional bistable switch for 1x/10x (as described on the schema for example)

## Installation

You need the TVout library.
Don't forget to read it's readme: you need to copy the tvout fonts folder

## Usage

Cable the Arduino according to the electrical schema.

You only need the 20kHz for fast single acquisition.

Fast single acquisition will use interrupts from an external pin to prevent cuts from acquisition.
This will also disturb video signal for the time of acquisition.

## License

Project under MIT License.
https://opensource.org/licenses/MIT

## History
The first goal of this project is a challenge proposed by Electrobidouilleur:
https://www.youtube.com/watch?v=GWkLWGPdXKE

It finds it's origin in Wullschj's work on this page, and have been fully rewritten to respect hypothetical copyrights :
https://forum.arduino.cc/index.php?topic=64327
