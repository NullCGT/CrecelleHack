
local tut_ctrl_key = nil;
local tut_alt_key = nil;

function tut_key(command)
   local s = nh.eckey(command);
   local m = s:match("^^([A-Z])$"); -- ^X is Ctrl-X
   if (m ~= nil) then
      tut_ctrl_key = m;
      return "Ctrl-" .. m;
   end

   m = s:match("^M%-([A-Z])$"); -- M-X is Alt-X
   if (m ~= nil) then
      tut_alt_key = m;
      return "Alt-" .. m;
   end

   return s;
end

function tut_key_help(x, y)
   if (tut_ctrl_key ~= nil) then
      des.engraving({ coord = { x,y }, type = "engrave", text = "Note: Outside the tutorial, Ctrl-key combinations are shown prefixed with a caret, like '^" .. tut_ctrl_key .. "'", degrade = false });
      tut_ctrl_key = nil;
   end
end

des.level_init({ style = "solidfill", fg = " " });
des.level_flags("mazelevel", "noflip",
                "nomongen", "nodeathdrops", "noautosearch");
des.map([[
---------------------------------------------------------------------------
|...|.......|.............................................................|
|...|.......|.............................................................|
|.---.......|.............----------......................................|
|.|.|.......|.---------...|........|......................................|
|S-.---|+|+--.|.|.|.|.|...|........|...|+|................................|
|.....||.|.|---.-.-.-.-----........|----.---..............................|
|.....||.|..+..........+...................|..............................|
|.....|-+------.-.-.-.-----........|----.---..............................|
|.....|...|...|.|.|.|.|...|........|...|+|................................|
|.....|...|...-S-------...|........|...|.|................................|
|--.---...---...|-------------------...|.|................................|
|.|.|.......|...|..................|...|.|................................|
|.|.|.......|...|..................|...|.|................................|
|.|.|.......|...|..................|...|.|................................|
|.|.|--...---...|..................|...|.|................................|
|.|...+...|.....|..................+...+.|................................|
---------------------------------------------------------------------------
]]);

des.region(selection.area(01,01, 73, 16), "lit");
des.non_diggable();
des.teleport_region({ region = { 3, 8, 3, 8 } });
nh.parse_config("OPTIONS=taunt:do the tutorial shuffle.");
-------- Entrance Area -----------
des.object({ coord = { 1, 6 }, id = "statue", montype = "blood golem", historic = true });
des.object({ coord = { 5, 6 }, id = "statue", montype = "black hole", historic = true });
des.object({ coord = { 1, 10 }, id = "statue", montype = "illusion", historic = true });
des.object({ coord = { 5, 10 }, id = "statue", montype = "squonk", historic = true });
des.engraving({ coord = { 3, 5 }, type = "engrave", text = "You can leave the tutorial via the magic portal", degrade = false });
des.trap({ type = "magic portal", coord = { 3, 4 }, seen = true });
---------- Secret area #1 ---------
des.door({ coord = { 1, 5 }, state = "locked"});
des.engraving({ coord = { 1, 4 }, type = "engrave", text = "You have found secret area #1/2", degrade = false });
des.engraving({ coord = { 3, 1 }, type = "engrave", text = "You can pick up snow from the ground.", degrade = false });
des.replace_terrain({ region={01,01, 03,02}, fromterrain=".", toterrain=".", coat="snow" })
---------- Secret area #2 ---------
des.engraving({ coord = { 15, 11 }, type = "engrave", text = "You have found secret area #2/2", degrade = false });
des.engraving({ coord = { 15, 13 }, type = "engrave", text = "Use the #taunt command to taunt", degrade = false });
des.engraving({ coord = { 11, 16 }, type = "engrave", text = "A custom taunt can be defined in your config file", degrade = false });
des.monster({ coord = { 11, 9 }, id = "mountain nymph", peaceful = 1, asleep = 1 });
---------- Introductions ----------
des.engraving({ coord = { 3, 11 }, type = "engrave", text = "Welcome to the CrecelleHack Tutorial. This tutorial is modeled off the 3.7 tutorial", degrade = false });
des.engraving({ coord = { 3, 13 }, type = "engrave", text = "This tutorial will walk you through a few key differences between this variant and vanilla", degrade = false });
des.engraving({ coord = { 3, 15 }, type = "engrave", text = "If you have not played NetHack before, please familiarize yourself with NetHack 3.7 first", degrade = false });
if (u.race == "kobold") then
    des.engraving({ coord = { 5, 16 }, type = "engrave", text = "As a kobold, your face a special challenge: your ability scores are mediocre and all status effects time out twice as fast", degrade = false });
else
   des.engraving({ coord = { 5, 16 }, type = "engrave", text = "Let's start with the floor coating system", degrade = false });
