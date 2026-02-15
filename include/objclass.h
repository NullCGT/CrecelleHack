/* NetHack 3.7	objclass.h	$NHDT-Date: 1596498553 2020/08/03 23:49:13 $  $NHDT-Branch: NetHack-3.7 $:$NHDT-Revision: 1.22 $ */
/* Copyright (c) Stichting Mathematisch Centrum, Amsterdam, 1985. */
/*-Copyright (c) Pasi Kallinen, 2018. */
/* NetHack may be freely redistributed.  See license for details. */

#ifndef OBJCLASS_H
#define OBJCLASS_H

/* [misnamed] definition of a type of object; many objects are composites
   (liquid potion inside glass bottle, metal arrowhead on wooden shaft)
   and object definitions only specify one type on a best-fit basis */

struct material {
    const char *name;
    /* Relative weights of different materials.
    * This used to be an attempt at making them super realistic, with densities in
    * terms of their kg/m^3 and as close to real life as possible, but that just
    * doesn't work because it makes materials infeasible to use. Nobody wants
    * anything gold or platinum if it weighs three times as much as its iron
    * counterpart, and things such as wooden plate mails were incredibly
    * overpowered by weighing about one-tenth as much as the iron counterpart.
    * Instead, use arbitrary units. */
    char ac;
    /* Relative weights of different materials.
    * This used to be an attempt at making them super realistic, with densities in
    * terms of their kg/m^3 and as close to real life as possible, but that just
    * doesn't work because it makes materials infeasible to use. Nobody wants
    * anything gold or platinum if it weighs three times as much as its iron
    * counterpart, and things such as wooden plate mails were incredibly
    * overpowered by weighing about one-tenth as much as the iron counterpart.
    * Instead, use arbitrary units. */
    int density;
    /* Relative prices for the different materials.
    * Units for this are much more poorly defined than for weights; the best
    * approximation would be something like "zorkmids per aum".
    * We only care about the ratio of two of these together. */
    int cost;
    uchar clr;
};

#define OBJ_MATERIAL_LIST OMAT(NO_MATERIAL, "mysterious", 0, 0, 0, CLR_BLACK), \
    OMAT(LIQUID,        "liquid",   0,  10,   1,   HI_ORGANIC), \
    OMAT(WAX,           "wax",      1,  15,   1,   CLR_WHITE), \
    OMAT(VEGGY,         "organic",  1,  10,   1,   HI_ORGANIC), \
    OMAT(FLESH,         "flesh",    3,  10,   3,   CLR_RED), \
    OMAT(PAPER,         "paper",    1,  5,    2,   CLR_WHITE), \
    OMAT(CLOTH,         "cloth",    2,  10,   3,   HI_CLOTH), \
    OMAT(LEATHER,       "leather",  3,  15,   5,   HI_LEATHER), \
    OMAT(WOOD,          "wood",     4,  30,   8,   HI_WOOD), \
    OMAT(BLEAKWOOD,     "bleakwood", \
                                    5,  15,   15,  HI_WOOD), \
    OMAT(BONE,          "bone",     5,  25,   20,  CLR_WHITE), \
    OMAT(DRAGON_HIDE,   "dragonhide", \
                                    10, 20,   200, CLR_BLACK), \
    OMAT(IRON,          "iron",     5,  80,   10,  HI_METAL), \
    OMAT(METAL,         "steel",    5,  75,   15,  HI_METAL), \
    OMAT(COPPER,        "copper",   4,  85,   18,  HI_COPPER), \
    OMAT(SILVER,        "silver",   5,  90,   30,  HI_SILVER), \
    OMAT(GOLD,          "gold",     3,  120,  60,  HI_GOLD), \
    OMAT(PLATINUM,      "platinum", 4,  120,  80,  CLR_WHITE), \
    OMAT(NIGHTIRON,     "nightiron", \
                                    5,  75,   95,  CLR_BLACK), \
    OMAT(MITHRIL,       "mithril",  6,  30,   50,  HI_SILVER), \
    OMAT(PLASTIC,       "plastic",  3,  20,   10,  CLR_WHITE), \
    OMAT(GLASS,         "glass",    5,  60,   20,  HI_GLASS), \
    OMAT(ICECRYSTAL,    "ice",      5,  60,   3,   HI_GLASS), \
    OMAT(GEMSTONE,      "gemstone", 7,  55,   500, CLR_RED), \
    OMAT(MINERAL,       "stone",    6,  70,   10,  CLR_GRAY), \
    OMAT(SALT,          "salt",     2,  20,   1,   CLR_WHITE), \
    OMAT(LODEN,         "lodenstone", \
                                    8,  3500, 1, CLR_GRAY)
#define OMAT(id, nam, ac, dens, cost, clr) id
enum obj_material_types {
    OBJ_MATERIAL_LIST,
    NUM_MATERIAL_TYPES
};
#undef OMAT
#define MAT_DENS(mat) materials[mat].density
#define MAT_COST(mat) materials[mat].cost
#define MAT_NAME(mat) materials[mat].name

