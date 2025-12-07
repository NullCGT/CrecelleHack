# 1.4
## Object Materials Patch
- This version of the object materials patch is pulled from
  EvilHack, meaning that the code was written by
  aosdict (copperwater), K2, saltwatterterrapin,
  and entrez. Please let me know if I'm forgetting anyone.
  I had to do an enormous amount of work to update it to
  play nicely with 3.7, and I probably still missed some
  things. There are a few differences from EvilHack:
    - Much of the searing code looks very different due
      to 3.7 changes to how silver hits are handled.
    - Runeswords are made of steel by default, thus
      cutting down the length of Stormbringer's un-ID'd
      description.
    - Grimtooth is made of iron, and Frost Brand is made
      of ice.
    - Orcs do not hate mithril. There is no basis for
      this, and I would rather make orcs distinct in
      different ways.
    - Sabers are made of silver by default.
    - Any object that is somehow converted to meat or
      vegetable matter can be eaten.
    - Figurines have randomized material types.
    - New option, implicit_material, to quash default materials.
- Rare Material: Nightiron.
  - Generates very rarely.
  - -1 damage during the day, +3 at night.
  - Treated as iron for material hatred purposes.

## Artifacts
- Lucifer
  - Chaotic bone morningstar (does not fuzz).
  - +1d4 to-hit.
  - Against non-prone flying or floating monsters, does 1d20
    extra damage and automatically knocks them prone.
- Wrath of Sankis
  - Neutral golden pick axe (fuzzes)
  - +1d5 damage against dwarves.
  - Provides resistance to fire.
  - Bloodthirsty, like stormbringer.
  - When wielded, player constantly drips blood onto the floor.
- Acidfall
  - Similar to fire and frost brand, but corresponds to acid.
  - Can be invoked for temporary stoning resistance.
  - Originally from SpliceHack.
- Skullcrusher
  - Does 1d10 extra damage to any monster with a skull.
  - Destroys the helmet of any monster it strikes.
  - Originally from Slash'Em.
- Sympathy
  - Neutral intelligent quarterstaff (fuzzes)
  - +1d3 to-hit, +1d4 damage.
  - Whenever it is wielded or rubbed on an object, will change
    materials to match what it last touched.
- You may dual-wield fire and frost brand together.
- Unintelligent non-crossaligned artifacts do not resist being
  held second to another artifact. Based on YANI archive #845.
- Sunsword deals double damage at midday.

## Spells
- Force field
  - Creates a force field which blocks movement.
  - Level 4 escape spell.

## Monsters
- Lawful monsters that cast clerical spells create force
  fields around the player instead of summoning insects.
- Chaotic monsters that cast clerical spells cause random
  potions to rain on the player instead of summoning insects.
  It is important to note that this impacts Master Kaen.
- Unaligned priests use a random set of priest spells.
- Fixed randomly generated quest leaders not having
  their items.

## Races
- Elf
  - Elves are not used to meat. They take an alignment
    penalty and begin throwing up after eating any kind of
    meat.
- Gnome
  - Gnomes start the game at the top of the gnomish mines.
  - Gnomes have many new inventory substitutions.
  - Gnomes hunger more slowly.
  - Gnomish knights begin with saddled large dogs.

## Roles
- Barbarian
  - Barbarians begin with cram rations rather than food
    rations, potentially resulting in inventory substitutions.

## New Objects
- Gnomish Pointy Hat
  - Ported from dNetHack. Differences are as follows:
    - Pointy hats do not contain a candle.
    - Pointy hats grant a point of luck when worn by gnomish players.
- Scroll of Transmute Material
  - Transform an object's material to another random material.

## New Monsters
- Transmuter (R)
  - Originally from SpliceHack, but implemented in original form here.
  - Changes the material of items.
- Knight Watch (K)
  - Appears when robbing a shop at night, replacing standard keystone
    kops. They are more dangerous.
  - They also appear when a shop is robbed if the player is a knight.

## Objects
- New wand appearances: meat and leek.
- Eating a wand of death is instantly fatal.
- Figurines weigh much less.
- Nerf skulls and skull helms so they do not provide extrinsics.
- Switched from previous object harmonics system to a much more
  contemporary oprop system.
    - Oprops are not known unless the item is identified or probed.
    - Angels spawn with harmonic weapons.
    - When unknown, appear as "harmonic."
    - Creating excalibur does not remove the oprop.
