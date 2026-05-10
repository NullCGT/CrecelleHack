# CrecelleHack: A NetHack 5.0 Variant

CrecelleHack is an actively maintained variant of NetHack 5.0. Delve into
a lush, growing dungeon filled with strange creatures and retrieve the
Amulet of Yendor for your god.

## Configuration

- Information about Crecelle-specific config file options are available
at the top of [the opthelp file](dat/opthelp).
- In addition,
[an example config file](sys/windows/crecellehackrc.template)
is available to be copied.
- If you would like to view these options in game, the top of the
  #options menu is populated by Crecelle-specific options.

## Documentation

Note that many of the following documentation files contain spoilers.
Read at your own risk.

- [Changelog](doc/variantfixes.md)
- [New Monsters Spoiler](doc/variantmonsters.md)
- [Unique Monster Spoiler](doc/variantuniques.md)
- [Monster Spell Spoiler](doc/variantspells.md)
- [Object Properties Spoiler](doc/variantprops.md)

## Design Goals

- Craft a dungeon that feels more alive and fosters a sense of place.
  NetHack is a fantasy soup game, with most of its ideas derived from
  D&D 1e. While this is not necessarily a bad thing and is not something
  I wish to totally divorce Crecelle from, it is not entirely to my
  taste, and I think that a stronger setting would make for a much stronger
  game. This place isn't just a deathtrap or generic dungeon, it's a home.
- Promote strategic gameplay and open up additional character development
  options. NetHack is a game in which character builds, such that they are,
  include very little variation. Further, the lack of a strong clock and
  the presence of burst economy in the form of wishes ensures that players
  do not attempt to ascend until they have everything they need. The part of
  NetHack that I find most fun (improvising in order to escape from difficult
  situations) vanishes not only with player skill, but with character skill,
  as players minimize all potential risks that their character might face.
- Add monsters and items that are interesting for reasons beyond their stat
  blocks. Further, diversify existing monsters, forcing the player to vary
  how they approach them. Many monsters in NetHack tend to blend together,
  particularly in the lategame. While I do not believe it is possible to
  fully solve this problem, I believe that with careful modification of
  NetHack's combat system and enemies it is possible to craft a combat
  system with greater depth.
- Implement ideas that seem impossible to implement in NetHack. I've been
  coding NetHack variants and coding in C for a long time, and I get a lot
  of joy out of implementing SMOPs. I want to surprise players with things
  that they thought they would never see in NetHack, like potions coating
  and alchemizing on the floor and a dynamic day/night cycle.

## Ascending

So you ascended! Congratulations. If you post an issue with proof that you
did so, I will add your character (or a character you create) to the game
in the form of a unique monster. One per person, while supplies last, and
I reserve the right to reject anything in poor taste or remove them in
the future. I'm sure this is a good idea and that only good things will
come of this.

## FAQ

### What license is this under?

CrecelleHack of course falls under the NetHack license.

### What happened to SpliceHack?

SpliceHack is no longer maintained by me. This variant is, in many ways,
a design evolution of SpliceHack. It's more focused, more tightly coded,
less buggy, and hopefully more culturally sensitive. It's not perfect
(we've inherited the Samurai role from vanilla NetHack, after all) but
I'm endeavouring not to make the same missteps I feel that I did previously.

### Why doesn't this variant support tiles?

I love supporting tileset-based play, and I wish I could. Unfortunately, many
of the features in CrecelleHack are impossible to represent graphically and
rely on ascii for presentation. Any kind of tiles support would require
significant changes in how graphical tiles are handled by NetHack.

### How do I report a bug?

Open up a GitHub issue, or contact me in IRC or Discord. This goes for bugs
as well as YANIs, YASIs, and EPIs. Please don't bother the NetHack DevTeam
about anything in this variant. I don't really look at Reddit or newsgroups,
so posting there is unlikely to get an answer.

Please don't hesitate to open up an issue. I'm human and I make mistakes; 
if something in the game is broken, insensitive, or just plain not fun, I would
always rather know.

### How do I compile this?

Exactly as you would compile vanilla NetHack 3.7.