enum obj_armor_types {
    ARM_SUIT   = 0,
    ARM_SHIELD = 1,        /* needed for special wear function */
    ARM_HELM   = 2,
    ARM_GLOVES = 3,
    ARM_BOOTS  = 4,
    ARM_CLOAK  = 5,
    ARM_SHIRT  = 6
};

#define OPROP_LIST OPROP(NONE, "buggy", 0, 0), \
OPROP(SANGUINE,  "sanguine",    50,     1), \
OPROP(BOREAL,    "boreal",      150,    2), \
OPROP(BLAZING,   "blazing",     150,    3), \
OPROP(CRACKLING, "crackling",   150,    4), \
OPROP(SUBTLE,    "subtle",      100,    5), \
OPROP(HEXED,     "hexed",       100,    6), \
OPROP(ACIDIC,    "acidic",      100,    7), \
OPROP(HUNGRY,    "hungry",      100,    8), \
OPROP(ANTIMAGIC, "nulling",      50,    9), \
OPROP(BRINY,     "briny",       100,    10)
#define OPROP(id, nam, prob, val) OPROP_##id = val
enum oprops_enum {OPROP_LIST, NUM_OPROPS};
#undef OPROP

struct objclass {
    short oc_name_idx;              /* index of actual name */
    short oc_descr_idx;             /* description when name unknown */
    char *oc_uname;                 /* called by user */
    Bitfield(oc_name_known, 1);     /* discovered */
    Bitfield(oc_merge, 1);          /* merge otherwise equal objects */
    Bitfield(oc_uses_known, 1);     /* obj->known affects full description;
                                     * otherwise, obj->dknown and obj->bknown
                                     * tell all, and obj->known should always
                                     * be set for proper merging behavior. */
    Bitfield(oc_encountered, 1);    /* hero has observed such an item at least
                                       once (perhaps without naming it) */
    Bitfield(oc_magic, 1);          /* inherently magical object */
    Bitfield(oc_charged, 1);        /* may have +n or (n) charges */
    Bitfield(oc_unique, 1);         /* special one-of-a-kind object */
    Bitfield(oc_nowish, 1);         /* cannot wish for this object */

    Bitfield(oc_big, 1);
#define oc_bimanual oc_big /* for weapons & tools used as weapons */
#define oc_bulky oc_big    /* for armor */
    Bitfield(oc_tough, 1); /* hard gems/rings */
    Bitfield(oc_finesse, 1); /* uses dex bonus */

    Bitfield(oc_spare1, 5);         /* padding to align oc_dir + oc_material;
                                     * can be cannibalized for other use;
                                     * aka 6 free bits */

    Bitfield(oc_dir, 3);
    /* oc_dir: zap style for wands and spells */
#define NODIR     1 /* non-directional */
#define IMMEDIATE 2 /* directional beam that doesn't ricochet */
#define RAY       3 /* beam that does bounce off walls */
    /* overloaded oc_dir: strike mode bit mask for weapons and weptools */
#define PIERCE    1 /* pointed weapon punctures target */
#define SLASH     2 /* sharp weapon cuts target */
#define WHACK     4 /* blunt weapon bashes target */
    Bitfield(oc_material, 5); /* one of obj_material_types */

    schar oc_subtyp;
#define oc_skill oc_subtyp  /* Skills of weapons, spellbooks, tools, gems */
#define oc_armcat oc_subtyp /* for armor (enum obj_armor_types) */

    uchar oc_oprop; /* property (invis, &c.) conveyed */
    char  oc_class; /* object class (enum obj_class_types) */
    schar oc_delay; /* delay when using such an object */
    uchar oc_color; /* color of the object */

    short oc_prob;            /* probability, used in mkobj() */
    unsigned short oc_weight; /* encumbrance (1 cn = 0.1 lb.) */
    short oc_cost;            /* base cost in shops */
    /* We no longer use the AD&D rules! :) */
    /* for weapons, and tools, rocks, and gems useful as weapons */
    schar oc_wndam, oc_wddam; /* max small/large monster damage */
    schar oc_oc1, oc_oc2;
#define oc_hitbon oc_oc1 /* weapons: "to hit" bonus */
#define oc_scaling oc_oc2 /* weapons: scaling stat */

#define a_ac oc_oc1     /* armor class, used in ARM_BONUS in do.c */
#define a_can oc_oc2    /* armor: used in mhitu.c */
#define oc_level oc_oc2 /* books: spell level */

    unsigned short oc_nutrition; /* food value */
};

struct class_sym {
    char sym;
    const char *name;
    const char *explain;
};

struct objdescr {
    const char *oc_name;  /* actual name */
    const char *oc_descr; /* description when name unknown */
};

/*
 * All objects have a class. Make sure that all classes have a corresponding
 * symbol below.
 */

enum objclass_defchars {
#define OBJCLASS_DEFCHAR_ENUM
#include "defsym.h"
#undef OBJCLASS_DEFCHAR_ENUM
};

