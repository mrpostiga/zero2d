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
z2d.events.ON_END = 0;
z2d.events.DEFEND = 1;
z2d.events.JUMP = 2;
z2d.events.TILT_FORWARD = 3;
z2d.events.TILT_BACK = 4;
z2d.events.TILT_UP = 5;
z2d.events.TILT_DOWN = 6;
z2d.events.SMASH_FORWARD = 7;
z2d.events.SMASH_BACK = 8;
z2d.events.SMASH_UP = 9;
z2d.events.SMASH_DOWN = 10;
z2d.events.ATTACK = 11;
