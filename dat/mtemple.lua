--       SCCS Id: @(#)mtemple.des  3.4     1993/02/23
--       Copyright (c) 1989 by Jean-Christophe Collet
--       Copyright (c) 1991 by M. Stephenson
--       Ported to lua by Kestrel Gregorich-Trevor with Paxed's lua conversion script.
-- NetHack may be freely redistributed.  See license for details.
--
-- This is the Temple of Moloch.
-- Within lie priests, demons, and, most importantly.... candles!
--
--
des.level_init({ style = "solidfill", fg = " " });

des.level_flags("mazelevel", "noflipy");

--0        1         2         3         4         5         6
--1234567890123456789012345678901234567890123456789012345678901234567890
des.map({ halign = "center", valign = "bottom", map = [[
            ----- ----- ----- ----- -----               
            |...| |...| |...| |...| |...|               
   ----------...---...---...---...---...|-|        -----
   |...|..................................|        |...|
   |...+..................................S########S...|
   |...|..................................|        |...|
   ----------...---...---...---...---...|-|        -----
            |...| |...| |...| |...| |...|               
            ----- ----- ----- ----- -----               
]] });
des.region(selection.area(00,00,55,08), "unlit")
--REGION:(39,03,41,05),unlit,"temple"
des.region({region={08,01,41,07}, lit=0, type="temple"})
des.stair("up", 05,04)
des.levregion({ region = {05,04,05,04}, exclude = {0,0,0,0}, type="branch" })
des.door("locked",07,04)
-- the altar of Moloch (making four will make four priests....)
des.altar({x=40,y=04,align="noalign",type="shrine"})
des.altar({x=40,y=04,align="noalign",type="shrine"})
des.altar({x=40,y=04,align="noalign",type="shrine"})
des.altar({x=40,y=04,align="noalign",type="shrine"})
-- flanking the doorway....
des.trap("spiked pit",06,03)
des.trap("spiked pit",06,05)
-- the treasure chamber!
for x = 52,54 do
    for y = 3,5 do
        des.object("chest", x, y)
    des.object("wax candle", x, y)
    des.gold({coord={x,y}})
    des.object({ x = x, y = y})
    des.object({ x = x, y = y})
    des.object({ x = x, y = y})
    end
end
-- five gargoyles on either side, in the niches of the temple
des.monster({ id = "gargoyle", x=14, y=1,peaceful=0,asleep=1})
des.monster({ id = "gargoyle", x=20, y=1,peaceful=0,asleep=1})
des.monster({ id = "gargoyle", x=26, y=1,peaceful=0,asleep=1})
des.monster({ id = "gargoyle", x=32, y=1,peaceful=0,asleep=1})
des.monster({ id = "gargoyle", x=38, y=1,peaceful=0,asleep=1})
des.monster({ id = "gargoyle", x=14, y=7,peaceful=0,asleep=1})
des.monster({ id = "gargoyle", x=20, y=7,peaceful=0,asleep=1})
des.monster({ id = "gargoyle", x=26, y=7,peaceful=0,asleep=1})
des.monster({ id = "gargoyle", x=32, y=7,peaceful=0,asleep=1})
des.monster({ id = "gargoyle", x=38, y=7,peaceful=0,asleep=1})
-- demons down by the altar...
des.monster({ id = "bone devil", x=37, y=2,peaceful=0,asleep=1})
des.monster({ id = "ice devil", x=38, y=2,peaceful=0,asleep=1})
des.monster({ id = "barbed devil", x=39, y=2,peaceful=0,asleep=1})
des.monster({ id = "vrock", x=37, y=6,peaceful=0,asleep=1})
des.monster({ id = "horned devil", x=38, y=6,peaceful=0,asleep=1})
des.monster({ id = "hezrou", x=39, y=6,peaceful=0,asleep=1})
-- a horde of zombies is also inside....
for x = 17,23 do
    for y = 3,5 do
        des.monster({ class = "Z", x=x, y=y,peaceful=0,asleep=1})
    end
end
des.engraving({06,04},"burn","Those Not of Moloch, Begone!")
