/* NetHack 3.7	mcastu.c	$NHDT-Date: 1726168598 2024/09/12 19:16:38 $  $NHDT-Branch: NetHack-3.7 $:$NHDT-Revision: 1.105 $ */
/* Copyright (c) Stichting Mathematisch Centrum, Amsterdam, 1985. */
/*-Copyright (c) Robert Patrick Rankin, 2011. */
/* NetHack may be freely redistributed.  See license for details. */

#include "hack.h"

#define MAX_MON_SPELLS 12
#define MSPEL_LIST MSPEL("psi bolt", 1, PSI_BOLT), \
    MSPEL("open wounds", 1, OPEN_WOUNDS), \
    MSPEL("disguise self", 1, DISGUISE), \
    MSPEL("cure self", 2, CURE_SELF), \
    MSPEL("darkness", 2, DARKNESS), \
    MSPEL("grease", 2, GREASE), \
    MSPEL("blood rain", 2, BLOOD_RAIN), \
    MSPEL("confusion", 3, CONFUSE_YOU), \
    MSPEL("mirror image", 3, MIRROR_IMAGE), \
    MSPEL("haste self", 3, HASTE_SELF), \
    MSPEL("bloodrush", 4, BLOODRUSH), \
    MSPEL("stunning force", 4, STUN_YOU), \
    MSPEL("sleepel", 4, SLEEP_YOU), \
    MSPEL("hold", 5, PARALYZE), \
    MSPEL("invisibility", 5, DISAPPEAR), \
    MSPEL("blind", 6, BLIND_YOU), \
    MSPEL("strength of newt", 7, WEAKEN_YOU), \
    MSPEL("vulnerability", 8, VULNERABILITY), \
    MSPEL("summon vermin", 9, INSECTS), \
    MSPEL("destroy armor", 9, DESTRY_ARMR), \
    MSPEL("bloody pierce", 9, BLOOD_SPEAR), \
    MSPEL("raise dead", 10, RAISE_DEAD), \
    MSPEL("levitation", 10, LEVITATE_YOU), \
    MSPEL("curse", 10, CURSE_ITEMS), \
    MSPEL("force field", 10, FORCE_FIELD), \
    MSPEL("chaos rain", 10, CHAOS_RAIN), \
    MSPEL("lightning bolt", 12, LIGHTNING), \
    MSPEL("divine wrath", 13, FIRE_PILLAR), \
    MSPEL("aggravate monsters", 14, AGGRAVATION), \
    MSPEL("teleport", 16, TELEPORT), \
    MSPEL("summon nasties", 16, SUMMON_MONS), \
    MSPEL("geyser", 14, GEYSER), \
    MSPEL("gravity wave", 15, GRAVITY), \
    MSPEL("simulacrum", 19, CLONE_WIZ), \
    MSPEL("blood bind", 27, BLOOD_BIND), \
    MSPEL("a forbidden spell", 21, DEATH_TOUCH), 
#define MSPEL(nam, lev, id) MCU_##id
enum mcastu_spells { MSPEL_LIST };
#undef MSPEL
#define MSPEL(nam, lev, id) { nam, lev }
struct mspel {
    const char *name;
    unsigned lev;
};
struct mspel mon_all_spells[] = { MSPEL_LIST };
#undef MSPEL
#undef MSPEL_LIST

DISABLE_WARNING_FORMAT_NONLITERAL

int mon_mage_spells[MAX_MON_SPELLS] = { MCU_PSI_BOLT, MCU_CURE_SELF, MCU_HASTE_SELF,
                                        MCU_STUN_YOU, MCU_DISAPPEAR, MCU_WEAKEN_YOU,
                                        MCU_DESTRY_ARMR, MCU_CURSE_ITEMS, MCU_AGGRAVATION,
                                        MCU_SUMMON_MONS, MCU_CLONE_WIZ, MCU_DEATH_TOUCH};
int mon_shadow_mage_spells[MAX_MON_SPELLS] = { MCU_PSI_BOLT, MCU_DARKNESS, MCU_HASTE_SELF,
                                        MCU_STUN_YOU, MCU_DISAPPEAR, MCU_WEAKEN_YOU,
                                        MCU_DESTRY_ARMR, MCU_CURSE_ITEMS, MCU_SLEEP_YOU,
                                        MCU_SUMMON_MONS, MCU_MIRROR_IMAGE, MCU_DEATH_TOUCH};
int mon_vamp_spells[MAX_MON_SPELLS] = { MCU_OPEN_WOUNDS, MCU_CURE_SELF, MCU_BLOODRUSH,
                                        MCU_DISAPPEAR, MCU_CURSE_ITEMS, MCU_BLOOD_SPEAR,
                                        MCU_BLOOD_RAIN, MCU_BLOOD_BIND, -1, -1, -1, -1};
int mon_cleric_spells[MAX_MON_SPELLS] = { MCU_OPEN_WOUNDS, MCU_CURE_SELF, MCU_CONFUSE_YOU,
                                          MCU_PARALYZE, MCU_BLIND_YOU, MCU_INSECTS,
                                          MCU_CURSE_ITEMS, MCU_LIGHTNING, MCU_FIRE_PILLAR,
                                          MCU_GEYSER, -1, -1 };
int mon_law_cleric_spells[MAX_MON_SPELLS] = { MCU_OPEN_WOUNDS, MCU_CURE_SELF, MCU_CONFUSE_YOU,
                                          MCU_PARALYZE, MCU_BLIND_YOU, MCU_FORCE_FIELD,
                                          MCU_CURSE_ITEMS, MCU_LIGHTNING, MCU_FIRE_PILLAR,
                                          MCU_GEYSER, -1, -1 };
int mon_chaos_cleric_spells[MAX_MON_SPELLS] = { MCU_OPEN_WOUNDS, MCU_CURE_SELF, MCU_CONFUSE_YOU,
                                          MCU_PARALYZE, MCU_BLIND_YOU, MCU_CHAOS_RAIN,
                                          MCU_CURSE_ITEMS, MCU_LIGHTNING, MCU_FIRE_PILLAR,
                                          MCU_GEYSER, -1, -1 };
int mon_undead_spells[MAX_MON_SPELLS] = { MCU_HASTE_SELF, MCU_STUN_YOU, MCU_WEAKEN_YOU,
                                          MCU_SLEEP_YOU, MCU_VULNERABILITY,
                                          MCU_CURSE_ITEMS, MCU_AGGRAVATION, MCU_RAISE_DEAD,
                                          MCU_DEATH_TOUCH, MCU_MIRROR_IMAGE, MCU_DISAPPEAR,
                                          MCU_TELEPORT };
int mon_demo_spells[MAX_MON_SPELLS] = { MCU_PSI_BOLT, MCU_OPEN_WOUNDS, MCU_CURE_SELF, 
                                        MCU_HASTE_SELF, MCU_STUN_YOU, MCU_WEAKEN_YOU,
                                        MCU_DESTRY_ARMR, MCU_CURSE_ITEMS, MCU_AGGRAVATION,
                                        MCU_SUMMON_MONS, MCU_CHAOS_RAIN, MCU_DEATH_TOUCH};
