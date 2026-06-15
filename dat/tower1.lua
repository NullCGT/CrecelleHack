-- NetHack tower tower1.lua	$NHDT-Date: 1717178759 2024/05/31 18:05:59 $  $NHDT-Branch: NetHack-3.7 $:$NHDT-Revision: 1.3 $
--	Copyright (c) 1989 by Jean-Christophe Collet
-- NetHack may be freely redistributed.  See license for details.
--
--
-- Upper stage of Vlad's tower
des.level_init({ style = "solidfill", fg = " " });

des.level_flags("mazelevel", "noteleport", "hardfloor", "solidify")
des.map({ halign = "right", valign = "center", map = [[
                  --- --- ---  
                  |.| |.| |.|  
 ------------------S---S---S---
-|..............|.......+.+...|
|...............---+-----.-----
|.....\.............|||||...+.|
|...............---+-----.-----
-|..............|.......+.+...|
 ------------------S---S---S---
                  |.| |.| |.|  
                  --- --- ---  
]] });

local niches = { {19,01}, {19,09}, {23,01}, {23,09}, {27,01}, {27,09} };
shuffle(niches);

des.ladder("down", 27,05)
-- The lord and his court
des.monster("Vlad the Impaler", 6, 05)
des.monster("V",niches[1])
des.monster("V",niches[2])
des.monster("V",niches[3])
des.monster("blood golem", 8, 03)
des.monster("blood golem", 8, 07)
-- The brides; they weren't named in Bram Stoker's original _Dracula_
-- and when appearing in umpteen subsequent books and movies there is
-- no consensus for their names.  According to the Wikipedia entry for
-- "Brides of Dracula", the "Czechoslovakian TV film Hrabe Drakula (1971)"
-- gave them titles rather than (or perhaps in addition to) specific names
-- and we use those titles here.  Marking them as 'waiting' forces them to
-- start in vampire form instead of vampshifted into bat/fog/wolf form.
local Vgenod = nh.is_genocided("vampire");
local Vnames = { nil, nil, nil };
if (not Vgenod) then
   Vnames = { "Madame", "Marquise", "Countess" };
end
des.monster({ id="vampire lady", coord=niches[4], name=Vnames[1], waiting=1 })
des.monster({ id="vampire lady", coord=niches[5], name=Vnames[2], waiting=1 })
des.monster({ id="vampire lady", coord=niches[6], name=Vnames[3], waiting=1 })
-- The doors
des.door("closed",24,03)
des.door("closed",26,03)
des.door("closed",19,04)
des.door("locked",26,05)
des.door("locked",24,07)
des.door("locked",26,07)
des.door("closed",19,06)
-- treasures
des.object("chest", 07,05)

des.object("chest",niches[6])
des.object("chest",niches[1])
des.object("chest",niches[2])
des.object("chest",niches[3])
des.object({ id = "chest", coord=niches[4],
             contents = function()
                des.object({ id = "wax candle", quantity=math.random(4,8) })
             end
});
des.object({ id = "chest", coord=niches[5],
             contents = function()
                des.object({ id = "tallow candle", quantity=math.random(4,8) })
             end
});
-- We have to protect the tower against outside attacks
des.non_diggable(selection.area(00,00,31,10))