- Rename wand of water to wand of aqua bolt.
- All characters can dual-wield weapons (badly).
- Breaking a wand of force creates a force field.
- Zapping a wand of growth at a tree will cause it to become
  fruitful again.

## Options
- implicit_material: quash default material names.
- shorten_buc: shortens full buc name to [B][U][C].
- no_flipped_soko: stop sokoban levels from being flipped.

## Dungeon Changes
- Every tree grows a specific kind of fruit.
- Looking at a tree now tells you what kind of fruit it grows.
  Tourists simply see trees, because they are not familiar with
  the local flora.
- Shopkeepers now lock their shops when the sun sets and unlock
  them when the sun rises.

## Conducts
- Conflictless: Never generated conflict (from xNetHack).
- Blessless: Never blessed an item with holy water.

## Misc
- Added some secret areas to the tutorial.
- Added many new ways to drip liquid or cause monsters to drip liquid.
- Indicate dripping on the status line.

# 1.3

## Coating System
- Improved descriptions for walls made of dirt and sand.
- Added several options related to coating display that can be altered
  via config file.
- Improved messaging around coatings in several instances.
- Frost no longer causes ice-esque slippage.
- Water damage causes empty bottles to fill with water (suggested by
  karafruit).
- You can dip things into acidic monsters that engulf you.
- Improved the names of harmonic weapon oprops.
- You can wish for specific harmonic oprops in wizard mode.

## Dungeon Changes
- Dungeon is generated in biomes. This replaces the previous system of
  gradually decreasing grass and increasing fungi.

## Misc
- Reverted changes to reflection from 1.0.
- New feedback for cracked and greasy mirrors. Cracked mirrors scare
  monsters more easily.
- Support 4 status lines in curses.
- Many additional options for config files.
  - obscure_role_obj_names is taken from dnethack.
- Ported xnethack's player orientation patch, based on some ancient
  Splice code I wrote.

## Monster Changes
- Monsters can gain water walking, jumping.
  From SpliceHack, code ported directly from Evil and updated to 3.7.
- Monsters can gain telepathy from items, as well as bonus strength
  and magical breathing.
- Kobolds have been altered to skew closer to their appearance in early
  editions of D&D as well as anime like Record of Lodoss War. This is
  to say that their doglike features have been emphasized.
- Water trolls have a generation probability of zero, allowing them
  to spawn in certain branches and biomes.
- Dust vorticies regenerate health incredibly quickly when on sand
  or dirt.
- Fire, steam, and ice vorticies can absorb nearby coatings to rapidly heal
  and occasionally level up.

## Item Changes
- Implemented the weapon size system, extremely similar to the one that
  exists in dnethack. The differences are as follows:
  - Weapons larger than the player's size are always two-handed. Weapons
    smaller than the player's size are always one-handed.
  - Weight modifications are less drastic.
  - implicit_medium is a config option that allows control over whether
    "medium" is omitted from sized item descriptions. Defaults to true.
- Revert changes to scrolls of enchant weapon.
  - This change was undercooked and damaged game balance significantly.
- Throwing or hitting monsters with lanterns or lamps can cause oil to
  splatter everywhere. If the object is lit and nonmagical, the oil
  can occasionally catch on fire.
- Dipping a unicorn horn into hazardous waste will purify it.

## New Monsters
- Killer Coins
  - These are from SLASH'EM, but have been substantially modified here,
    because monsters should present interesting problems and ideas.
  - Different size piles of killer coins have been renamed.
  - When killed, killer coins explode into gold pieces, showering the
    area with coins. As a side effect, any death drops are also scattered.
  - Coins are additionally immune to stoning, have a weight, and can
    be rarely generated when polypiling gold.
- Phantom Fungus
  - Originally from dnethack. Identical here, with the exception that
    they do not generate except in fungal biomes, and their AC is
    lower.
- Goat
  - Originally appears in Slash'EM. Identical here, but only spawn
    in woodlands and are also metalivorous (because they eat tin cans).
  - Have variant male and female names.