end
des.door({ coord = { 6, 16 }, state = "closed" });
--------- Floor Interactions -------
des.door({ coord = { 8, 8 }, state = "closed" });
des.engraving({ coord = { 8, 13 }, type = "engrave", text = "Try throwing this tonic at something", degrade = false });
des.object({ id = "tonic of acid", spe = 0, buc = "not-cursed", coord = { 8, 13} });
des.engraving({ coord = { 8, 7 }, type = "engrave", text = "Notice how the tonic splattered all over the floor", degrade = false });
des.engraving({ coord = { 8, 6 }, type = "engrave", text = "The dungeon in CrecelleHack can become coated in all sorts of things, including tonics, grass, and more", degrade = false });
des.door({ coord = { 8, 5 }, state = "closed" });
-------- Fire Spreading -------------
des.replace_terrain({ region={05,01, 11,04}, fromterrain=".", toterrain=".", coat="grass" })
des.door({ coord = { 10, 5 }, state = "closed" });
des.engraving({ coord = { 8, 3 }, type = "engrave", text = "Try standing on that altar and zapping this wand of fire south", degrade = false });
des.altar({ x = 8, y = 1, align="chaos", type="shrine" });
des.object({ coord = { 8, 3 }, id = "wand of fire", buc = "blessed" });
des.engraving({ coord = { 10, 7 }, type = "engrave", text = "There are many coating interactions to discover in your adventure", degrade = false });
des.door({ coord = { 12, 7 }, state = "closed" });
-------- The Hall of Rules Changes -----------
des.engraving({ coord = { 13, 7 }, type = "engrave", text = "Welcome to the hall of rule changes!", degrade = false });
des.engraving({ coord = { 14, 7 }, type = "engrave", text = "This hall covers the most critical rule changes to be aware of", degrade = false });
-- Niche
des.engraving({ coord = { 15, 6 }, type = "engrave", text = "Magic Cancellation is displayed on the status line", degrade = false });
des.object({ id = "cloak of protection", spe = 0, buc = "not-cursed", coord = { 15, 5 }})
-- Niche
des.engraving({ coord = { 17, 6 }, type = "engrave", text = "Dexterity contributes to AC instead of to-hit", degrade = false });
des.object({ coord = { 17, 5 }, id = "tonic of gain ability", buc = "blessed", quantity = math.random(3, 5) });
-- Niche
des.engraving({ coord = { 19, 6 }, type = "engrave", text = "Scrolls of Genocide have been renamed to Scrolls of Erasure", degrade = false });
des.object({ coord = { 19, 5 }, id = "scroll of light", buc = "not-cursed", name = "(Not a Real Erasure Scroll)" });
-- Niche
des.engraving({ coord = { 17, 8 }, type = "engrave", text = "Weapons and armor can be of different sizes and materials", degrade = false });
des.object({ coord = { 17, 9 }, id = "stiletto", material = "silver ", osize = 3, buc = "not-cursed" });
des.object({ coord = { 17, 9 }, id = "two-handed sword", osize = 1, buc = "not-cursed" });
des.object({ coord = { 17, 9 }, id = "scale mail", osize = 0, buc = "not-cursed" });
-- Niche
des.engraving({ coord = { 19, 8 }, type = "engrave", text = "Items can have special properties", degrade = false });
des.object({ coord = { 19, 9 }, id = "long sword", oprop = "crackling", buc = "not-cursed" });
-- Niche
des.engraving({ coord = { 15, 8 }, type = "engrave", text = "Unlocking tools can break", degrade = false });
des.object({ coord = { 15, 9 }, id = "lock pick", quantity = 5, buc = "not-cursed" });
des.object({ coord = { 15, 9 }, id = "chest", trapped = 0, locked = 1, buc = "not-cursed" });
des.door({ coord = { 15, 10 }, state = "locked"});

des.engraving({ coord = { 22, 7 }, type = "engrave", text = "Please note that this is not a comprehensive list of changes, merely a few salient ones.", degrade = false });
des.door({ coord = { 23, 7 }, state = "closed" });
--------- Commands Tutorial ------------
if (u.role == "Grappler") then
    des.engraving({ coord = { 24,7 }, type = "engrave", text = "As a grappler, standard attacks are grabs. Move after grabbing for a special attack.", degrade = false });
else
   des.engraving({ coord = { 24, 7 }, type = "engrave", text = "New commands are available for interacting with monsters.", degrade = false });
   des.engraving({ coord = { 26, 7 }, type = "engrave", text = "Try them out in this area.", degrade = false });