int mon_trickster_spells[MAX_MON_SPELLS] = { MCU_PSI_BOLT, MCU_HASTE_SELF, MCU_DISAPPEAR,
                                             MCU_LEVITATE_YOU,
                                             MCU_AGGRAVATION, MCU_MIRROR_IMAGE, MCU_CONFUSE_YOU,
                                             MCU_GREASE, MCU_DISGUISE, MCU_CURSE_ITEMS, MCU_SUMMON_MONS, 
                                             MCU_TELEPORT };

staticfn void cursetxt(struct monst *, boolean);
staticfn int choose_magic_spell(int);
staticfn int choose_clerical_spell(int);
staticfn int choose_monster_spell(struct monst *, int);
staticfn int m_cure_self(struct monst *, int);
staticfn void cast_monster_spell(struct monst *, int, int);
staticfn boolean is_undirected_spell(int);
staticfn boolean spell_would_be_useless(struct monst *, int);
staticfn int spawn_mirror_image(struct monst *, int, int);

/* feedback when frustrated monster couldn't cast a spell */
staticfn void
cursetxt(struct monst *mtmp, boolean undirected)
{
    if (canseemon(mtmp) && couldsee(mtmp->mx, mtmp->my)) {
        const char *pointer_msg; /* how do they point? */
        const char *point_msg; /* spellcasting monsters are impolite */

        if (nohands(mtmp->data)) {
            if (haseyes(mtmp->data)) {
                pointer_msg = "looks";
            } else {
                pointer_msg = "wiggles";
            }
        } else {
            pointer_msg = "points";
        }

        if (undirected)
            point_msg = "all around, then curses";
        else if ((Invis && !perceives(mtmp->data)
                  && (mtmp->mux != u.ux || mtmp->muy != u.uy))
                 || is_obj_mappear(&gy.youmonst, STRANGE_OBJECT)
                 || u.uundetected)
            point_msg = "and curses in your general direction";
        else if (Displaced && (mtmp->mux != u.ux || mtmp->muy != u.uy))
            point_msg = "and curses at your displaced image";
        else
            point_msg = "at you, then curses";

        pline_mon(mtmp, "%s %s %s.", Monnam(mtmp), pointer_msg, point_msg);
    } else if ((!(svm.moves % 4) || !rn2(4))) {
        if (!Deaf)
            Norep("You hear a mumbled curse.");   /* Deaf-aware */
    }
}


staticfn int
choose_monster_spell(struct monst *mtmp, int adtyp) {
    int n;
    int spellval;
    int spell;
    int a;

    a = mon_aligntyp(mtmp);
    if (a == A_NONE)
        a = aligns[(mtmp->m_id % 3)].value;
    do {
        n = rn2(MAX_MON_SPELLS);
        if (mtmp->data->mlet == S_VAMPIRE
            || mtmp->data == &mons[PM_BLOOD_IMP])
            spell = mon_vamp_spells[n];
        else if (mtmp->data == &mons[PM_DEMOGORGON])
            spell = mon_demo_spells[n];
        else if (mtmp->data == &mons[PM_DARK_ONE])
            spell = mon_shadow_mage_spells[n];
        else if (is_undead(mtmp->data) || mtmp->data == &mons[PM_ORCUS])
            spell = mon_undead_spells[n];
        else if (mtmp->data->mlet == S_GNOME || mtmp->data->mlet == S_KOBOLD
                 || mtmp->data == &mons[PM_DISPATER])
            spell = mon_trickster_spells[n];
        else if (adtyp == AD_CLRC) {
            if (a == A_LAWFUL)
                spell = mon_law_cleric_spells[n];
            else if (a == A_CHAOTIC)
                spell = mon_chaos_cleric_spells[n];
            else if (a == A_NONE) {

            } else
                spell = mon_cleric_spells[n];
        } else
            spell = mon_mage_spells[n];
        spellval = mon_all_spells[spell].lev;
    } while (spell == -1 || spellval > mtmp->m_lev );
    return spell;
}
/* convert a level-based random selection into a specific mage spell;
   inappropriate choices will be screened out by spell_would_be_useless() */
staticfn int
choose_magic_spell(int spellval)
{
    /* for 3.4.3 and earlier, val greater than 22 selected default spell */
    while (spellval > 24 && rn2(25))
        spellval = rn2(spellval);

    switch (spellval) {
    case 24:
    case 23:
        if (Antimagic || Hallucination)
            return MCU_PSI_BOLT;
        FALLTHROUGH;
        /*FALLTHRU*/
    case 22:
    case 21:
    case 20:
        return MCU_DEATH_TOUCH;
    case 19:
    case 18:
        return MCU_CLONE_WIZ;
    case 17:
    case 16:
    case 15:
        return MCU_SUMMON_MONS;
    case 14:
    case 13:
        return MCU_AGGRAVATION;
    case 12:
    case 11:
    case 10:
        return MCU_CURSE_ITEMS;
    case 9:
    case 8:
        return MCU_DESTRY_ARMR;
    case 7:
    case 6:
        return MCU_WEAKEN_YOU;
    case 5:
    case 4:
        return MCU_DISAPPEAR;
    case 3:
        return MCU_STUN_YOU;
    case 2:
        return MCU_HASTE_SELF;
    case 1:
        return MCU_CURE_SELF;
    case 0:
    default:
        return MCU_PSI_BOLT;
    }
}

/* convert a level-based random selection into a specific cleric spell */
staticfn int
choose_clerical_spell(int spellnum)
{
    /* for 3.4.3 and earlier, num greater than 13 selected the default spell
     */
    while (spellnum > 15 && rn2(16))
        spellnum = rn2(spellnum);

    switch (spellnum) {
    case 15:
    case 14:
        if (rn2(3))
            return MCU_OPEN_WOUNDS;
        FALLTHROUGH;
        /*FALLTHRU*/
    case 13:
        return MCU_GEYSER;
    case 12:
        return MCU_FIRE_PILLAR;
    case 11:
        return MCU_LIGHTNING;
    case 10:
    case 9:
        return MCU_CURSE_ITEMS;
    case 8:
        return MCU_INSECTS;
    case 7:
    case 6:
        return MCU_BLIND_YOU;
    case 5:
    case 4:
        return MCU_PARALYZE;
    case 3:
    case 2:
        return MCU_CONFUSE_YOU;
    case 1:
        return MCU_CURE_SELF;
    case 0:
    default:
        return MCU_OPEN_WOUNDS;
    }
}

/* return values:
 * 1: successful spell
 * 0: unsuccessful spell
 */