- Sand Golem
- Spectre
  - Ported from EvilHack, but originally from SpliceHack.

## New Objects
- Resizing Kit
  - As in dnethack, it can be used to resize objects. Unlike dnethack, it
    offers more granular control over object size. Objects can be one
    size larger, one size smaller, or the same size as the player's
    current form. In addition, any object where size is a factor can
    be resized.
- Snowball
  - Can be obtained through some creative application of commands.

## Pets
- Allow the player to issue granular orders to their pets, directing
  them on how who to attack, whether to pick up items, and more.
- New skill: Leadership. Determines what kind of orders you can issue to
  pets.
- Maximum number of pets is now one third charisma. The code here is
  inhereted from EvilHack and dNetHack, but is more efficient. It also has
  a side effect of excess pets untaming one at a time, rather than all at
  once, so massive bursts of pets can still be semi-viable.
- Also from dnethack, skill in leadership increases pet follow distance.
- Orders can be issued by chatting with a pet or by using #order.

## Races
- New Race: Kobold
  - Although kobolds are playable in Grunt and SlashTHEM, their appearance
    here is distinct.
  - Kobolds are a challenge race, with overall poor ability scores. They
    always start with a kobold companion. Tame kobolds will never betray
    a kobold player.
  - Kobolds start with poison resistance, and like orcs they may eat
    tripe rations without penalty. They are of small size.
  - Kobolds can only talk to dogs and other kobolds, but have a chance of
    taming canines when talking to them.

## Roles
- Jumping is now the grappler special spell.

## Weather System
- First weather change takes much longer.
- First incoming weather system can no longer be a tornado.

# 1.2

## Dungeon Changes
- New minetown variants: Zootown and Lavender Town.
  - Lavender Town originates with me in Splice. I fixed a few issues
    with this one and also added a bit of variance to it.
  - Zootown is from Spork, modified by K2 for EvilHack, then ported
    to Lua by me in Splice.
- Bazaar Town has a 10% chance of generating as a cold level
  filled with frost.
- Swamps generate with mud in addition to fungus. This impacts
  juiblex's level.
- The Castle has increased variance from game to game. The following
  effects are possible:
  - Front of the castle has walls partially replaced by iron bars.
  - A storeroom contains potions.
  - Mimics mimic chests in some towers (idea from RGRN).
  - Surrounded by swamp instead of maze.
  - Monster and terrain variance based on temperature.
- Birds, rather than zombies, can appear in the castle courtyard.
- Castle maze extends closer to the backdoor of the castle.
- Add xNetHack's oracle level variants.

## Traps
- Sleeping gas traps actually create clouds of sleep gas.
- Sleeping gas traps in Gehennom may emit other types of gas.

## Items
- Winter Hat
  - Worn by mercenaries such as soldiers when generated on a
    level that is cold.

# 1.1

## Commands
- Add the #shout command. Present in many variants, but this uses
  the 3.7-compatible code written by aosdict/Phol En Wodan.

## Harmony System
- Complete rework of the harmony system.
  - Harmonic weapons now work differently depending on the type
    of harmony. Full details about this system can be found
    in doc/variantprops.md.
- Made harmonic objects rarer.
- Harmonic objects do not need to be identified to determine
  what they harmonize with.
- Dangerous monsters have a chance to spawn with harmonic objects
  related to their abilities.
- Reading a scroll of enchant armor while wearing a skull
  corresponding to a monster now not only transforms the skull
  into a skull helm, but harmonizes that helm with the monster's
  associated harmonies.
- Remove obscure ordering system for determining whether a harmony
  is active.
- Disable monster double damage harmonization while reworking
  the system to be more interesting.

## Monsters
- Slightly increase chance of random spawns of unique monsters.
- Yellow dragons leave behind a trail of acid.
- More types of monsters spread fire when they move.
- Monsters can spawn with and throw potions of hazardous waste.
- Monsters can intentionally throw potions to trigger alchemical
  blasts.
- Monsters can seek out and put on eyewear.
- Monsters can wear amulets of change in order to change genders.
- Monsters that hatch from eggs are considered to be babies, and
  their names reflect this. This has no impact on gameplay.
- Implemented the monster max HP changes from xNetHack (originally
  from EvilHack, by way of SporkHack).
