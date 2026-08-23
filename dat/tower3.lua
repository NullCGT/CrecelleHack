-- NetHack tower tower3.lua	$NHDT-Date: 1781994889 2026/06/20 22:34:49 $  $NHDT-Branch: NetHack-5.0 $:$NHDT-Revision: 1.2 $
--	Copyright (c) 1989 by Jean-Christophe Collet
-- NetHack may be freely redistributed.  See license for details.
--
des.level_init({ style = "solidfill", fg = " " });

des.level_flags("mazelevel", "hardfloor", "solidify", "outdoors", "noflip")
des.map({ halign = "right", valign = "center", map = [[
..................................................}}]}}}}}}}}}}}}}}}}}}}}}}
..................................................}}}}...................}}
.................................................}}}}.....................}
.................................................}}}........---.---.---...}
..................................................}}........|.|.|.|.|.|...}
..................................................}}......---S---S---S---.}
.................................................}}}......|.S.........S.|.}
..................................................}}....-----.........-----
...................................................}....+...|.........+...|
..................................................}.....|.---.........---.|
..................................................}}....|.|.S.........S.|.|
..................................................}.....|.---S---S---S---.|
..................................................}}....|...|.|.|.|.|.|...|
..................................................}}}...---.---.---.---.---
..................................................}}}.....|.............|}}
..................................................}}}}}...---------------}}
............................................}}....}}}}}}}}}}}}}}......}}}}}
................}}.........}.............}}}}}}..}}}}}}}}}}}}}}}}}}}}}}}}}}
}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}
}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}
]] });

-- THE FIELD
for i = 1, 30 do
  local o = des.object({ id = "corpse", montype = "human",
                        x = math.random(0, 51),
                        y = math.random(0, 19),
                        buried = true });
  o:stop_timer("rot-corpse");
  o:start_timer("zombify-mon", math.random(75,200));
end
des.region({ region={50,00, 51,19},lit=0,type="morgue",filled=0,irregular=1 })
local fields = selection.floodfill(00,00);
for i = 1, 10 do
  des.grave({ x = math.random(3, 51), y = math.random(0, 19)});
end

-- A few reference monsters hanging out
des.monster({id="panther", x = math.random(4, 51), y = 08, asleep=1})
des.monster({id="vampire bat", x = math.random(0, 51), y = 06, asleep=1})
des.monster({id="vampire bat", x = math.random(0, 51), y = 10, asleep=1})
-- Candles around the path leading in
des.object({ id = "candle", quantity = 1, lit = 1, x = 06, y = 10})
des.object({ id = "candle", quantity = 1, lit = 1, x = 09, y = 06})
des.object({ id = "candle", quantity = 1, lit = 1, x = 12, y = 10})
des.object({ id = "candle", quantity = 1, lit = 1, x = 15, y = 06})
des.object({ id = "candle", quantity = 1, lit = 1, x = 18, y = 10})
des.object({ id = "candle", quantity = 1, lit = 1, x = 21, y = 06})
des.object({ id = "candle", quantity = 1, lit = 1, x = 24, y = 10})
des.object({ id = "candle", quantity = 1, lit = 1, x = 27, y = 06})
des.object({ id = "candle", quantity = 1, lit = 1, x = 30, y = 10})
des.object({ id = "candle", quantity = 1, lit = 1, x = 33, y = 06})
des.object({ id = "candle", quantity = 1, lit = 1, x = 36, y = 10})
des.object({ id = "candle", quantity = 1, lit = 1, x = 39, y = 06})
des.object({ id = "candle", quantity = 1, lit = 1, x = 42, y = 10})
des.object({ id = "candle", quantity = 1, lit = 1, x = 45, y = 06})
des.object({ id = "candle", quantity = 1, lit = 1, x = 48, y = 10})


-- THE TOWER ITSELF
-- Entrance torches
des.object({ id = "candle", lit = 1, x = 56, y = 07})
des.object({ id = "candle", lit = 1, x = 56, y = 09})
-- Random places are the 10 niches
local place = { {61,04},{65,04},{69,04},{59,06},{71,06},
	   {59,10},{71,10},{61,12},{64,12},{69,12} }
des.levregion({ type="branch", region={02,08,02,08} })
des.ladder("up", 61,10)
-- Entry door is, of course, locked
des.door("locked",56,08)
des.door("locked",70,08)
-- Let's put a dragon behind the door, just for the fun...
des.monster("D", 69, 08)
des.monster({ x=68, y=07 })
des.monster({ x=68, y=09 })
des.monster("blood imp")
des.object("long sword",place[4])
des.trap({ coord = place[4] })
des.object("lock pick",place[1])
des.trap({ coord = place[1] })
des.object("elven cloak",place[2])
des.trap({ coord = place[2] })
des.object("blindfold",place[3])
des.trap({ coord = place[3] })
-- Walls in the tower are non diggable
des.non_diggable(selection.area(00,00,75,19))
