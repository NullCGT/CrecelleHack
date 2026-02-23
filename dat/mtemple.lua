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
des.object("chest", 52, 3)
des.object("wax candle", 52, 3)
des.gold({52,3})
des.object({ x = 52, y = 3})
des.object({ x = 52, y = 3})
des.object({ x = 52, y = 3})
des.object("chest", 53, 3)
des.object("wax candle", 53, 3)
des.gold({53,3})
des.object({ x = 53, y = 3})
des.object({ x = 53, y = 3})
des.object({ x = 53, y = 3})
des.object("chest", 54, 3)
des.object("wax candle", 54, 3)
des.gold({54,3})
des.object({ x = 54, y = 3})
des.object({ x = 54, y = 3})
des.object({ x = 54, y = 3})
des.object("chest", 52, 4)
des.object("wax candle", 52, 4)
des.gold({52,4})
des.object({ x = 52, y = 4})
des.object({ x = 52, y = 4})
des.object({ x = 52, y = 4})
des.object("chest", 53, 4)
des.object("wax candle", 53, 4)
des.gold({53,4})
des.object({ x = 53, y = 4})
des.object({ x = 53, y = 4})
des.object({ x = 53, y = 4})
des.object("chest", 54, 4)
des.object("wax candle", 54, 4)
des.gold({53,4})
des.object({ x = 54, y = 4})
des.object({ x = 54, y = 4})
des.object({ x = 54, y = 4})
des.object("chest", 52, 5)
des.object("wax candle", 52, 5)
des.gold({52,5})
des.object({ x = 52, y = 5})
des.object({ x = 52, y = 5})
des.object({ x = 52, y = 5})
des.object("chest", 53, 5)
des.object("wax candle", 53, 5)
des.gold({53,5})
des.object({ x = 53, y = 5})
des.object({ x = 53, y = 5})
des.object({ x = 53, y = 5})
des.object("chest", 54, 5)
des.object("wax candle", 54, 5)
des.gold({54,5})
des.object({ x = 54, y = 5})
des.object({ x = 54, y = 5})
des.object({ x = 54, y = 5})
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
des.monster({ class = "Z", x=17, y=3,peaceful=0,asleep=1})
des.monster({ class = "Z", x=18, y=3,peaceful=0,asleep=1})
des.monster({ class = "Z", x=19, y=3,peaceful=0,asleep=1})
des.monster({ class = "Z", x=20, y=3,peaceful=0,asleep=1})
des.monster({ class = "Z", x=21, y=3,peaceful=0,asleep=1})
des.monster({ class = "Z", x=22, y=3,peaceful=0,asleep=1})
des.monster({ class = "Z", x=23, y=3,peaceful=0,asleep=1})
des.monster({ class = "Z", x=17, y=4,peaceful=0,asleep=1})
des.monster({ class = "Z", x=18, y=4,peaceful=0,asleep=1})
des.monster({ class = "Z", x=19, y=4,peaceful=0,asleep=1})
des.monster({ class = "Z", x=20, y=4,peaceful=0,asleep=1})
des.monster({ class = "Z", x=21, y=4,peaceful=0,asleep=1})
des.monster({ class = "Z", x=22, y=4,peaceful=0,asleep=1})
des.monster({ class = "Z", x=23, y=4,peaceful=0,asleep=1})
des.monster({ class = "Z", x=17, y=5,peaceful=0,asleep=1})
des.monster({ class = "Z", x=18, y=5,peaceful=0,asleep=1})
des.monster({ class = "Z", x=19, y=5,peaceful=0,asleep=1})
des.monster({ class = "Z", x=20, y=5,peaceful=0,asleep=1})
des.monster({ class = "Z", x=21, y=5,peaceful=0,asleep=1})
des.monster({ class = "Z", x=22, y=5,peaceful=0,asleep=1})
des.monster({ class = "Z", x=23, y=5,peaceful=0,asleep=1})
des.engraving({06,04},"burn","Those Not of Moloch, Begone!")
