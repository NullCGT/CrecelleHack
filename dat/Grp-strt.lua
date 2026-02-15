des.level_init({ style = "solidfill", fg = " " });

des.level_flags("mazelevel", "noteleport", "hardfloor", "outdoors")

des.map([[
|---------|--------------------------------------------------|--------------|
|.........|............|||...................................|....+....+....|
|--..--...|.................................|.....|........|.|....|....|....|
||....|...|....................------.......|................|-----....-----|
--....--....................................|.....--------...|....+....+....|
|T....T|...............|||..............--..|................|....|....|....|
|......|------------------}}}}....}}}}-.....|.......--.......|-----....-----|
.T....T..................|}..........}|.....|.......|....|...|....+....+....|
.........................|}..........}|..................|...|....|....|....|
.........................|}..........}|.........---....---...|-----....-----|
.........................|}..........}|...........|..........|....+....+....|
.........................|}}}}}}}}}}}}|...........----.......|....|....|....|
.........................-------------|...------.............-------++------|
.........................|............|......|...........--.................|
.........................|............|......|......||...|..................|
......................................|.............||.............----++---|
......................................|..------.....||.....|.|.....|........|
......................................|.......|............---.....|........|
.........................|............|.......|..................--|........|
.........................|------------|............................|--------|
]]);

des.replace_terrain({ region = { 0, 8, 24, 19}, fromterrain=".", toterrain="T", chance=7 })
-- Dungeon Description
des.region(selection.area(00,00,75,19), "unlit")
-- Stairs
des.stair("down", 75,01)
-- Portal arrival point
des.levregion({ region = {37,16,37,16}, type="branch" })
-- Masked Mummy, the quest leader in hiding
des.monster({ id = "Masked Mummy", coord = {71, 17}})
-- The treasure of the quest leader
des.object("chest", 71, 17)
-- Trainees and their loot.
des.monster("trainee", 62, 01)
des.object({ x = 62, y = 01 })
des.monster("trainee", 62, 04)
des.object({ x = 62, y = 04 })
des.monster("trainee", 62, 07)
des.object({ x = 62, y = 07 })
des.monster("trainee", 75, 10)
des.object({ x = 62, y = 10 })
des.monster("trainee", 75, 01)
des.object({ x = 75, y = 01 })
des.monster("trainee", 75, 04)
des.object({ x = 75, y = 04 })
des.monster("trainee", 75, 07)
des.object({ x = 75, y = 07 })
des.monster("trainee", 75, 10)
des.object({ x = 75, y = 10 })
-- Lock your doors!
des.door("locked", 66, 01)
des.door("locked", 66, 04)
des.door("locked", 66, 07)
des.door("locked", 66, 10)
des.door("locked", 71, 01)
des.door("locked", 71, 04)
des.door("locked", 71, 07)
des.door("locked", 71, 10)
des.door("locked", 71, 15)
des.door("locked", 72, 15)

-- Non diggable walls
des.non_diggable(selection.area(00,00,75,19))
-- Training traps
des.trap({ x = 27, y = 7})
des.trap({ x = 27, y = 8})
des.trap({ x = 27, y = 9})
des.trap({ x = 27, y = 10})
des.trap({ x = 36, y = 7})
des.trap({ x = 36, y = 8})
des.trap({ x = 36, y = 9})
des.trap({ x = 36, y = 10})
-- bears to stop the owlbears
des.trap("bear")
des.trap("bear")
des.trap("bear")
des.trap("bear")
des.trap("bear")
des.trap("bear")
des.trap("bear")
des.trap("bear")
des.trap("bear")
des.trap("bear")
des.trap("bear")
des.trap("bear")
des.trap("bear")
des.trap("bear")

-- Wrestlerssss
des.monster({ id = "rope golem", peaceful = 0 })
des.monster({ id = "owlbear", peaceful = 0 })
des.monster({ id = "rope golem", peaceful = 0 })
des.monster({ id = "owlbear", peaceful = 0 })
des.monster({ id = "rope golem", peaceful = 0 })
des.monster({ id = "owlbear", peaceful = 0 })
des.monster({ id = "rope golem", peaceful = 0 })
des.monster({ id = "owlbear", peaceful = 0 })
des.monster({ id = "rope golem", peaceful = 0 })
des.monster({ id = "owlbear", peaceful = 0 })
des.monster({ id = "rope golem", peaceful = 0 })
des.monster({ id = "owlbear", peaceful = 0 })
des.monster({ id = "rope golem", peaceful = 0 })
des.monster({ id = "owlbear", peaceful = 0 })
des.monster()
des.monster()
des.monster()
des.monster()
des.monster()
des.monster()
des.monster()
des.monster()