int
castmu(
    struct monst *mtmp,   /* caster */
    struct attack *mattk, /* caster's current attack */
    boolean thinks_it_foundyou,    /* might be mistaken if displaced */
    boolean foundyou)              /* knows hero's precise location */
{
    int dmg, ml = mtmp->m_lev;
    int ret;
    int spellnum = 0;

    /* Three cases:
     * -- monster is attacking you.  Search for a useful spell.
     * -- monster thinks it's attacking you.  Search for a useful spell,
     *    without checking for undirected.  If the spell found is directed,
     *    it fails with cursetxt() and loss of mspec_used.
     * -- monster isn't trying to attack.  Select a spell once.  Don't keep
     *    searching; if that spell is not useful (or if it's directed),
     *    return and do something else.
     * Since most spells are directed, this means that a monster that isn't
     * attacking casts spells only a small portion of the time that an
     * attacking monster does.
     */
    if ((mattk->adtyp == AD_SPEL || mattk->adtyp == AD_CLRC) && ml) {
        int cnt = 40;

        do {
            spellnum = choose_monster_spell(mtmp, mattk->adtyp);
            /* not trying to attack?  don't allow directed spells */
            if (!thinks_it_foundyou) {
                if (!is_undirected_spell(spellnum)
                    || spell_would_be_useless(mtmp, spellnum)) {
                    if (foundyou)
                        impossible(
                       "spellcasting monster found you and doesn't know it?");
                    return M_ATTK_MISS;
                }
                break;
            }
        } while (--cnt > 0
                 && spell_would_be_useless(mtmp, spellnum));
        if (cnt == 0)
            return M_ATTK_MISS;
    }

    /* monster unable to cast spells? */
    if (mtmp->mcan || mtmp->mspec_used || !ml
        || m_seenres(mtmp, cvt_adtyp_to_mseenres(mattk->adtyp))) {
        cursetxt(mtmp, is_undirected_spell(spellnum));
        return M_ATTK_MISS;
    }

    if (mattk->adtyp == AD_SPEL || mattk->adtyp == AD_CLRC) {
        /* monst->m_lev is unsigned (uchar), monst->mspec_used is int */
        mtmp->mspec_used = (int) ((mtmp->m_lev < 8) ? (10 - mtmp->m_lev) : 2);
    }

    /* Monster can cast spells, but is casting a directed spell at the
     * wrong place?  If so, give a message, and return.
     * Do this *after* penalizing mspec_used.
     *
     * FIXME?
     *  Shouldn't wall of lava have a case similar to wall of water?
     *  And should cold damage hit water or lava instead of missing
     *  even when the caster has targeted the wrong spot?  Likewise
     *  for fire mis-aimed at ice.
     */
    if (!foundyou && thinks_it_foundyou
        && !is_undirected_spell(spellnum)) {
        pline_mon(mtmp, "%s casts %s at %s!",
                 ((Role_if(PM_WIZARD) && mattk->adtyp == AD_SPEL)
                    || (Role_if(PM_CLERIC) && mattk->adtyp == AD_CLRC))
                        ? mon_all_spells[spellnum].name : "a spell",
                 canseemon(mtmp) ? Monnam(mtmp) : "Something",
                 is_waterwall(mtmp->mux, mtmp->muy) ? "empty water"
                                                    : "thin air");
        return M_ATTK_MISS;
    }

    nomul(0);
    if (rn2(ml * 10) < (mtmp->mconf ? 100 : 20)) { /* fumbled attack */
        Soundeffect(se_air_crackles, 60);
        if (canseemon(mtmp) && !Deaf) {
            set_msg_xy(mtmp->mx, mtmp->my);
            pline_The("air crackles around %s.", mon_nam(mtmp));
        }
        return M_ATTK_MISS;
    }
    if (canspotmon(mtmp) || !is_undirected_spell(spellnum)) {
        pline_mon(mtmp, "%s casts %s%s!",
                 canspotmon(mtmp) ? Monnam(mtmp) : "Something",
                 ((Role_if(PM_WIZARD) && mattk->adtyp == AD_SPEL)
                    || (Role_if(PM_CLERIC) && mattk->adtyp == AD_CLRC))
                        ? mon_all_spells[spellnum].name : "a spell",
                 is_undirected_spell(spellnum) ? ""
                 : (Invis && !perceives(mtmp->data)
                    && !u_at(mtmp->mux, mtmp->muy))
                   ? " at a spot near you"
                   : (Displaced && !u_at(mtmp->mux, mtmp->muy))
                     ? " at your displaced image"
                     : " at you");
    }

    /*
     * As these are spells, the damage is related to the level
     * of the monster casting the spell.
     */
    if (!foundyou) {
        dmg = 0;
        if (mattk->adtyp != AD_SPEL && mattk->adtyp != AD_CLRC) {
            impossible(
              "%s casting non-hand-to-hand version of hand-to-hand spell %d?",
                       Monnam(mtmp), mattk->adtyp);
            return M_ATTK_MISS;
        }
    } else if (mattk->damd)
        dmg = d((int) ((ml / 2) + mattk->damn), (int) mattk->damd);
    else
        dmg = d((int) ((ml / 2) + 1), 6);
    if (Half_spell_damage)
        dmg = (dmg + 1) / 2;

    ret = M_ATTK_HIT;
    /*
     * FIXME: none of these hit the steed when hero is riding, nor do
     *  they inflict damage on carried items.
     */
    switch (mattk->adtyp) {
    case AD_FIRE:
        pline("You're enveloped in flames.");
        if (Fire_resistance) {
            shieldeff(u.ux, u.uy);
            pline("But you resist the effects.");
            monstseesu(M_SEEN_FIRE);
            dmg = 0;
        } else {
            monstunseesu(M_SEEN_FIRE);
        }
        burn_away_slime();
        /* burn up flammable items on the floor, melt ice terrain */
        mon_spell_hits_spot(mtmp, AD_FIRE, u.ux, u.uy);
        break;
    case AD_COLD:
        pline("You're covered in frost.");
        if (Cold_resistance) {
            shieldeff(u.ux, u.uy);
            pline("But you resist the effects.");
            monstseesu(M_SEEN_COLD);
            dmg = 0;
        } else {
            monstunseesu(M_SEEN_COLD);
        }
        /* freeze water or lava terrain */
        /* FIXME: mon_spell_hits_spot() uses zap_over_floor(); unlike with
         * fire, it does not target susceptible floor items with cold */
        mon_spell_hits_spot(mtmp, AD_COLD, u.ux, u.uy);
        break;
    case AD_MAGM:
        You("are hit by a shower of missiles!");
        if (Antimagic) {
            shieldeff(u.ux, u.uy);
            pline_The("missiles bounce off!");
            monstseesu(M_SEEN_MAGR);
            dmg = 0;
        } else {
            dmg = d((int) mtmp->m_lev / 2 + 1, 6);
            monstunseesu(M_SEEN_MAGR);
        }
        /* shower of magic missiles scuffs an engraving */
        mon_spell_hits_spot(mtmp, AD_MAGM, u.ux, u.uy);
        break;
    case AD_SPEL: /* wizard spell */
    case AD_CLRC: /* clerical spell */
        cast_monster_spell(mtmp, dmg, spellnum);
        dmg = 0; /* done by the spell casting functions */
        break;
    } /* switch */
    if (dmg)
        mdamageu(mtmp, dmg);
    return ret;
}