- Monsters considered "roguish" can spawn with poisoned items or
  potions of sickness.
- Monsters can dip items into potions.
- Ported EvilHack and Spork's covetous monster behavior tweaks.
- Ported EvilHack's ability for monsters to break boulders.
- Made some fixes to defsym.h to clarify monsters.
- Reduced nightcrust light emission range.
- Water elementals soak enemies with water on hit.
- Greater variance in the weapons that monsters spawn with.
- Crimson deaths create a massive cloud of bloody mist upon death.
- Change name of wargs to dire wolves.

## New Monsters
- Salt Golem
- Mustelid
- Giant Mustelid

## Unique Monsters
- Many new unique monsters.
- The deferred Goblin King boss is now a potential unique
  monster spawn.

## Objects
- Dented pots have been renamed to Yendorian Bascinets.
- Add in some additional helmet and cloak appearances to be
  shuffled into the existing ones.
- Yellow dragon scales and yellow dragon scale mail cause you
  to leave behind a trail of acid.
- Wands of polymorph leave behind random potion coatings when
  zapped and remove all existing coatings.
- Wands of cancellation cancel potion coatings.
- Wands of make invisible change existing potion coatings into
  potions of invisibility.
- Lower damage and raise volatility of potions of hazardous
  waste.
- Allow acid spilled by acid beams to alchemize.
- Tools can become eroded.
- All blades are considered poisonable.
- All players recognize water.
- Added ice as a material, which is used for icicles and for a
  new randomized wand appearance. It can both shatter and be
  melted, making it extremely delicate.
- Significantly increased wand of teleportation rarity.

### New Objects
- Bestiary
  - Reading it allows the player to learn information about
    a monster, so that they can fully view that monster's
    stats when using the farlook command.
- Scroll of Control Weather
  - Blessed: Direct control over the current weather.
  - Uncursed: Swaps current weather from rain to shine.
  - Cursed: Causes bizarre inclement weather.
  - Confused and not cursed: Summons a tame tornado.
- Scroll of Maze
  - Allows you to send a monster to a randomly generated maze.
    This does not affect Riders, but it bypasses all forms of
    magic resistance.
- Mirrored Glasses
  - Provide reflection.
- Tinker Goggles
  - Block flash effects, as sunglasses do.
  - Protects from alchemic blasts when performing alchemy.
- Skeleton
  - Found on the ground of the dungeon, and occasionally
    created through other situations, such as by zapping
    a wand of death or being struck by lightning.s
- Banana Peel
  - Replaces bolas.
- Acoustic Guitar
  - Tonal instrument.
- Electric Guitar
  - Fires bolts of electricity, as a fire or frost horn would.
- Wand of water
  - Does damage as a wand of striking, but cannot be resisted.
  - Coats things in water.
- Potion of teleportitis
  - Non-cursed potions teleport the drinker.
  - Gas released from the potion teleports those that breathe it.

## Floor Coating System
- Added frost and mud as a floor coatings.
  - Frost causes those walking over it to slip, as walking over ice.
  - Mud costs additional movement points to move through. Additionally,
    many monsters can hide themselves in the mud in order to ambush
    the player.
- Floor coatings can be added in level generation lua. Maps
  have been updated to reflect this.
- Allow blood golems to be created while polypiling.
- Remove cockatrice blood once it petrifies something.
- Only healers recognize the origin of pools of blood.
- Monsters and players can now drip with potions, spreading them
  across the level. You stop dripping after a short time, but can
  hasten the process using a towel.
- You can #tip potions onto yourself to douse yourself in a liquid.
- You can clean liquids off yourself by using #wipe.
- Towels can now be used to wipe the floor by engraving with them, rather
  than by using #rub.
- New messages for hurtling across coatings.
- Fire traps now evaporate potion puddles.
- Drain life destroys fungal and grass coatings.
- Can now use #name to name potions on the floor.

## Dungeon Changes
- Allow fountains to become frozen. Frozen fountains cannot be
  dipped into or quaffed from.
- New special room: Laboratory
- Fixed many special rooms having grass inside.

## GUI
- Farlook a monster with (:) to see a summary of a monster's stats,
  including attacks and harmonies.
