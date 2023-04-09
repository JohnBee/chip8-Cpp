# Chip-8 C++ Emulator
A Chip-8 Interpreter/Emulator Written in C++.

# Demo
[Breakout](https://github.com/JohnEarnest/chip8Archive/tree/master/src/br8kout)
![Breakout](/demo/CHIP-8_EMULATOR_breakout.gif)

[Snake](https://github.com/JohnEarnest/chip8Archive/tree/master/src/snake)
![Snake](/demo/CHIP-8_EMULATOR_snake.gif)

[Flight Runner](https://github.com/JohnEarnest/chip8Archive/tree/master/src/flightrunner)
![FlightRunner](/demo/CHIP-8_EMULATOR_flightrunner.gif)


# Building
1. [Install SDL2](https://github.com/libsdl-org/SDL/releases/tag/release-2.26.5) for your Target
2. Setup CMake Cache using CLI or CMake-GUI to point to your SDL2 install library and headers location.
3. Create a Build Folder e.g.
   ```
   > mkdir build && cd ./build
   ```
4. Run CMake to generate your desired Solution e.g.
   ```
   > cmake .. -G"MingGW Makefiles"
   ```
5. Run Make/ Build of choice.
   ```
   > make
   ``` 

# Running
1. Navigate to ``` cd ./build ``` 
2. Place ```SDL2.dll``` Next to ```chip8-emulator.exe```
3. Execute the Emulator ``` ./chip8-emulator.exe <path-to-game.ch8> ```

# Testing
Testing of accuracy was accomplished using an assortment of Test Roms:
https://github.com/corax89/chip8-test-rom
https://github.com/Timendus/chip8-test-suite

# Requirements
```
CMAKE
SDL2
```