staticfn int
m_cure_self(struct monst *mtmp, int dmg)
{
    int heal_dice = max(3, 3 + mtmp->m_lev / 8);
    if (mtmp->mhp < mtmp->mhpmax) {
        if (canseemon(mtmp))
            pline_mon(mtmp, "%s looks better.", Monnam(mtmp));
        /* note: player healing does 6d4; this used to do 1d8 */
        healmon(mtmp, d(heal_dice, 6), 0);
        dmg = 0;
    }
    return dmg;
}

/* unlike the finger of death spell which behaves like a wand of death,
   this monster spell only attacks the hero */
void
touch_of_death(struct monst *mtmp)
{
    char kbuf[BUFSZ];
    int dmg = 50 + d(8, 6);
    int drain = dmg / 2;

    /* if we get here, we know that hero isn't magic resistant and isn't
       poly'd into an undead or demon */
    You_feel("drained...");
    (void) death_inflicted_by(kbuf, "the touch of death", mtmp);

    if (Upolyd) {
        u.mh = 0;
        rehumanize(); /* fatal iff Unchanging */
    } else if (drain >= u.uhpmax) {
        svk.killer.format = KILLED_BY;
        Strcpy(svk.killer.name, kbuf);
        done(DIED);
    } else {
        /* HP manipulation similar to poisoned(attrib.c) */
        int olduhp = u.uhp,
            uhpmin = minuhpmax(3),
            newuhpmax = u.uhpmax - drain;

        setuhpmax(max(newuhpmax, uhpmin), FALSE);
        dmg = adjuhploss(dmg, olduhp); /* reduce pending damage if uhp has
                                        * already been reduced due to drop
                                        * in uhpmax */
        losehp(dmg, kbuf, KILLED_BY);
    }
    svk.killer.name[0] = '\0'; /* not killed if we get here... */
}

/* give a reason for death by some monster spells */
char *
death_inflicted_by(
    char *outbuf,            /* assumed big enough; pm_names are short */
    const char *deathreason, /* cause of death */
    struct monst *mtmp)      /* monster who caused it */
{
    Strcpy(outbuf, deathreason);
    if (mtmp) {
        struct permonst *mptr = mtmp->data,
            *champtr = (ismnum(mtmp->cham)) ? &mons[mtmp->cham] : mptr;
        const char *realnm = pmname(champtr, Mgender(mtmp)),
            *fakenm = pmname(mptr, Mgender(mtmp));

        /* greatly simplified extract from done_in_by(), primarily for
           reason for death due to 'touch of death' spell; if mtmp is
           shape changed, it won't be a vampshifter or mimic since they
           can't cast spells */
        if (!type_is_pname(champtr) && !the_unique_pm(mptr))
            realnm = an(realnm);
        Sprintf(eos(outbuf), " inflicted by %s%s",
                the_unique_pm(mptr) ? "the " : "", realnm);
        if (champtr != mptr)
            Sprintf(eos(outbuf), " imitating %s", an(fakenm));
    }
    return outbuf;
}

/*
 * Monster wizard and cleric spellcasting functions.
 */

/*
   If dmg is zero, then the monster is not casting at you.
   If the monster is intentionally not casting at you, we have previously
   called spell_would_be_useless() and spellnum should always be a valid
   undirected spell.
   If you modify either of these, be sure to change is_undirected_spell()
   and spell_would_be_useless().
 */