- Examining an item in one's inventory allows one to view the item's
  stats.
- You can learn more about monsters' attacks by getting hit by them,
  chatting with them, using a stethoscope, or a wand of probing.
- Default to 3 status lines.
- Add code to windowports to support botl handling of many more statuses.
- Switch riding to opt_in on botl and being held to opt_out.

## Roles and Races
- Rewrote the wrestler role entirely.
  - Wrestlers are now known as grapplers.
  - Attacking a monster as a grappler now initiates a grap instead of an attack.
  - The direction one moves while holding a grabbed monster determines the kind
    of attack one performs against it.
  - Using #grapple while holding a monster releases that monster instead of
    executing a special move.

## Skills
- New Skill: Improvised Weaponry
  - Improvised weaponry allows players to improve their to-hit with
    abnormally wielded objects.
  - Grappler damage for improvised weapons caps at 1d20 instead of 1d6.

## Misc
- Bonfires no longer block sight.
- Potions destroyed in chests now splatter everywhere and create clouds of
  gas.
- Potion vapors are tracked and displayed as poison gas and mist would be,
  and can impact both players and monsters in new ways.
- Trapped chests that create gas clouds actually create clouds of gas.

# 1.0

## Artifacts

### Type Randomization
- Many artifacts have semi-randomized object types.
  - The randomized object type must have a similar damage
    type to the original type of the artifact. For example,
    Sunsword cannot be a mace.
  - The brands are always the same type.

## Commands

### \#grapple: Grapple a monster
- Grappling a monster holds that monster in place.
- Trains the grappling skill, which allows you to escape
  and execute grabs more easily.
- Use special techniques by using #grapple when you are
  already grappling an opponent.

### \#trip: Trip a monster.
- Tripping traditionally uses one's lower body, but polearms,
  whips, and flails can be used to trip as well.
- Being tripped puts one into a downed state. Until one moves
  to a different location, AC and to-hit are reduced.
- While in a prone state, all harmonic bonuses are negated. This
  applies to both players and monsters.

### \#taunt: Taunt nearby monsters.
- Wastes a few turns and wakes up nearby monsters.
- A custom taunt can be defined in the options file. Custom taunts
  are prepended by the word "You" when used in game.

### \#twoweapon
- can be used to attack with both ends of a double-headed
  weapon, such as a quarterstaff, two-bladed sword, or dual axe.

## Floor Coating System
- Room floors can be dirt or stone. 
- Floors can be coated with various substances, such as potions, grass,
  blood, and more.
- These coatings interface with the harmony system.
- All of these coatings can be interacted with in various ways, to
  various effects. Have fun!

## Day-Night Cycle
- Exactly what it sounds like! Watch your vision wax and
  wane over time!

## GUI
- MC is displayed on the status line as a percentage.

## Harmony System
- Most monsters harmonize on a specific type of terrain. While
  harmonizing, a monster deals double damage.
- Harmonic weapons and armor provide massive bonuses while standing
  on terrain that they harmonize with.
  - Armor grants 3 bonus points of AC while harmonized.
  - Weapons deal double damage when harmonized.

## Monsters

### General
- Dogs and large dogs now have breed names attached to their names.
- YANI from Riker on the YANI Archive: Werecreatures that transform
  out of sight are disguised as standard creatures of their type.
- Horses and ponies have altered names.
- Tigers are orange.
- Monsters can grapple and trip the player.
- Bats and vampires only spawn at night.
- Renamed mordor orcs to fen orcs and uruk-hai to fell orcs.

### Unique Monsters
- Dispater uses the trickster monster spell list, making him likely
  to both disguise himself and summon illusory duplicates.
- Orcus uses the undead monster spell list identical to the one used
  by liches.
- In the course of dungeon exploration, one may encounter unique monsters.
  Further details about these monsters are detailed in uniquefixes.txt.

### Mounts
- The following monster classes are now valid mounts: f, d
- Many monsters now do not spawn in Gehennom.

### Monster Spells
- Different monsters have different spell lists.
- Many existing spells take advantage of the floor coating system.
- Cure self now heals additional hp for high-level monsters.
- See variantspells.md for a full list of new spells.

