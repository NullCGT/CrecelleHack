

#define MCF_NONE     0x0000
#define MCF_INDIRECT 0x0001 /* untargeted/indirect spell */
#define MCF_SIGHT    0x0002 /* monster needs to see hero */
#define MCF_HOSTILE  0x0004 /* cast by hostile monsters only */

#if defined(MCASTU_ENUM)
#define MONSPELL(def, nam, lvl, flags) MCAST_##def
#elif defined(MCASTU_INIT)
#define MONSPELL(def, nam, lvl, flags) { nam, lvl, flags }
#elif defined(DUMP_MCASTU_ENUM1)
#define MONSPELL(def, nam, lvl, flags) MCAST_DUMPENUM_##def
#elif defined(DUMP_MCASTU_ENUM2)
#define MONSPELL(def, nam, lvl, flags) { MCAST_DUMPENUM_##def, #def }
#endif

MONSPELL(PSI_BOLT,     "psi bolt",         0, MCF_HOSTILE|MCF_SIGHT),
MONSPELL(OPEN_WOUNDS,  "open wounds",      0, MCF_HOSTILE|MCF_SIGHT),
MONSPELL(DISGUISE,     "disguise self",    0, MCF_HOSTILE|MCF_INDIRECT),
MONSPELL(CURE_SELF,    "cure self",        1, MCF_INDIRECT),
MONSPELL(DARKNESS,     "darkness",         1, MCF_HOSTILE|MCF_SIGHT),
MONSPELL(GREASE,       "grease",           1, MCF_HOSTILE|MCF_SIGHT),
MONSPELL(BLOOD_RAIN,   "blood rain",       1, MCF_HOSTILE|MCF_INDIRECT|MCF_SIGHT),
MONSPELL(HASTE_SELF,   "haste self",       2, MCF_INDIRECT),
MONSPELL(BLOODRUSH,    "bloodrush",        2, MCF_INDIRECT),
MONSPELL(CONFUSE_YOU,  "confusion",        2, MCF_HOSTILE|MCF_SIGHT),
MONSPELL(MIRROR_IMAGE, "mirror image",     2, MCF_HOSTILE|MCF_INDIRECT),
MONSPELL(STUN_YOU,     "stunning force",   3, MCF_HOSTILE|MCF_SIGHT),
MONSPELL(SLEEP_YOU,    "sleepel",          3, MCF_HOSTILE|MCF_SIGHT),
MONSPELL(DISAPPEAR,    "invisibility",     4, MCF_INDIRECT),
MONSPELL(PARALYZE,     "hold",             4, MCF_HOSTILE|MCF_SIGHT),
MONSPELL(BLIND_YOU,    "blind",            6, MCF_HOSTILE|MCF_SIGHT),
MONSPELL(WEAKEN_YOU,   "strength of newt", 6, MCF_HOSTILE|MCF_SIGHT),
MONSPELL(DESTRY_ARMR,  "destroy armor",    8, MCF_HOSTILE|MCF_SIGHT),
MONSPELL(BLOOD_SPEAR,  "bloody pierce",    8, MCF_HOSTILE|MCF_SIGHT|MCF_INDIRECT),
MONSPELL(INSECTS,      "summon vermin",    8, MCF_HOSTILE|MCF_INDIRECT|MCF_SIGHT),
MONSPELL(RAISE_DEAD,   "raise dead",      10, MCF_HOSTILE|MCF_SIGHT|MCF_INDIRECT),
MONSPELL(LEVITATE_YOU, "levitation",      10, MCF_HOSTILE|MCF_SIGHT),
MONSPELL(CURSE_ITEMS,  "curse",           10, MCF_HOSTILE|MCF_SIGHT),
MONSPELL(FORCE_FIELD,  "force field",     10, MCF_HOSTILE|MCF_SIGHT|MCF_INDIRECT),
MONSPELL(CHAOS_RAIN,   "chaos rain",      10, MCF_HOSTILE|MCF_SIGHT|MCF_INDIRECT),
MONSPELL(LIGHTNING,    "lightning bolt",  11, MCF_HOSTILE|MCF_SIGHT),
MONSPELL(FIRE_PILLAR,  "divine wrath",    12, MCF_HOSTILE|MCF_SIGHT),
MONSPELL(GEYSER,       "geyser",          13, MCF_HOSTILE|MCF_SIGHT),
MONSPELL(AGGRAVATION,  "aggravation",     13, MCF_INDIRECT|MCF_HOSTILE|MCF_SIGHT),
MONSPELL(TELEPORT,     "teleport",        15, MCF_INDIRECT),
MONSPELL(SUMMON_MONS,  "summon nasties",  15, MCF_HOSTILE|MCF_INDIRECT|MCF_SIGHT),
MONSPELL(GRAVITY,      "gravity wave",    15, MCF_HOSTILE|MCF_INDIRECT|MCF_SIGHT),
MONSPELL(CLONE_WIZ,    "simulacrum",      18, MCF_HOSTILE|MCF_INDIRECT|MCF_SIGHT),
MONSPELL(BLOOD_BIND,   "blood bind",      27, MCF_HOSTILE|MCF_INDIRECT|MCF_SIGHT),
MONSPELL(DEATH_TOUCH,  "a forbidden spell", 20, MCF_HOSTILE|MCF_SIGHT),

#undef MONSPELL