staticfn void
cast_monster_spell(struct monst *mtmp, int dmg, int spellnum)
{
    coord mm;
    int orig_dmg = 0;
    if (dmg < 0) {
        impossible("monster cast monster spell (%d) with negative dmg (%d)?",
                   spellnum, dmg);
        return;
    }
    if (dmg == 0 && !is_undirected_spell(spellnum)) {
        impossible("cast directed monster spell (%d) with dmg=0?", spellnum);
        return;
    }

    switch (spellnum) {
    case MCU_DEATH_TOUCH:
        pline("Oh no, %s's using the touch of death!", mhe(mtmp));
        if (nonliving(gy.youmonst.data) || is_demon(gy.youmonst.data)) {
            You("seem no deader than before.");
        } else if (!Antimagic && rn2(mtmp->m_lev) > 12) {
            if (Hallucination) {
                You("have an out of body experience.");
            } else {
                touch_of_death(mtmp);
            }
            monstunseesu(M_SEEN_MAGR);
        } else {
            if (Antimagic) {
                shieldeff(u.ux, u.uy);
                monstseesu(M_SEEN_MAGR);
            }
            pline("Lucky for you, it didn't work!");
        }
        dmg = 0;
        break;
    case MCU_CLONE_WIZ:
        if (mtmp->iswiz && svc.context.no_of_wizards == 1) {
            pline("Double Trouble...");
            clonewiz();
            dmg = 0;
        } else
            impossible("bad wizard cloning?");
        break;
    case MCU_SUMMON_MONS: {
        int count = nasty(mtmp);

        if (!count) {
            ; /* nothing was created? */
        } else if (mtmp->iswiz) {
            SetVoice(mtmp, 0, 80, 0);
            verbalize("Destroy the thief, my pet%s!", plur(count));
        } else {
            boolean one = (count == 1);
            const char *mappear = one ? "A monster appears"
                                      : "Monsters appear";

            /* messages not quite right if plural monsters created but
               only a single monster is seen */
            if (Invis && !perceives(mtmp->data)
                && (mtmp->mux != u.ux || mtmp->muy != u.uy))
                pline("%s %s a spot near you!", mappear,
                      one ? "at" : "around");
            else if (Displaced && (mtmp->mux != u.ux || mtmp->muy != u.uy))
                pline("%s %s your displaced image!", mappear,
                      one ? "by" : "around");
            else
                pline("%s from nowhere!", mappear);
        }
        dmg = 0;
        break;
    }
    case MCU_MIRROR_IMAGE: {
        int quan = rnd(mtmp->m_lev < 10 ? 2 : 5);
        coord bypos;
        boolean created = FALSE;
        for (int i = 0; i < quan; i++) {
            if (!enexto(&bypos, mtmp->mx, mtmp->my, mtmp->data))
                break;
            created = spawn_mirror_image(mtmp, bypos.x, bypos.y);
        }
        if (mtmp->iswiz && created) {
            SetVoice(mtmp, 0, 80, 0);
            verbalize("Ah, but which of us is the real one, fool?");
        } else if (mtmp && canseemon(mtmp)) {
            pline_mon(mtmp, "%s image splinters!", s_suffix(Monnam(mtmp)));
        }
        dmg = 0;
        break;
    }
    case MCU_RAISE_DEAD:
        pline_The("dead speak!");
        mm.x = mtmp->mx;
        mm.y = mtmp->my;
        if (!rn2(3))
            (void) unturn_dead(&gy.youmonst);
        mkundead(&mm, TRUE, NO_MINVENT);
        dmg = 0;
        break;
    case MCU_AGGRAVATION:
        You_feel("that monsters are aware of your presence.");
        aggravate();
        dmg = 0;
        break;
    case MCU_DESTRY_ARMR:
        if (Antimagic) {
            shieldeff(u.ux, u.uy);
            monstseesu(M_SEEN_MAGR);
            pline("A field of force surrounds you!");
        } else if (!destroy_arm(some_armor(&gy.youmonst))) {
            Your("skin itches.");
        } else {
            /* monsters only realize you aren't magic-protected if armor is
               actually destroyed */
            monstunseesu(M_SEEN_MAGR);
        }
        dmg = 0;
        break;
    case MCU_WEAKEN_YOU: /* drain strength */
        if (Antimagic) {
            shieldeff(u.ux, u.uy);
            monstseesu(M_SEEN_MAGR);
            You_feel("momentarily weakened.");
        } else {
            char kbuf[BUFSZ];

            You("suddenly feel weaker!");
            dmg = mtmp->m_lev - 6;
            if (dmg < 1) /* paranoia since only chosen when m_lev is high */
                dmg = 1;
            if (Half_spell_damage)
                dmg = (dmg + 1) / 2;
            losestr(rnd(dmg),
                    death_inflicted_by(kbuf, "strength loss", mtmp),
                    KILLED_BY);
            svk.killer.name[0] = '\0'; /* not killed if we get here... */
            monstunseesu(M_SEEN_MAGR);
        }
        dmg = 0;
        break;
    case MCU_DISAPPEAR: /* makes self invisible */
        if (!mtmp->minvis && !mtmp->invis_blkd) {
            if (canseemon(mtmp))
                pline_mon(mtmp, "%s suddenly %s!", Monnam(mtmp),
                      !See_invisible ? "disappears" : "becomes transparent");
            mon_set_minvis(mtmp);
            if (cansee(mtmp->mx, mtmp->my) && !canspotmon(mtmp))
                map_invisible(mtmp->mx, mtmp->my);
            dmg = 0;
        } else
            impossible("no reason for monster to cast disappear spell?");
        break;
    case MCU_VULNERABILITY: { /* make player vulnerable to something */
        int vul = (mtmp->data->mlet == S_LICH) ? COLD_VUL : rn1(6, FIRE_VUL);
        You("feel more vulnerable!");
        switch (vul) {
        case COLD_VUL:
            incr_itimeout(&HCold_vulnerability, (long) dmg);
            monstunseesu(M_SEEN_COLD);
            break;
        case FIRE_VUL:
            incr_itimeout(&HFire_vulnerability, (long) dmg);
            monstunseesu(M_SEEN_FIRE);
            break;
        case SLEEP_VUL:
            incr_itimeout(&HSleep_vulnerability, (long) dmg);
            monstunseesu(M_SEEN_SLEEP);
            break;
        case DISINT_VUL:
            incr_itimeout(&HDisint_vulnerability, (long) dmg);
            monstunseesu(M_SEEN_DISINT);
            break;
        case SHOCK_VUL:
            incr_itimeout(&HShock_vulnerability, (long) dmg);
            monstunseesu(M_SEEN_ELEC);
            break;
        case POISON_VUL:
            incr_itimeout(&HPoison_vulnerability, (long) dmg);
            monstunseesu(M_SEEN_POISON);
            break;
        }
        dmg = 0;
        break;
    } case MCU_SLEEP_YOU:
        if (!Free_action && !Sleep_resistance) {
            You_feel("feel exhausted.");
            fall_asleep(-d(5, 5), TRUE);
            exercise(A_DEX, FALSE);
        } else {
            You("yawn.");
            monstseesu(M_SEEN_SLEEP);
        }
        dmg = 0;
        break;
    case MCU_STUN_YOU:
        if (Antimagic || Free_action) {
            shieldeff(u.ux, u.uy);
            monstseesu(M_SEEN_MAGR);
            if (!Stunned)
                You_feel("momentarily disoriented.");
            make_stunned(1L, FALSE);
        } else {
            You(Stunned ? "struggle to keep your balance." : "reel...");
            dmg = d(ACURR(A_DEX) < 12 ? 6 : 4, 4);
            if (Half_spell_damage)
                dmg = (dmg + 1) / 2;
            make_stunned((HStun & TIMEOUT) + (long) dmg, FALSE);
            monstunseesu(M_SEEN_MAGR);
        }
        dmg = 0;
        break;
    case MCU_BLOOD_BIND:
        if (canseemon(mtmp))
            urgent_pline("%s claps %s hands together:", Monnam(mtmp), mhis(mtmp));
        verbalize("Blood bind!");
        /* Goodbye. */
        for (int x = 0; x < COLNO; x++) {
            for (int y = 0; y < ROWNO; y++) {
                if (has_coating(x, y, COAT_BLOOD)) {
                    remove_coating(x, y, COAT_BLOOD);
                    explode(x, y, PHYS_EXPL_TYPE, d(4, 4), 0, EXPL_MAGICAL);
                }
            }
        }
        break;
    case MCU_BLOOD_SPEAR:
        dmg = d(10, 10);
        pline("The blood on the %s springs to life and %s you!",
                surface(u.ux, u.uy),
                (mtmp->data == &mons[PM_VLAD_THE_IMPALER]) ? "impales" : "stabs");
        remove_coating(u.ux, u.uy, COAT_BLOOD);
        break;
    case MCU_BLOODRUSH:
        if (canseemon(mtmp))
            pline_mon(mtmp, "%s is covered in blood.", Monnam(mtmp));
        make_mdripping(mtmp, -1 * mtmp->mnum);
        FALLTHROUGH;
        /*FALLTHRU*/
    case MCU_HASTE_SELF:
        mon_adjust_speed(mtmp, 1, (struct obj *) 0);
        dmg = 0;
        break;
    case MCU_PSI_BOLT:
        /* prior to 3.4.0 Antimagic was setting the damage to 1--this
           made the spell virtually harmless to players with magic res. */
        if (Antimagic) {
            shieldeff(u.ux, u.uy);
            monstseesu(M_SEEN_MAGR);
            dmg = (dmg + 1) / 2;
        } else {
            monstunseesu(M_SEEN_MAGR);
        }
        if (dmg <= 5)
            You("get a slight %sache.", body_part(HEAD));
        else if (dmg <= 10)
            Your("brain is on fire!");
        else if (dmg <= 20)
            Your("%s suddenly aches painfully!", body_part(HEAD));
        else
            Your("%s suddenly aches very painfully!", body_part(HEAD));
        break;
    case MCU_GRAVITY: {
        int quan = rnd(2);
        coord bypos;
        pline_The("air quavers.");
        for (int i = 0; i < quan; i++) {
            if (!enexto(&bypos, mtmp->mx, mtmp->my, mtmp->data))
                break;
            makemon(&mons[PM_GRAVIMETRIC_SPHERE], bypos.x, bypos.y, MM_NOCOUNTBIRTH | MM_ANGRY);
        }
        dmg = 0;
        break;
    } case MCU_GEYSER:
        /* this is physical damage (force not heat),
         * not magical damage or fire damage
         */
        pline("A sudden geyser slams into you from nowhere!");
        dmg = d(8, 6);
        if (Half_physical_damage)
            dmg = (dmg + 1) / 2;
#if 0   /* since inventory items aren't affected, don't include this */
        /* make floor items wet */
        water_damage_chain(level.objects[u.ux][u.uy], TRUE);
#endif
        add_coating(u.ux, u.uy, COAT_POTION, POT_WATER);
        make_dripping(rnd(20), POT_WATER, NON_PM);
        break;
    case MCU_TELEPORT: {
        /* Warp the monster directly next to the player, or teleport them
           elsewhere if their health is low.*/
        if (mtmp->mhp * 3 >= mtmp->mhpmax)
            mnexto(mtmp, RLOC_MSG);
        else {
            coordxy sx, sy;
            coordxy ox = mtmp->mx;
            coordxy oy = mtmp->my;
            choose_stairs(&sx, &sy, (mtmp->m_id % 2));
            mnearto(mtmp, sx, sy, TRUE, RLOC_NOMSG);
            /* Leave behind an illusory duplicate (maybe) */
            if (!Protection_from_shape_changers && rn2(mtmp->m_lev) < 20) {
                spawn_mirror_image(mtmp, ox, oy);
            }
        }
        dmg = 0;
        break;   
    } 
    case MCU_FIRE_PILLAR:
        pline("A pillar of fire strikes all around you!");
        orig_dmg = dmg = d(8, 6);
        if (Fire_resistance) {
            shieldeff(u.ux, u.uy);
            monstseesu(M_SEEN_FIRE);
            dmg = 0;
        } else {
            monstunseesu(M_SEEN_FIRE);
        }
        if (Half_spell_damage)
            dmg = (dmg + 1) / 2;
        burn_away_slime();
        (void) burnarmor(&gy.youmonst);
        /* item destruction dmg */
        (void) destroy_items(&gy.youmonst, AD_FIRE, orig_dmg);
        ignite_items(gi.invent);
        /* burn up flammable items on the floor, melt ice terrain */
        mon_spell_hits_spot(mtmp, AD_FIRE, u.ux, u.uy);
        evaporate_potion_puddles(u.ux, u.uy);
        make_dripping(0, 0, NON_PM);
        break;
    case MCU_LIGHTNING: {
        boolean reflects;

        Soundeffect(se_bolt_of_lightning, 80);
        pline("A bolt of lightning strikes down at you from above!");
        reflects = ureflects("It bounces off your %s%s.", "");
        orig_dmg = dmg = d(8, 6);
        if (reflects || Shock_resistance) {
            shieldeff(u.ux, u.uy);
            dmg = 0;
            if (reflects) {
                monstseesu(M_SEEN_REFL);
                break;
            }
            monstunseesu(M_SEEN_REFL);
            monstseesu(M_SEEN_ELEC);
        } else {
            monstunseesu(M_SEEN_ELEC | M_SEEN_REFL);
        }
        if (Half_spell_damage)
            dmg = (dmg + 1) / 2;
        (void) destroy_items(&gy.youmonst, AD_ELEC, orig_dmg);
        /* lightning might destroy iron bars if hero is on such a spot;
           reflection protects terrain here [execution won't get here due
           to 'if (reflects) break' above] but hero resistance doesn't;
           do this before maybe blinding the hero via flashburn() */
        mon_spell_hits_spot(mtmp, AD_ELEC, u.ux, u.uy);
        /* blind hero; no effect if already blind */
        (void) flashburn((long) rnd(100), TRUE);
        break;
    }
    case MCU_CURSE_ITEMS:
        You_feel("as if you need some help.");
        rndcurse();
        dmg = 0;
        break;
    case MCU_FORCE_FIELD:
        pline("A wall of force slams down around you!");
        create_force_field(u.ux, u.uy, 2, (long) rn1(5, 5));
        dmg = 0;
        break;
    case MCU_BLOOD_RAIN:
    case MCU_CHAOS_RAIN: {
        int startx = max(u.ux - 1, 0);
        int starty = max(u.uy - 1, 0);
        int stopx = min(u.ux + 1, COLNO - 1);
        int stopy = min(u.uy + 1, ROWNO - 1);
        int otyp;
        pline("%s rains down around you!",
                (spellnum == MCU_BLOOD_RAIN) ? "Blood" : "Raw chaos");
        for (int i = startx; i <= stopx; i++) {
            for (int j = starty; j <= stopy; j++) {
                otyp = (spellnum == MCU_BLOOD_RAIN) ? POT_BLOOD : 
                    (POT_GAIN_ABILITY + rn2(POT_OIL - POT_GAIN_ABILITY));
                floor_spillage(i, j, otyp, PM_HUMAN);
            }
        }
        dmg = d(1, 6);
        break;
    }
    case MCU_INSECTS: {
        /* Try for insects, and if there are none
           left, go for (sticks to) snakes.  -3. */
        struct permonst *pm = mkclass(S_ANT, 0);
        struct monst *mtmp2 = (struct monst *) 0;
        char whatbuf[QBUFSZ], let = (pm ? S_ANT : S_SNAKE);
        boolean success = FALSE, seecaster;
        int i, quan, oldseen, newseen;
        coord bypos;
        const char *fmt, *what;

        oldseen = monster_census(TRUE);
        quan = (mtmp->m_lev < 2) ? 1 : rnd((int) mtmp->m_lev / 2);
        if (quan < 3)
            quan = 3;
        for (i = 0; i <= quan; i++) {
            if (!enexto(&bypos, mtmp->mux, mtmp->muy, mtmp->data))
                break;
            if ((pm = mkclass(let, 0)) != 0
                && (mtmp2 = makemon(pm, bypos.x, bypos.y, MM_ANGRY | MM_NOMSG | MM_ESUM))
                   != 0) {
                success = TRUE;
                mtmp2->msleeping = mtmp2->mpeaceful = mtmp2->mtame = 0;
                if (!has_esum(mtmp))
                    newesum(mtmp);
                ESUM(mtmp2)->ownermid = mtmp->m_id;
                set_malign(mtmp2);
            }
        }
        newseen = monster_census(TRUE);

        /* not canspotmon() which includes unseen things sensed via warning */
        seecaster = canseemon(mtmp) || tp_sensemon(mtmp) || Detect_monsters;
        what = (let == S_SNAKE) ? "snakes" : "insects";
        if (Hallucination)
            what = makeplural(bogusmon(whatbuf, (char *) 0));

        fmt = 0;
        if (!seecaster) {
            if (newseen <= oldseen || Unaware) {
                /* unseen caster fails or summons unseen critters,
                   or unconscious hero ("You dream that you hear...") */
                You_hear("someone summoning %s.", what);
            } else {
                char *arg;

                if (what != whatbuf)
                    what = strcpy(whatbuf, what);
                /* unseen caster summoned seen critter(s) */
                arg = (newseen == oldseen + 1) ? an(makesingular(what))
                                               : whatbuf;
                if (!Deaf) {
                    Soundeffect(se_someone_summoning, 100);
                    You_hear("someone summoning something, and %s %s.", arg,
                             vtense(arg, "appear"));
                } else {
                    pline("%s %s.", upstart(arg), vtense(arg, "appear"));
                }
            }

        /* seen caster, possibly producing unseen--or just one--critters;
           hero is told what the caster is doing and doesn't necessarily
           observe complete accuracy of that caster's results (in other
           words, no need to fuss with visibility or singularization;
           player is told what's happening even if hero is unconscious) */
        } else if (!success) {
            fmt = "%s casts at a clump of sticks, but nothing happens.%s";
            what = "";
        } else if (let == S_SNAKE) {
            fmt = "%s transforms a clump of sticks into %s!";
        } else if (Invis && !perceives(mtmp->data)
                   && (mtmp->mux != u.ux || mtmp->muy != u.uy)) {
            fmt = "%s summons %s around a spot near you!";
        } else if (Displaced && (mtmp->mux != u.ux || mtmp->muy != u.uy)) {
            fmt = "%s summons %s around your displaced image!";
        } else {
            fmt = "%s summons %s!";
        }
        if (fmt)
            pline_mon(mtmp, fmt, Monnam(mtmp), what);

        dmg = 0;
        break;
    }
    case MCU_BLIND_YOU:
        /* note: resists_blnd() doesn't apply here */
        if (!Blinded) {
            int num_eyes = eyecount(gy.youmonst.data);

            pline("Scales cover your %s!", (num_eyes == 1)
                                               ? body_part(EYE)
                                               : makeplural(body_part(EYE)));
            make_blinded(Half_spell_damage ? 100L : 200L, FALSE);
            if (!Blind)
                Your1(vision_clears);
            dmg = 0;
        } else
            impossible("no reason for monster to cast blindness spell?");
        break;
    case MCU_PARALYZE:
        if (Antimagic || Free_action) {
            shieldeff(u.ux, u.uy);
            monstseesu(M_SEEN_MAGR);
            if (gm.multi >= 0)
                You("stiffen briefly.");
            dmg = 1; /* to produce nomul(-1), not actual damage */
        } else {
            if (gm.multi >= 0)
                You("are frozen in place!");
            dmg = 4 + (int) mtmp->m_lev;
            if (Half_spell_damage)
                dmg = (dmg + 1) / 2;
            monstunseesu(M_SEEN_MAGR);
        }
        nomul(-dmg);
        gm.multi_reason = "paralyzed by a monster";
        gn.nomovemsg = 0;
        dmg = 0;
        break;
    case MCU_LEVITATE_YOU: {
        struct obj *pseudo = mksobj(SPE_LEVITATION, FALSE, FALSE);
        pseudo->cursed = 1;
        pseudo->quan = 20L;
        (void) peffects(pseudo);
        obfree(pseudo, (struct obj *) 0);
        dmg = 0;
        break;
    } case MCU_CONFUSE_YOU:
        if (Antimagic) {
            shieldeff(u.ux, u.uy);
            monstseesu(M_SEEN_MAGR);
            You_feel("momentarily dizzy.");
        } else {
            boolean oldprop = !!Confusion;

            dmg = (int) mtmp->m_lev;
            if (Half_spell_damage)
                dmg = (dmg + 1) / 2;
            make_confused(HConfusion + dmg, TRUE);
            if (Hallucination)
                You_feel("%s!", oldprop ? "trippier" : "trippy");
            else
                You_feel("%sconfused!", oldprop ? "more " : "");
            monstunseesu(M_SEEN_MAGR);
        }
        dmg = 0;
        break;
    case MCU_GREASE:
        make_glib((int) (Glib & TIMEOUT) + rn1(4, 5));
        pline("Grease splatters you!");
        potion_splatter(u.ux, u.uy, POT_OIL, NON_PM);
        dmg = 0;
        break;
    case MCU_DISGUISE:
        if (canseemon(mtmp))
            pline_mon(mtmp, "%s %s.", Monnam(mtmp),
                Role_if(PM_ROGUE) ? "magically disguises itself" : "transforms");
        mtmp->m_ap_type = M_AP_MONSTER;
        mtmp->mappearance = rndmonnum();
        newsym(mtmp->mx, mtmp->my);
        dmg = 0;
        break;
    case MCU_DARKNESS:
        litroom(FALSE, (struct obj *) 0);
        dmg = 0;
        break;
    case MCU_CURE_SELF:
        dmg = m_cure_self(mtmp, dmg);
        break;
    case MCU_OPEN_WOUNDS:
        if (Antimagic) {
            shieldeff(u.ux, u.uy);
            monstseesu(M_SEEN_MAGR);
            dmg = (dmg + 1) / 2;
        } else {
            monstunseesu(M_SEEN_MAGR);
        }
        if (dmg <= 5)
            Your("skin itches badly for a moment.");
        else if (dmg <= 10)
            pline("Wounds appear on your body!");
        else if (dmg <= 20)
            pline("Severe wounds appear on your body!");
        else
            Your("body is covered with painful wounds!");
        /* Start to bleed if it's bad enough */
        make_dripping(rn1(10, 10), POT_BLOOD, gy.youmonst.mnum);
        break;
    default:
        impossible("mcastu: invalid monster spell (%d)", spellnum);
        dmg = 0;
        break;
    }

    if (dmg)
        mdamageu(mtmp, dmg);
}

