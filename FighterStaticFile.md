# Introduction #

Every entity in the game (whether it be a fighter or a projectile) contains the file **static.02d**. This file contains all the information regarding the entity's sprite sheets, frame sizes, frame locations, etc. It also goes on to define the entity's various individual states. Each state consists of a collection of frames (the series of frames that is animated in the game) and their durations (how many game pulses they hold each frame to make the animation look right).


# File Spec #

The first line is dedicated to the entity's name. This is only relevant for fighters, but even non-fighter entities need to have his line. All subsequent data can be placed into as many lines as desired for readability.

Next are two numbers: the number of sprite sheets followed by the number of frames this entity has overall (across all sprite sheets).

## Frame Data Entry Keys ##

Next is the frame data entry. A single letter key (indicating a function) is followed by its data parameters. The valid keys are **b**, **f**, **s**, and **x**. Note that all x/y coordinates are based on screen pixel space: 0,0 is the upper left corner.

**b** - _base_ - requires two parameters: x and y indicating the location of the frame's anchor

**s** - _size_ - requires two parameters: width and height of the frame

**f** - _frame_ - requires four parameters: frame index (zero-based), sheet number, and x/y location of the frame on the sheet

**x** - _finish_ - indicates that the frame data entry phase is over

This data entry system functions as a state machine. In other words, you do not have to specify the base and size for every individual frame. Each frame will simply take on the last base and size specified. This is particularly useful for repeating data and for accommodating uniform sprite sheets.

## State Data Entry Keys ##

Next is the state data entry. This section uses the same rules as frame data entry did, but the keys are different. Before any data entry begins, one number is required indicating the number of states that will be read in.

**s** - _state_ - requires two parameters: the state index (zero-based) and the number of frames in this state

**d** - _duration_ - requires one parameter: the number of game pulses to apply to the frames

**f** - _frame_ - requires one parameter: the frame index (corresponds to the index entered above) to push onto the stack for this state animation

**x** - _finish_ - indicates that the state data entry phase is over