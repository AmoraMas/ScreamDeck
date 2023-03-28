# ScreamDeck

> Custom 36-key keyboard to run key-presses and/or macros.

## Table of Contents

- [General Information](#general-information)
  - [Technologies Used](#technologies-used)
  - [Features](#features)
  - [Screenshots](#screenshots)
  - [Setup](#setup)
  - [Usage](#usage)
  - [Current Status](#current-status)
- [Room For Improvement](#room-for-improvement)
- [Acknowledgements](#acknowledgements)
- [Contact](#contact)
- [License](#license)

## General Information

> Built with the intention to save money and learn by building my own custom Stream Deck like interface for when I was interested in doing game streaming. I ended up not saving any money as the buttons, key-caps, plexiglass, and boards cost me near $50 anyways. But I learned a lot about how to work with Arduino, building a physical product from the ground up, programming in C, and learning that not every board can take the same programming. I am still looking for ways to utilize this project, but the last time I opened it up to fix a software problem I thought was a hardware issue, the micro-USB port broke off of the board. My attempts to solder it back on were unsuccessful.

## Technologies Used

> C, Arduino, custom wiring, wood-work

## Features

- Runs on an Arduino Pro Micro board
- Has multiple profiles, so not limited to just 36 keys/macros
- Entire project was handmade from the box, to cutting out every square button hole in the plexiglass
- Functions as a keyboard, so is not dependent on OS, but can benefit from additional software that can be installed on any system, or connected to any software that accepts keyboard shortcuts.

## Screenshot(s)

> ![ScreenShot](/images/screenshot.png)

## Setup

- Wire up your Arduino Pro Micro to your buttons.
- Push .ino file to the board.
- Run test profile to verify that all buttons are working as expected.
- Adjust app as needed.
- Swap to another profile and enjoy.

[Project Demonstration]()

## Usage

Once everything is wired up and programmed, plug into your computer and use the macros. Can set up with an application like AutoHotKey or AutoKey to run scripts or to have addition functionality past simple button pressing.

## Current Status

> Complete. Modify button mapping/profiles as needed for your needs.

## Room For Improvement

- Ideas:
  - Use thinner wires than I did.
  - Possible to remove a set(s) of buttons and add a screen for visual output.
  - Labeling would be helpful
  - 3D printed keys or enclosure
- Todo:
  - N/A

## Acknowledgements

- ### Inspired By:
  - My desire for a homemade Stream Deck and to learn
- ### Based On:
  - https://www.sparkfun.com/tutorials/337
- ### Contributors:
  - None

## Contact

> [amoramas1984@protonmail.com](mailto:amoramas1984@protonmail.com)

## License

> MIT