end
des.monster({ coord = { 28, 4 }, id = "red mold", peaceful = 0 });
des.engraving({ coord = { 28, 5 }, type = "engrave", text = "#Trip (".. tut_key("trip") ..") to knock monsters over.", degrade = false });
des.monster({ coord = { 30, 4 }, id = "brown mold", peaceful = 0 });
des.engraving({ coord = { 30, 5 }, type = "engrave", text = "#Shout (".. tut_key("shout") ..") to yell messages that will appear in server logs.", degrade = false });
des.monster({ coord = { 28, 10 }, id = "green mold", peaceful = 0 });
des.engraving({ coord = { 28, 9 }, type = "engrave", text = "#Grapple (".. tut_key("grapple") ..") to hold monsters in place.", degrade = false });
des.monster({ coord = { 30, 10 }, id = "yellow mold", peaceful = 1 });
des.engraving({ coord = { 30, 9 }, type = "engrave", text = "#Order (".. tut_key("order") ..") to issue commands to tame monsters.", degrade = false });
des.object({ coord = { 30, 9 }, id = "scroll of taming", buc = "blessed" });
des.engraving({ coord = { 41, 7 }, type = "engrave", text = "Exit tutorial", degrade = false});
des.trap({ type = "magic portal", coord = { 42,7 }, seen = true });
------------ Combat Tutorial ---------------
des.engraving({ coord = { 40, 8 }, type = "engrave", text = "Combat Changes", degrade = false });
des.door({ coord = { 40, 9 }, state = "closed" });
des.engraving({ coord = { 40, 10 }, type = "engrave", text = "In CrecelleHack, you may view item stats by selecting an item in your inventory and pressing '/'.", degrade = false });
des.object({ coord = { 40, 12 }, id = "athame", buc = "not-cursed" });
des.engraving({ coord = { 40, 12 }, type = "engrave", text = "Try examining this athame.", degrade = false });
des.engraving({ coord = { 40, 14 }, type = "engrave", text = "Note how the weapon's stats scale with your ability scores.", degrade = false });
des.engraving({ coord = { 40, 16 }, type = "engrave", text = "For every two points of an ability score, the number of damage dice rolled increases by one.", degrade = false });
des.door({ coord = { 39, 16 }, state = "closed" });
des.engraving({ coord = { 37, 09 }, type = "engrave", text = "Basic weapon skill allows you to wield weapons of that type without using an action.", degrade = false });
des.object({ coord = { 38, 09 }, id = "statue", montype = "gnome", historic = true });
des.engraving({ coord = { 37, 10 }, type = "engrave", text = "Skilled weapon skill allows you to identify weapons of that type by wielding them.", degrade = false });
des.object({ coord = { 38, 10 }, id = "statue", montype = "gnome lord", historic = true });
des.engraving({ coord = { 37, 11 }, type = "engrave", text = "Expert weapon skill increases the sides of damage dice rolled with that weapon type by one", degrade = false });
des.object({ coord = { 38, 11 }, id = "statue", montype = "gnome king", historic = true });
des.engraving({ coord = { 37, 13 }, type = "engrave", text = "To increase your ability scores you must #enhance them.", degrade = false });
des.object({ coord = { 38, 13 }, id = "statue", montype = "elf", historic = true });
des.engraving({ coord = { 37, 14 }, type = "engrave", text = "Weapon skills do not improve your to-hit or damage. Instead you must improve your ability scores.", degrade = false });
des.object({ coord = { 38, 14 }, id = "statue", montype = "elf-lord", historic = true });
des.engraving({ coord = { 37, 15 }, type = "engrave", text = "Many weapon skills have been combined.", degrade = false });
des.object({ coord = { 38, 15 }, id = "statue", montype = "elvenking", historic = true });
des.door({ coord = { 35, 16 }, state = "closed" });
des.engraving({ coord = { 34, 16 }, type = "engrave", text = "You may use this area to experiment with the combat system.", degrade = false });
des.monster({ coord = { 28, 14 }, id = "jackal", peaceful = 0, asleep = 1 });
des.object({ coord = { 17, 12 }, id = "dagger", buc = "not-cursed" });
des.object({ coord = { 18, 12 }, id = "short sword", buc = "not-cursed" });
des.object({ coord = { 19, 12 }, id = "long sword", buc = "not-cursed" });
des.object({ coord = { 20, 12 }, id = "two-handed sword", buc = "not-cursed" });
des.object({ coord = { 21, 12 }, id = "quarterstaff", buc = "not-cursed" });
des.object({ coord = { 22, 12 }, id = "mace", buc = "not-cursed" });

------------ Unfinished ------------------
des.engraving({ coord = { 40, 6 }, type = "engrave", text = "Unfinished Area", degrade = false });
des.door({ coord = { 40, 5 }, state = "locked" });

-- entering and leaving tutorial _branch_ now handled by core
-- // nh.callback("cmd_before", "tutorial_cmd_before");
-- // nh.callback("level_enter", "tutorial_enter");
-- // nh.callback("level_leave", "tutorial_leave");
-- // nh.callback("end_turn", "tutorial_turn");

