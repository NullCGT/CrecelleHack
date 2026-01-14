/* NetHack 3.7	monattk.h	$NHDT-Date: 1596498548 2020/08/03 23:49:08 $  $NHDT-Branch: NetHack-3.7 $:$NHDT-Revision: 1.13 $ */
/* NetHack may be freely redistributed.  See license for details. */
/* Copyright 1988, M. Stephenson */

#ifndef MONATTK_H
#define MONATTK_H

/*      Add new attack types below - ordering affects experience (exper.c).
 *      Attacks > AT_BUTT are worth extra experience.
 */

#define MATK_LIST MATK(AT_NONE, "passive"), \
    MATK(AT_CLAW, "claw"), \
    MATK(AT_BITE, "bite"), \
    MATK(AT_KICK, "kick"), \
    MATK(AT_BUTT, "butt"), \
    MATK(AT_TUCH, "touch"), \
    MATK(AT_STNG, "sting"), \
    MATK(AT_HUGS, "hug"), \
    MATK(AT_SPIT, "spit"), \
    MATK(AT_ENGL, "engulf"), \
    MATK(AT_BREA, "breath"), \
    MATK(AT_EXPL, "explode"), \
    MATK(AT_BOOM, "death throes"), \
    MATK(AT_GAZE, "gaze"), \
    MATK(AT_TENT, "tentacle"), \
    MATK(AT_WEAP, "weapon"), \
    MATK(AT_MAGC, "spell"),
#define MATK(id, nam) id
enum mattk_ids { AT_ANY = -1, MATK_LIST };
#undef MATK
#define MATK(id, nam) nam
static const char *const mattk_names[] = { MATK_LIST };

#define DISTANCE_ATTK_TYPE(atyp) ((atyp) == AT_SPIT \
                                  || (atyp) == AT_BREA \
                                  || (atyp) == AT_MAGC \
                                  || (atyp) == AT_GAZE)

/*      Add new damage types below.
 *
 *      Note that 1-10 correspond to the types of attack used in buzz().
 *      Please don't disturb the order unless you rewrite the buzz() code.
 */
#define MAD_LIST MAD(AD_PHYS, "physical"), \
    MAD(AD_MAGM, "magic missiles"), \
    MAD(AD_FIRE, "fire"), \
    MAD(AD_COLD, "cold"), \
    MAD(AD_SLEE, "sleep"), \
    MAD(AD_DISN, "disintegration"), \
    MAD(AD_ELEC, "shock"), \
    MAD(AD_DRST, "strength poison"), \
    MAD(AD_ACID, "acid"), \
    MAD(AD_SPC1, ""), \
    MAD(AD_SPC2, ""), \
    MAD(AD_BLND, "blind"), \
    MAD(AD_STUN, "stun"), \
    MAD(AD_SLOW, "slow"), \
    MAD(AD_PLYS, "paralyze"), \
    MAD(AD_DRLI, "drain life"), \
    MAD(AD_DREN, "drain energy"), \
    MAD(AD_LEGS, "wound legs"), \
    MAD(AD_STON, "petrify"), \
    MAD(AD_STCK, "sticky"), \
    MAD(AD_SGLD, "pickpocket"), \
    MAD(AD_SITM, "theft"), \
    MAD(AD_SEDU, "seduce"), \
    MAD(AD_TLPT, "teleport"), \
    MAD(AD_RUST, "rust"), \
    MAD(AD_CONF, "confuse"), \
    MAD(AD_DGST, "digest"), \
    MAD(AD_HEAL, "heal"), \
    MAD(AD_WRAP, "wrap"), \
    MAD(AD_WERE, "lycanthropy"), \
    MAD(AD_DRDX, "dexterity poison"), \
    MAD(AD_DRCO, "constitution poison"), \
    MAD(AD_DRIN, "eat brains"), \
    MAD(AD_DISE, "disease"), \
    MAD(AD_DCAY, "decay"), \
    MAD(AD_SSEX, "flirtation"), \
    MAD(AD_HALU, "hallucination"), \
    MAD(AD_DETH, "death"), \
    MAD(AD_PEST, "pestilence"), \
    MAD(AD_FAMN, "famine"), \
    MAD(AD_SLIM, "slime"), \
    MAD(AD_ENCH, "disenchant"), \
    MAD(AD_TMUT, "transmute"), \
    MAD(AD_CORR, "corrode"), \
    MAD(AD_POLY, "polymorph"), \
    MAD(AD_WORM, "spam mail"), \
    MAD(AD_HONY, "spread honey"), \
    MAD(AD_SOAK, "soak"), \
    MAD(AD_TLAW, "teleport away"), \
    MAD(AD_CLRC, "clerical"), \
    MAD(AD_SPEL, "arcane"), \
    MAD(AD_RBRE, "random"), \
    MAD(AD_SAMU, "steal amulet"), \
    MAD(AD_CURS, "curse"),
#define MAD(id, nam) id
enum mad_ids { AD_ANY = -1, MAD_LIST };
#undef MAD
#define MAD(id, nam) nam
static const char *const mad_names[] = { MAD_LIST };

struct mhitm_data {
    int damage;
    int hitflags; /* M_ATTK_DEF_DIED | M_ATTK_AGR_DIED | ... */
    boolean done;
    boolean permdmg;
    int specialdmg;
    int dieroll;
};

/*
 *  Monster-to-monster attacks.  When a monster attacks another (mattackm),
 *  any or all of the following can be returned.  See mattackm() for more
 *  details.
 */
#define M_ATTK_MISS 0x0     /* aggressor missed */
#define M_ATTK_HIT 0x1      /* aggressor hit defender */
#define M_ATTK_DEF_DIED 0x2 /* defender died */
#define M_ATTK_AGR_DIED 0x4 /* aggressor died */
#define M_ATTK_AGR_DONE 0x8 /* aggressor is done with their turn */

#endif /* MONATTK_H */
