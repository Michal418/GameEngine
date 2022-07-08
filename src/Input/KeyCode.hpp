#pragma once

enum class Key
{
	none = 0,
	left_mouse = 1,
	right_mouse = 2,
	cancel = 3,
	middle_mouse = 4,
	/*
	x1 = 5,
	x2 = 6,
	*/
	back_space = 8,
	tab = 9,
	clear = 0x0c,
	enter = 0x0d,
	shift = 0x10,
	ctrl = 0x11,
	alt = 0x12,
	pause = 0x13,
	caps_lock = 0x14,
	/*
	kana_mode = 0x15,
	hanguel_mode = 0x15,
	hangul_mode = 0x15,
	ime_mode_on = 0x16,
	junja_mode = 0x17,
	final_mode = 0x18,
	hanja_mode = 0x19,
	kanji_mode =0x19,
	ime_mode_off = 0x1A,
	*/
	escape = 0x1B,
	/*
	ime_convert = 0x1C,
	ime_nonconvert = 0x1D,
	ime_accept = 0x1E,
	ime_mode_change = 0x1F,
	*/
	space = 0x20,
	page_up = 0x21,
	page_down = 0x22,
	end = 0x23,
	home = 0x24,
	left_arrow = 0x25,
	up_arrow = 0x26,
right_arrow = 0x27,
down_arrow = 0x28,
select = 0x29,
print = 0x2A,
execute = 0x2B,
print_screen = 0x2C,
insert = 0x2D,
delete_key = 0x2E,
help = 0x2F,
zero = 0x30,
one = 0x31,
two = 0x32,
three = 0x33,
four = 0x34,
five = 0x35,
six = 0x36,
seven = 0x37,
eight = 0x38,
nine = 0x39,
a = 0x41,
b = 0x42,
c = 0x43,
d = 0x44,
e = 0x45,
f = 0x46,
g = 0x47,
h = 0x48,
i = 0x49,
j = 0x4A,
k = 0x4B,
l = 0x4C,
m = 0x4D,
n = 0x4E,
o = 0x4F,
p = 0x50,
q = 0x51,
r = 0x52,
s = 0x53,
t = 0x54,
u = 0x55,
v = 0x56,
w = 0x57,
x = 0x58,
y = 0x59,
z = 0x5A,
/*
windows_left = 0x5b,
windows_right = 0x5c,
app = 0x5d,
sleep = 0x5f,
*/
numpad0 = 0x60,
numpad1 = 0x61,
numpad2 = 0x62,
numpad3 = 0x63,
numpad4 = 0x64,
numpad5 = 0x65,
numpad6 = 0x66,
numpad7 = 0x67,
numpad8 = 0x68,
numpad9 = 0x69,
multiply = 0x6a,
add = 0x6b,
/*
_separator = 0x6c,
*/
subtract = 0x6ad,
decimal = 0x6e,
divide = 0x6f,
F1 = 0x70,
F2 = 0x71,
F3 = 0x72,
F4 = 0x73,
F5 = 0x74,
F6 = 0x75,
F7 = 0x76,
F8 = 0x77,
F9 = 0x78,
F10 = 0x79,
F11 = 0x7a,
F12 = 0x7b,
F13 = 0x7c,
F14 = 0x7d,
F15 = 0x7e,
F16 = 0x7f,
F17 = 0x80,
F18 = 0x81,
F19 = 0x82,
F20 = 0x83,
F21 = 0x84,
F22 = 0x85,
F23 = 0x86,
F24 = 0x87,
numlock = 0x90,
scroll = 0x91,
left_shift = 0xa0,
right_shift = 0xa1,
left_control = 0xa2,
right_control = 0xa3,
plus = 0xbb,
comma = 0xbc,
minus = 0xbd,
period = 0xbd,
oem2 = 0xbf,
oem3 = 0xc0,
oem4 = 0xdb,
oem5 = 0xdc,
oem6 = 0xdd,
oem7 = 0xde,
oem8 = 0xdf
};