RESTORE_WARNING_FORMAT_NONLITERAL

staticfn boolean
is_undirected_spell(int spellnum)
{
    switch (spellnum) {
    case MCU_CLONE_WIZ:
    case MCU_SUMMON_MONS:
    case MCU_AGGRAVATION:
    case MCU_DISAPPEAR:
    case MCU_HASTE_SELF:
    case MCU_BLOODRUSH:
    case MCU_RAISE_DEAD:
    case MCU_GRAVITY:
    case MCU_INSECTS:
    case MCU_CHAOS_RAIN:
    case MCU_FORCE_FIELD:
    case MCU_CURE_SELF:
    case MCU_MIRROR_IMAGE:
    case MCU_DISGUISE:
    case MCU_TELEPORT:
    case MCU_BLOOD_SPEAR:
    case MCU_BLOOD_RAIN:
    case MCU_BLOOD_BIND:
    case MCU_DARKNESS:
        return TRUE;
    default:
        break;
    }
    return FALSE;
}

/* Some spells are useless under some circumstances. */
staticfn boolean
spell_would_be_useless(struct monst *mtmp, int spellnum)
{
    /* Some spells don't require the player to really be there and can be cast
     * by the monster when you're invisible, yet still shouldn't be cast when
     * the monster doesn't even think you're there.
     * This check isn't quite right because it always uses your real position.
     * We really want something like "if the monster could see mux, muy".
     */
    boolean mcouldseeu = couldsee(mtmp->mx, mtmp->my);
    NhRegion *ff;

    /* aggravate monsters, etc. won't be cast by peaceful monsters */
    if (mtmp->mpeaceful
        && (spellnum == MCU_AGGRAVATION || spellnum == MCU_SUMMON_MONS
            || spellnum == MCU_CLONE_WIZ || spellnum == MCU_GRAVITY
            || spellnum == MCU_RAISE_DEAD || spellnum == MCU_MIRROR_IMAGE
            || spellnum == MCU_INSECTS || spellnum == MCU_CHAOS_RAIN
            || spellnum == MCU_BLOOD_SPEAR || spellnum == MCU_BLOODRUSH
            || spellnum == MCU_BLOOD_RAIN || spellnum == MCU_BLOOD_BIND
            || spellnum == MCU_DARKNESS))
        return TRUE;
    /* illusiory armies play absolute hell with fuzzing. */
    if (iflags.debug_fuzzer && spellnum == MCU_MIRROR_IMAGE)
        return TRUE;
    /* haste self when already fast */
    if (mtmp->permspeed == MFAST
        && (spellnum == MCU_HASTE_SELF || spellnum == MCU_BLOODRUSH))
        return TRUE;
    /* teleport and already close to the player */
    if (spellnum == MCU_TELEPORT && distu(mtmp->mx, mtmp->my) <= 4
        && mtmp->mhp * 3 >= mtmp->mhpmax)
        return TRUE;
    if (spellnum == MCU_LEVITATE_YOU && (Levitation || Flying || Punished))
        return TRUE;
    /* invisibility when already invisible */
    if ((mtmp->minvis || mtmp->invis_blkd) && spellnum == MCU_DISAPPEAR)
        return TRUE;
    /* peaceful monster won't cast invisibility if you can't see
        invisible,
        same as when monsters drink potions of invisibility.  This doesn't
        really make a lot of sense, but lets the player avoid hitting
        peaceful monsters by mistake */
    if (mtmp->mpeaceful && !See_invisible && spellnum == MCU_DISAPPEAR)
        return TRUE;
    /* healing when already healed */
    if (mtmp->mhp == mtmp->mhpmax && spellnum == MCU_CURE_SELF)
        return TRUE;
    /* don't summon monsters if it doesn't think you're around */
    if (!mcouldseeu && (spellnum == MCU_SUMMON_MONS
                        || (!mtmp->iswiz && spellnum == MCU_CLONE_WIZ)))
        return TRUE;
    if ((!mtmp->iswiz || svc.context.no_of_wizards > 1)
        && spellnum == MCU_CLONE_WIZ)
        return TRUE;
    /* aggravation (global wakeup) when everyone is already active */
    if (spellnum == MCU_AGGRAVATION) {
        /* if nothing needs to be awakened then this spell is useless
            but caster might not realize that [chance to pick it then
            must be very small otherwise caller's many retry attempts
            will eventually end up picking it too often] */
        if (!has_aggravatables(mtmp))
            return rn2(100) ? TRUE : FALSE;
    }
    /* Cannot disguise if protected */
    if ((Protection_from_shape_changers || mtmp->mpeaceful)
        && (spellnum == MCU_DISGUISE || spellnum == MCU_MIRROR_IMAGE))
        return TRUE;
    if (spellnum == MCU_FORCE_FIELD &&
        (mtmp->mpeaceful || !mcouldseeu || distu(mtmp->mx, mtmp->my) <= 4
        || (((ff = visible_region_at(u.ux, u.uy)) != 0)
            && ff->glyph == S_force_field)))
        return TRUE;
    /* healing when already healed */
    if (mtmp->mhp == mtmp->mhpmax && spellnum == MCU_CURE_SELF)
        return TRUE;
    /* don't summon insects if it doesn't think you're around */
    if (!mcouldseeu &&
        (spellnum == MCU_INSECTS || spellnum == MCU_CHAOS_RAIN
         || spellnum == MCU_BLOOD_RAIN || spellnum == MCU_BLOOD_SPEAR
         || spellnum == MCU_BLOOD_BIND))
        return TRUE;
    /* blindness spell on blinded player */
    if (Blinded && spellnum == MCU_BLIND_YOU)
        return TRUE;
    /* blood spear when player not on blood */
    if ((spellnum == MCU_BLOOD_SPEAR || spellnum == MCU_BLOOD_BIND)
        && (!has_coating(u.ux, u.uy, COAT_BLOOD)))
        return TRUE;
    /* blood rain if already bloody */
    if (spellnum == MCU_BLOOD_RAIN
        && has_coating(u.ux, u.uy, COAT_BLOOD))
        return TRUE;
    return FALSE;
}

