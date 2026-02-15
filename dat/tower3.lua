-- NetHack tower tower3.lua	$NHDT-Date: 1652196038 2022/05/10 15:20:38 $  $NHDT-Branch: NetHack-3.7 $:$NHDT-Revision: 1.1 $
--	Copyright (c) 1989 by Jean-Christophe Collet
-- NetHack may be freely redistributed.  See license for details.
--
des.level_init({ style = "solidfill", fg = " " });

des.level_flags("mazelevel", "noteleport", "hardfloor", "solidify", "outdoors", "noflip")
des.map({ halign = "right", valign = "center", map = [[
...........................................................................
...........................................................................
...........................................................................
............................................................---.---.---....
............................................................|.|.|.|.|.|....
..........................................................---S---S---S---..
..........................................................|.S.........S.|..
........................................................-----.........-----
........................................................+...|.........+...|
........................................................|.---.........---.|
........................................................|.|.S.........S.|.|
........................................................|.---S---S---S---.|
........................................................|...|.|.|.|.|.|...|
........................................................---.---.---.---.---
..........................................................|.............|..
..........................................................---------------..
...........................................................................
...........................................................................
...........................................................................
]] });

-- THE FIELD
for i = 1, 30 do
  local o = des.object({ id = "corpse", montype = "giant",
                        x = math.random(0, 56),
                        y = math.random(0, 19),
                        buried = true });
  o:stop_timer("rot-corpse");
  o:start_timer("zombify-mon", math.random(56, 150));
end
des.region({ region={00,00, 56,19},lit=0,type="morgue",filled=0,irregular=1 })
local fields = selection.floodfill(00,00);
for i = 1, 20 do
  des.grave({ coord = fields:rndcoord(1) });
end

-- THE TOWER ITSELF
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
des.monster("human werewolf")
des.monster("human werewolf")
des.monster("panther")
des.monster("vampire bat")
des.monster("vampire bat")
des.monster("blood imp")
des.monster("nurse")
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