enum objclass_classes {
    RANDOM_CLASS =  0, /* used for generating random objects */
#define OBJCLASS_CLASS_ENUM
#include "defsym.h"
#undef OBJCLASS_CLASS_ENUM
    MAXOCLASSES
};

/* Default characters for object classes */
enum objclass_syms {
#define OBJCLASS_S_ENUM
#include "defsym.h"
#undef OBJCLASS_S_ENUM
};

/* for mkobj() use ONLY! odd '-SPBOOK_CLASS' is in case of unsigned enums */
#define SPBOOK_no_NOVEL (0 - (int) SPBOOK_CLASS)

#define BURNING_OIL (MAXOCLASSES + 1) /* Can be used as input to explode    */
#define MON_EXPLODE (MAXOCLASSES + 2) /* Exploding monster (e.g. gas spore) */
#define TRAP_EXPLODE (MAXOCLASSES + 3)

#if 0 /* moved to decl.h so that makedefs.c won't see them */
extern const struct class_sym
        def_oc_syms[MAXOCLASSES];       /* default class symbols */
extern uchar oc_syms[MAXOCLASSES];      /* current class symbols */
#endif

struct fruit {
    char fname[PL_FSIZ];
    int fid;
    struct fruit *nextf;
};
#define newfruit() (struct fruit *) alloc(sizeof(struct fruit))
#define dealloc_fruit(rind) free((genericptr_t)(rind))

enum objects_nums {
#define OBJECTS_ENUM
#include "objects.h"
#undef OBJECTS_ENUM
    NUM_OBJECTS
};

enum misc_object_nums {
    NUM_REAL_GEMS  = (LAST_REAL_GEM - FIRST_REAL_GEM + 1),
    NUM_GLASS_GEMS = (LAST_GLASS_GEM - FIRST_GLASS_GEM + 1),
    /* LAST_SPELL is SPE_BLANK_PAPER, guaranteeing that spl_book[] will
       have at least one unused slot at end to be used as a terminator */
    MAXSPELL       = (LAST_SPELL - FIRST_SPELL + 1),
};

extern NEARDATA struct objclass objects[NUM_OBJECTS + 1];
extern NEARDATA struct objdescr obj_descr[NUM_OBJECTS + 1];
extern NEARDATA struct material materials[NUM_MATERIAL_TYPES];

#define OBJ_NAME(obj) (obj_descr[(obj).oc_name_idx].oc_name)
#define OBJ_DESCR(obj) (obj_descr[(obj).oc_descr_idx].oc_descr)

#define is_organic(otmp) (otmp->material <= WOOD)
#define is_dragonhide(otmp) (otmp->material == DRAGON_HIDE)
#define is_mithril(otmp) (otmp->material == MITHRIL)
#define is_iron(otmp) (otmp->material == IRON || otmp->material == NIGHTIRON)
#define is_glass(otmp) (otmp->material == GLASS)
#define is_wood(otmp) (otmp->material == WOOD)
#define is_bone(otmp) (otmp->material == BONE)
#define is_stone(otmp) (otmp->material == MINERAL || otmp->material == LODEN)
#define is_metallic(otmp) \
    (otmp->material >= IRON && otmp->material <= MITHRIL)
#define is_heavy_metallic(otmp) \
    (otmp->material >= IRON && otmp->material <= PLATINUM)

/* primary damage: fire/rust/--- */
/* is_flammable(otmp), is_rottable(otmp) in mkobj.c */
#define is_rustprone(otmp) (otmp->material == IRON || otmp->material == NIGHTIRON)
#define is_crackable(otmp) \
    ((otmp->material == GLASS || otmp->material == ICECRYSTAL) \
     && ((otmp)->oclass == ARMOR_CLASS || (otmp)->oclass == TOOL_CLASS)) /* erosion_matters() */
/* secondary damage: rot/acid/acid */
#define is_corrodeable(otmp) \
    (otmp->material == COPPER          \
     || otmp->material == IRON          \
     || otmp->material == NIGHTIRON     \
     || otmp->material == SALT)
/* subject to any damage */
#define is_damageable(otmp) \
    (is_rustprone(otmp) || is_flammable(otmp)           \
     || is_rottable(otmp) || is_corrodeable(otmp)       \
     || is_crackable(otmp))
/* has a fuzzed weight */
#define is_fuzzy_weight(otmp) \
    ((otmp->oclass == WEAPON_CLASS || otmp->oclass == ARMOR_CLASS \
        || is_weptool(otmp)) && !(objects[otmp->otyp].oc_merge || otmp->nomerge || is_ammo(otmp)))
/* is necessary to win the game */
#define is_ascension_obj(otmp) \
    (otmp->otyp == AMULET_OF_YENDOR \
        || otmp->otyp == CANDELABRUM_OF_INVOCATION \
        || otmp->otyp == BELL_OF_OPENING \
        || otmp->otyp == SPE_BOOK_OF_THE_DEAD)

#endif /* OBJCLASS_H */