/* monster uses spell (ranged) */
int
buzzmu(struct monst *mtmp, struct attack *mattk)
{
    /* don't print constant stream of curse messages for 'normal'
       spellcasting monsters at range */
    if (!BZ_VALID_ADTYP(mattk->adtyp))
        return M_ATTK_MISS;

    if (mtmp->mcan || m_seenres(mtmp, cvt_adtyp_to_mseenres(mattk->adtyp))) {
        cursetxt(mtmp, FALSE);
        return M_ATTK_MISS;
    }
    if (lined_up(mtmp) && rn2(3)) {
        nomul(0);
        if (canseemon(mtmp))
            pline_mon(mtmp, "%s zaps you with a %s!", Monnam(mtmp),
                  flash_str(BZ_OFS_AD(mattk->adtyp), FALSE));
        gb.buzzer = mtmp;
        buzz(BZ_M_SPELL(BZ_OFS_AD(mattk->adtyp)), (int) mattk->damn,
             mtmp->mx, mtmp->my, sgn(gt.tbx), sgn(gt.tby));
        gb.buzzer = 0;
        return M_ATTK_HIT;
    }
    return M_ATTK_MISS;
}

/* Returns 1 if illusions were seen being created */
staticfn int
spawn_mirror_image(struct monst *mtmp, int x, int y) {
    struct monst *illusion = 
        makemon(&mons[PM_ILLUSION], 
        x, y, MM_NOCOUNTBIRTH | MM_ANGRY | MM_NOMSG);
    if (illusion) {
        if (mtmp->mappearance && !Protection_from_shape_changers)
            illusion->mappearance = mtmp->mappearance;
        else
            illusion->mappearance = mtmp->mnum;
        newsym(illusion->mx, illusion->my);
        if (canseemon(mtmp))
            return 1;
    }
    return 0;
}

/*mcastu.c*/
