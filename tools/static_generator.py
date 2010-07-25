#!/usr/bin/python
'''static_generator.py is a script for generating static.02d files for the Zero2D engine.
'''
import sys

try:
    import Image
except(ImportError):
    sys.stderr.write("Couldnt find the Python Imaging Library. It isn't required, but it allows the script to guess several parameters.")


#Default settings:
NAME = "Lolongo"
NUMSHEETS = 1
SHEETSIZE = (4096, 4096)
SPRITESIZE = (512, 512)
FRAMES = 35
TICKSPERFRAME = 1
DEFAULTBASE = (256, 256)

#~ #And another example for you:
#~ NAME = "Pimple"
#~ NUMSHEETS = 1
#~ FRAMES = 8
#~ SHEETSIZE = (512, 512)
#~ SPRITESIZE = (64, 128)
#~ DEFAULTBASE = (32, 112)
#~ TICKSPERFRAME = 20

try:
    import argparse
except(ImportError):
    sys.stderr.write("Couldn't find the argparse module, please install it or manually set the settings in this script.")
    argparse = False

if argparse:
    parser = argparse.ArgumentParser(description=__doc__)
    
    #Required
    parser.add_argument('--name', dest='name', action='store', help='Set the name of the character.', required=True, type=str)
    parser.add_argument('--sheet-file', dest='sheetfilename', action='store', help='Give the script a filename to use to guess some options.', required=True, type=str)
    parser.add_argument('--sheets', dest='sheets', action='store', help='Set the number of sprite sheets.', required=True, type=int)
    parser.add_argument('--frames', dest='frames', action='store', help='Set the number of frames in the sprite sheets.', required=True, type=int)

    #Optional
    parser.add_argument('--sheet-width', dest='sheetwidth', action='store', help='Set width of the sprite sheets.', type=int)
    parser.add_argument('--sheet-height', dest='sheetheight', action='store', help='Set height of the sprite sheets.', type=int)
    parser.add_argument('--sprite-width', dest='spritewidth', action='store', help='Set width of the sprite frames.', type=int)
    parser.add_argument('--sprite-height', dest='spriteheight', action='store', help='Set height of the sprite frames.', type=int)
    parser.add_argument('--ticks-per-frame', dest='ticks', action='store', help='Set number of ticks each frame is displayed for. For a 30fps animation, this should be around 1.', type=int, default=1)
    parser.add_argument('--default-base-x', dest='defaultbasex', action='store', help='Set default horizontal base coordinate for the sprites.', type=int)
    parser.add_argument('--default-base-y', dest='defaultbasey', action='store', help='Set default vertical base coordinate for the sprites.', type=int)

    args = parser.parse_args()

    NAME = args.name
    NUMSHEETS = args.sheets
    FRAMES = args.frames
    TICKSPERFRAME = args.ticks
    
    if args.sheetfilename != None:
	#Set SHEETSIZE and SPRITESIZE from args.sheetfilename
	pass

    if args.sheetwidth != None and args.sheetheight != None:
	SHEETSIZE = (args.sheetwidth, args.sheetheight)
    
    if args.spritewidth != None and args.spriteheight != None:
	SPRITESIZE = (args.spritewidth, args.spriteheight)
	if args.defaultbasex != None and args.defaultbasey != None:
	    DEFAULTBASE = (args.defaultbasex, args.defaultbasey)
	else:
	    DEFAULTBASE = (SPRITESIZE[0]/2, SPRITESIZE[1]/2)
	

print NAME
print NUMSHEETS, FRAMES+1
print "b %d %d" % DEFAULTBASE
print "s %d %d" % SPRITESIZE

print #Whitespace is good for you.

frame = 0
for y in xrange(0, SHEETSIZE[0], SPRITESIZE[1]):
	for x in xrange(0, SHEETSIZE[0], SPRITESIZE[1]):
		if frame > FRAMES:
			break
		print "f %d 0 %d %d" % (frame, x, y)
		frame += 1

print #It's like a vitamin, a little every day.

print "x",

#We have just one hardcoded state, I might add support for specifying states later.
print "1"
print "s 0 %d" % (frame),
print "d %d" % TICKSPERFRAME,
for x in xrange(frame):
	print "f %d" % x,

print "x"
