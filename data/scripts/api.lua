-- Zero2D requires this script to function properly.
-- DO NOT ALTER THIS SCRIPT!

-- sandboxing
io = nil;
os = nil;
debug = nil;

-- 
z2d = {};

-- function definitions
z2d.test = zero2d_api_test;
--z2d.registerState = zero2d_api_register_state;
--z2d.registerEvent = zero2d_api_register_event;

-- enumerated sprite states
z2d.states = {};
z2d.states.STAND = 0;
z2d.states.RUN = 1;

-- enumerated sprite events
z2d.events = {};
z2d.events.ON_ANIMATION_END = 0;
z2d.events.ATTACK = 1;
z2d.events.SPECIAL = 2;
z2d.events.DEFEND = 3;
z2d.events.JUMP = 4;
z2d.events.END_ATTACK = 5;
z2d.events.END_SPECIAL = 6;
z2d.events.END_DEFEND = 7;
z2d.events.END_JUMP = 8;
z2d.events.END_TILT = 9;
z2d.events.TILT_FORWARD = 10;
z2d.events.TILT_BACK = 11;
z2d.events.TILT_UP = 12;
z2d.events.TILT_DOWN = 13;
z2d.events.SMASH_FORWARD = 14;
z2d.events.SMASH_BACK = 15;
z2d.events.SMASH_DOWN = 16;
z2d.events.SMASH_UP = 17;
z2d.events.GOT_HIT = 18;
z2d.events.GOT_GRABBED = 19;
z2d.events.GOT_RELEASED = 20;
z2d.events.DO_NOTHING = 21;
z2d.events.TILT_LEFT = 22;
z2d.events.TILT_RIGHT = 23;
z2d.events.SMASH_LEFT = 24;
z2d.events.SMASH_RIGHT = 25;