### New Monsters
- 20 new monsters.
- See variantmonsters.md for a full list of these monsters and mild
  spoilers.

## Monster AI

### General
- Incorporated Slash'Em's traitor system, but updated to 3.7.
- Monsters will slather items with cans of grease if the player is
  polymorphed into a relevant form.

### Pathfinding
- Some monsters will avoid following the player into chokepoints.
- Monsters will only flee from the player if they could conceivably
  outrun them.
- Monsters do not track the player across the entire level, as in
  vanilla.

## New Role: Wrestler
- Use your special wrestling moves and the #grapple command to
  obtain the amulet!

## Objects

### General
- Renamed Scroll of Genocide to Scroll of Erasure.
- Renamed large shield to kite shield and small shield
  to roundshield.
- Quarterstaves can be wielded in dual weapon mode.
- Potions leave behind empty bottles when drunk. These bottles
  can be refilled with water at fountains, sinks, and other
  locations.
- Potions of water are considered inert and do not polymorph.
- Potions of holy water deal much more damage to demons.
- Bags have distinct appearances.
- Non-stacking weapons, armor, and weapon-tools have their
  weight mildly fuzzed.
- All gems shatter when thrown, regardless of their material.
  This opens up an additional route for getting glass onto
  the floor.

### Armor
- MC: Chance of cancelling a special effect.
  - Every piece of armor adds some amount of magic cancellation.
  - Maxes out at 90%.
  - Each point of enchantment reduces the MC of that piece of equipment by 1%.

### Weapons
- Some weapons are "finesse weapons." To-hit bonuses with
  finesse weapons are determined based on strength or
  dexterity, whichever is higher.
- Some weapons have a speed bonus or a speed penalty.
  - Generally, orc weapons are faster, elven weapons deal more damage,
    and dwarvish weapons deal more damage but are slower.

### New Objects
- Wand of Fecundity
  - Causes grass to grow when zapped.
  - Zapped monsters grow up.
  - Zapped eggs instantly start hatching.
- Skull
  - Derived from a monster.
- Skull Helm
  - Can be obtained by using a non-cursed scroll of enchant armor
    to enchant a worn skull.
  - Provide the same type of resistances possessed by the original
    monster. 
- Potion of blood
  - Function similarly to potions of blood from dnethack, although
    the implementation is different.
- Sunglasses
  - Provide protection against flash-based blindness.
- Shepherd's Crook
  - Similar to a quarterstaff.
  - Can be used for tripping purposes.
  - Can be wielded in dual mode.
- Bolas
  - Thrown weapon.
  - Players struck by a bolas are made prone.
- Heater Shield
- Two-Bladed Sword
- Dual Axe

### Locks and Picks
- Unlocking tools now stack.
- Unlocking tools have a small chance of being destroyed upon use.
  The chance is dependent upon the type of unlocking tool.

### General
- Roles are not restricted by race, with the exception of
  tourists (always human).

### Knights
- Knights receive a different mount depending on race.
  Orcs: Warg
  Elves: Jaguar

## System Changes

### Attributes
- Dexterity contributes to AC.
- Strength is normally the sole contributor to to-hit.

### Reflection
- Reflection does not reflect rays directly back at
  the one that fired them. Instead, the rays bounce off at a ninety degree
  angle. As a consequence, reflection no longer instantly kills Medusa.

### To-Hit Changes
- As in dNetHack, d(1, Luck) is added to your to-hit instead
  of Luck.
- New To-Hit bonuses: Restricted: -2 Unskilled: 0 Basic: +1 Skilled: +2
  Expert +3
- New Damage bonuses: Restricted: -2 Unskilled: 0 Basic: +1 Skilled: +2
  Expert: +3

### Misc
- Ported Hardfought's HTML Dumps and other tweaks.
- Ported xNetHack's invweight patch. Thanks to aosdict.
- Ported NetHack4's altar identification of items in bags. The code
  is pulled from dNetHack, but is likely identical to NetHack4's.
- Ported DynaHack's Enhanced Enhance menu. Code is pulled from
  some SpliceHack-Rewrite code I wrote a long time ago.

## Weather System
- Experience wind, rain, hail, and more.
- Weather changes dynamically over time, and may impact the gameplay
  in various ways.