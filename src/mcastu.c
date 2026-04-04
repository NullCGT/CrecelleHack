/* NetHack 3.7	mcastu.c	$NHDT-Date: 1770949988 2026/02/12 18:33:08 $  $NHDT-Branch: NetHack-3.7 $:$NHDT-Revision: 1.111 $ */
/* Copyright (c) Stichting Mathematisch Centrum, Amsterdam, 1985. */
/*-Copyright (c) Robert Patrick Rankin, 2011. */
/* NetHack may be freely redistributed.  See license for details. */

#include "hack.h"

#define MCASTU_ENUM
enum mcast_spells {
    #include "mcastu.h"
};
#undef MCASTU_ENUM

struct _mcast_data {
    const char *name;
    int level;
    int flags;
};

#define MCASTU_INIT
static struct _mcast_data mcast_data[] = {
    #include "mcastu.h"
};
#undef MCASTU_INIT

/* spell lists for specific monster casters */
/* the spells in the list should be in ascending level order */
static int mon_wizard_spells[] = { MCAST_PSI_BOLT, MCAST_CURE_SELF, MCAST_HASTE_SELF,
                                        MCAST_STUN_YOU, MCAST_DISAPPEAR, MCAST_WEAKEN_YOU,
                                        MCAST_DESTRY_ARMR, MCAST_CURSE_ITEMS, MCAST_AGGRAVATION,
                                        MCAST_SUMMON_MONS, MCAST_CLONE_WIZ, MCAST_DEATH_TOUCH};
static int mon_shadow_mage_spells[] = { MCAST_PSI_BOLT, MCAST_DARKNESS, MCAST_HASTE_SELF,
                                        MCAST_STUN_YOU, MCAST_DISAPPEAR, MCAST_WEAKEN_YOU,
                                        MCAST_DESTRY_ARMR, MCAST_CURSE_ITEMS, MCAST_SLEEP_YOU,
                                        MCAST_SUMMON_MONS, MCAST_MIRROR_IMAGE, MCAST_DEATH_TOUCH};
static int mon_vamp_spells[] = { MCAST_OPEN_WOUNDS, MCAST_CURE_SELF, MCAST_BLOODRUSH,
                                        MCAST_DISAPPEAR, MCAST_CURSE_ITEMS, MCAST_BLOOD_SPEAR,
                                        MCAST_BLOOD_RAIN, MCAST_BLOOD_BIND };
static int mon_cleric_spells[] = { MCAST_OPEN_WOUNDS, MCAST_CURE_SELF, MCAST_CONFUSE_YOU,
                                          MCAST_PARALYZE, MCAST_BLIND_YOU, MCAST_INSECTS,
                                          MCAST_CURSE_ITEMS, MCAST_LIGHTNING, MCAST_FIRE_PILLAR,
                                          MCAST_GEYSER };
static int mon_law_cleric_spells[] = { MCAST_OPEN_WOUNDS, MCAST_CURE_SELF, MCAST_CONFUSE_YOU,
                                          MCAST_PARALYZE, MCAST_BLIND_YOU, MCAST_FORCE_FIELD,
                                          MCAST_CURSE_ITEMS, MCAST_LIGHTNING, MCAST_FIRE_PILLAR,
                                          MCAST_GEYSER };
static int mon_chaos_cleric_spells[] = { MCAST_OPEN_WOUNDS, MCAST_CURE_SELF, MCAST_CONFUSE_YOU,
                                          MCAST_PARALYZE, MCAST_BLIND_YOU, MCAST_CHAOS_RAIN,
                                          MCAST_CURSE_ITEMS, MCAST_LIGHTNING, MCAST_FIRE_PILLAR,
                                          MCAST_GEYSER };
static int mon_undead_spells[] = { MCAST_HASTE_SELF, MCAST_STUN_YOU, MCAST_WEAKEN_YOU,
                                          MCAST_SLEEP_YOU, MCAST_CURSE_ITEMS,
                                          MCAST_CURSE_ITEMS, MCAST_AGGRAVATION, MCAST_RAISE_DEAD,
                                          MCAST_DEATH_TOUCH, MCAST_MIRROR_IMAGE, MCAST_DISAPPEAR,
                                          MCAST_TELEPORT };
static int mon_demo_spells[] = { MCAST_PSI_BOLT, MCAST_OPEN_WOUNDS, MCAST_CURE_SELF, 
                                        MCAST_HASTE_SELF, MCAST_STUN_YOU, MCAST_WEAKEN_YOU,
                                        MCAST_DESTRY_ARMR, MCAST_CURSE_ITEMS, MCAST_AGGRAVATION,
                                        MCAST_SUMMON_MONS, MCAST_CHAOS_RAIN, MCAST_DEATH_TOUCH};
static int mon_trickster_spells[] = { MCAST_PSI_BOLT, MCAST_HASTE_SELF, MCAST_DISAPPEAR,
                                             MCAST_LEVITATE_YOU,
                                             MCAST_AGGRAVATION, MCAST_MIRROR_IMAGE, MCAST_CONFUSE_YOU,
                                             MCAST_GREASE, MCAST_DISGUISE, MCAST_CURSE_ITEMS, MCAST_SUMMON_MONS, 
                                             MCAST_TELEPORT };

DISABLE_WARNING_FORMAT_NONLITERAL

staticfn void cursetxt(struct monst *, boolean);
staticfn int choose_monster_spell(struct monst *, int);
staticfn int m_cure_self(struct monst *, int);
staticfn void mcast_death_touch(struct monst *);
staticfn void mcast_clone_wiz(struct monst *);
staticfn void mcast_mirror_image(struct monst *);
staticfn void mcast_raise_dead(struct monst *);
staticfn void mcast_summon_mons(struct monst *);
staticfn void mcast_destroy_armor(void);
staticfn void mcast_weaken_you(struct monst *, int);
staticfn void mcast_disappear(struct monst *);
staticfn void mcast_stun_you(int);
staticfn void mcast_sleep_you(void);
staticfn int mcast_blood_spear(struct monst *);
staticfn void mcast_blood_bind(struct monst *);
staticfn int mcast_geyser(int);
staticfn int mcast_fire_pillar(struct monst *, int);
staticfn int mcast_lightning(struct monst *, int);
staticfn int mcast_psi_bolt(int);
staticfn int mcast_open_wounds(int);
staticfn void mcast_insects(struct monst *);
staticfn void mcast_blind_you(void);
staticfn int mcast_paralyze(struct monst *);
staticfn void mcast_confuse_you(struct monst *);
staticfn void mcast_levitate_you(void);
staticfn int mcast_weird_rain(int);
staticfn void mcast_spell(struct monst *, int, int);
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

/* choose a spell for monster to cast */
staticfn int
choose_monster_spell(struct monst *mtmp, int adtyp)
{
    int *list = NULL;
    int i, spellval, len = 0;
    int maxlev;
    int a;

    a = mon_aligntyp(mtmp);
    /* Randomly determine the alignment of unaligned casters. */
    if (a == A_NONE)
        a = aligns[(mtmp->m_id % 3)].value;

    /* which spell list to use? */
    if (mtmp->data->mlet == S_VAMPIRE
        || mtmp->data == &mons[PM_BLOOD_IMP]) {
        list = mon_vamp_spells;
        len = SIZE(mon_vamp_spells);
    } else if (mtmp->data == &mons[PM_DEMOGORGON]) {
        list = mon_demo_spells;
        len = SIZE(mon_demo_spells);
    } else if (mtmp->data == &mons[PM_DARK_ONE]) {
        list = mon_shadow_mage_spells;
        len = SIZE(mon_shadow_mage_spells);
    } else if (is_undead(mtmp->data) || mtmp->data == &mons[PM_ORCUS]) {
        list = mon_undead_spells;
        len = SIZE(mon_undead_spells);
    } else if (mtmp->data->mlet == S_GNOME || mtmp->data->mlet == S_KOBOLD
                || mtmp->data == &mons[PM_DISPATER]) {
        list = mon_trickster_spells;
        len = SIZE(mon_trickster_spells);
    } else if (adtyp == AD_CLRC) {
        if (a == A_LAWFUL) {
            list = mon_law_cleric_spells;
            len = SIZE(mon_law_cleric_spells);
        } else if (a == A_CHAOTIC) {
            list = mon_chaos_cleric_spells;
            len = SIZE(mon_chaos_cleric_spells);
        } else {
            list = mon_cleric_spells;
            len = SIZE(mon_cleric_spells);
        }
    } else {
        list = mon_wizard_spells;
        len = SIZE(mon_wizard_spells);
    }
    if (!list || len < 1)
        return MCAST_PSI_BOLT;

    /* max spell level in this monster spell list */
    maxlev = mcast_data[list[len - 1]].level;

    /* which level spell to cast? */
    spellval = rn2(mtmp->m_lev);
    if (spellval > maxlev && rn2(maxlev))
        spellval = rn2(maxlev);

    /* find the highest spell in the list we could cast */
    for (i = len-1; i >= 0; i--)
        if (mcast_data[list[i]].level <= spellval
            && !spell_would_be_useless(mtmp, list[i]))
            return list[i];

    /* or return the first spell in the list */
    return list[0];
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

    debugpline3("castmu:%s,lvl:%i,spell:%i", noit_Monnam(mtmp), ml, spellnum);

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
                        ? mcast_data[spellnum].name : "a spell",
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
                        ? mcast_data[spellnum].name : "a spell",
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
        if (Fire_immunity) {
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
        if (Cold_immunity) {
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
        mcast_spell(mtmp, dmg, spellnum);
        dmg = 0; /* done by the spell casting functions */
        break;
    } /* switch */
    if (dmg) {
        adjust_damage(&gy.youmonst, &dmg, mattk->adtyp);
        mdamageu(mtmp, dmg);
    }
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

staticfn void
mcast_death_touch(struct monst *mtmp)
{
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
}

staticfn void
mcast_clone_wiz(struct monst *mtmp)
{
    if (mtmp->iswiz && svc.context.no_of_wizards == 1) {
        pline("Double Trouble...");
        clonewiz();
    } else
        impossible("bad wizard cloning?");
}

staticfn void
mcast_mirror_image(struct monst *mtmp)
{
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
}

staticfn void
mcast_raise_dead(struct monst *mtmp)
{
    coord mm;
    pline_The("dead speak!");
    mm.x = mtmp->mx;
    mm.y = mtmp->my;
    if (!rn2(3))
        (void) unturn_dead(&gy.youmonst);
    mkundead(&mm, TRUE, NO_MINVENT);
}

staticfn void
mcast_summon_mons(struct monst *mtmp)
{
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
}

staticfn void
mcast_destroy_armor(void)
{
    if (Antimagic) {
        shieldeff(u.ux, u.uy);
        monstseesu(M_SEEN_MAGR);
        pline("A field of force surrounds you!");
    } else if (!destroy_arm()) {
        Your("skin itches.");
    } else {
        /* monsters only realize you aren't magic-protected if armor is
           actually destroyed */
        monstunseesu(M_SEEN_MAGR);
    }
}

staticfn void
mcast_weaken_you(struct monst *mtmp, int dmg)
{
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
}

staticfn void
mcast_disappear(struct monst *mtmp)
{
    if (!mtmp->minvis && !mtmp->invis_blkd) {
        if (canseemon(mtmp))
            pline_mon(mtmp, "%s suddenly %s!", Monnam(mtmp),
                      !See_invisible ? "disappears" : "becomes transparent");
        mon_set_minvis(mtmp, FALSE);
        if (cansee(mtmp->mx, mtmp->my) && !canspotmon(mtmp))
            map_invisible(mtmp->mx, mtmp->my);
    } else
        impossible("no reason for monster to cast disappear spell?");
}

staticfn void
mcast_stun_you(int dmg)
{
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
}

staticfn void
mcast_sleep_you(void)
{
    if (!Free_action && !Sleep_resistance) {
        You_feel("feel exhausted.");
        fall_asleep(-d(5, 5), TRUE);
        exercise(A_DEX, FALSE);
        monstunseesu(M_SEEN_SLEEP);
    } else {
        You("yawn.");
        monstseesu(M_SEEN_SLEEP);
    }
}

staticfn int
mcast_blood_spear(struct monst *mtmp)
{
    pline("The blood on the %s springs to life and %s you!",
                surface(u.ux, u.uy),
                (mtmp->data == &mons[PM_VLAD_THE_IMPALER]) ? "impales" : "stabs");
    remove_coating(u.ux, u.uy, COAT_BLOOD);
    return d(10, 10);
}

staticfn void
mcast_blood_bind(struct monst *mtmp)
{
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
}

staticfn int
mcast_geyser(int dmg)
{
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
    return dmg;
}

staticfn int
mcast_fire_pillar(struct monst *mtmp, int dmg)
{
    int orig_dmg;

    pline("A pillar of fire strikes all around you!");
    orig_dmg = dmg = d(8, 6);
    if (Fire_immunity) {
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
    return dmg;
}

staticfn int
mcast_lightning(struct monst *mtmp, int dmg)
{
    int orig_dmg;
    boolean reflects;
    Soundeffect(se_bolt_of_lightning, 80);
    pline("A bolt of lightning strikes down at you from above!");
    reflects = ureflects("It bounces off your %s%s.", "");
    orig_dmg = dmg = d(8, 6);
    if (reflects || Shock_immunity) {
        shieldeff(u.ux, u.uy);
        dmg = 0;
        if (reflects) {
            monstseesu(M_SEEN_REFL);
            return dmg;
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
    return dmg;
}

staticfn int
mcast_psi_bolt(int dmg)
{
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
    return dmg;
}

staticfn int
mcast_open_wounds(int dmg)
{
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
    return dmg;
}

staticfn void
mcast_insects(struct monst *mtmp)
{
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
            return;
        if ((pm = mkclass(let, 0)) != 0
            && (mtmp2 = makemon(pm, bypos.x, bypos.y, MM_ANGRY | MM_NOMSG))
            != 0) {
            success = TRUE;
            mtmp2->msleeping = mtmp2->mpeaceful = mtmp2->mtame = 0;
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
    if (fmt) {
        DISABLE_WARNING_FORMAT_NONLITERAL;
        pline_mon(mtmp, fmt, Monnam(mtmp), what);
        RESTORE_WARNING_FORMAT_NONLITERAL;
    }
}

staticfn void
mcast_blind_you(void)
{
    /* note: resists_blnd() doesn't apply here */
    if (!Blinded) {
        int num_eyes = eyecount(gy.youmonst.data);

        pline("Scales cover your %s!", (num_eyes == 1)
                                       ? body_part(EYE)
                                       : makeplural(body_part(EYE)));
        make_blinded(Half_spell_damage ? 100L : 200L, FALSE);
        if (!Blind)
            Your1(vision_clears);
    } else
        impossible("no reason for monster to cast blindness spell?");
}

staticfn int
mcast_paralyze(struct monst *mtmp)
{
    int dmg = 0;

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
    return dmg;
}

staticfn void
mcast_confuse_you(struct monst *mtmp)
{
    if (Antimagic) {
        shieldeff(u.ux, u.uy);
        monstseesu(M_SEEN_MAGR);
        You_feel("momentarily dizzy.");
    } else {
        boolean oldprop = !!Confusion;
        int dmg = (int) mtmp->m_lev;

        if (Half_spell_damage)
            dmg = (dmg + 1) / 2;
        make_confused(HConfusion + dmg, TRUE);
        if (Hallucination)
            You_feel("%s!", oldprop ? "trippier" : "trippy");
        else
            You_feel("%sconfused!", oldprop ? "more " : "");
        monstunseesu(M_SEEN_MAGR);
    }
}

staticfn void
mcast_levitate_you(void)
{
    struct obj *pseudo = mksobj(SPE_LEVITATION, FALSE, FALSE);
    pseudo->cursed = 1;
    pseudo->quan = 20L;
    (void) peffects(pseudo);
    obfree(pseudo, (struct obj *) 0);
}

staticfn int
mcast_weird_rain(int spellnum)
{
    int startx = max(u.ux - 1, 0);
    int starty = max(u.uy - 1, 0);
    int stopx = min(u.ux + 1, COLNO - 1);
    int stopy = min(u.uy + 1, ROWNO - 1);
    int otyp;
    pline("%s rains down around you!",
            (spellnum == MCAST_BLOOD_RAIN) ? "Blood" : "Raw chaos");
    for (int i = startx; i <= stopx; i++) {
        for (int j = starty; j <= stopy; j++) {
            otyp = (spellnum == MCAST_BLOOD_RAIN) ? POT_BLOOD : 
                (POT_GAIN_ABILITY + rn2(POT_OIL - POT_GAIN_ABILITY));
            floor_spillage(i, j, otyp, PM_HUMAN);
        }
    }
    return d(1, 6);
}

/*
   If dmg is zero, then the monster is not casting at you.
   If the monster is intentionally not casting at you, we have previously
   called spell_would_be_useless() and spellnum should always be a valid
   undirected spell.
   If you modify either of these, be sure to change is_undirected_spell()
   and spell_would_be_useless().
 */
staticfn void
mcast_spell(struct monst *mtmp, int dmg, int spellnum)
{
    int adtyp = 0;
    if (dmg < 0) {
        impossible("monster cast spell (%d) with negative dmg (%d)?",
                   spellnum, dmg);
        return;
    }
    if (dmg == 0 && !is_undirected_spell(spellnum)) {
        impossible("cast directed spell (%d) with dmg=0?", spellnum);
        return;
    }

    switch (spellnum) {
    case MCAST_DEATH_TOUCH:
        mcast_death_touch(mtmp);
        dmg = 0;
        break;
    case MCAST_CLONE_WIZ:
        mcast_clone_wiz(mtmp);
        dmg = 0;
        break;
    case MCAST_MIRROR_IMAGE:
        mcast_mirror_image(mtmp);
        dmg = 0;
        break;
    case MCAST_RAISE_DEAD:
        mcast_raise_dead(mtmp);
        dmg = 0;
        break;
    case MCAST_SUMMON_MONS:
        mcast_summon_mons(mtmp);
        dmg = 0;
        break;
    case MCAST_AGGRAVATION:
        You_feel("that monsters are aware of your presence.");
        aggravate();
        dmg = 0;
        break;
    case MCAST_CURSE_ITEMS:
        You_feel("as if you need some help.");
        rndcurse();
        dmg = 0;
        break;
    case MCAST_DESTRY_ARMR:
        mcast_destroy_armor();
        dmg = 0;
        break;
    case MCAST_WEAKEN_YOU: /* drain strength */
        mcast_weaken_you(mtmp, dmg);
        dmg = 0;
        break;
    case MCAST_DISAPPEAR: /* makes self invisible */
        mcast_disappear(mtmp);
        dmg = 0;
        break;
    case MCAST_SLEEP_YOU:
        mcast_sleep_you();
        dmg = 0;
        break;
    case MCAST_STUN_YOU:
        mcast_stun_you(dmg);
        dmg = 0;
        break;
    case MCAST_BLOOD_SPEAR:
        dmg = mcast_blood_spear(mtmp);
        break;
    case MCAST_BLOOD_BIND:
        mcast_blood_bind(mtmp);
        break;
    case MCAST_BLOODRUSH:
        if (canseemon(mtmp))
            pline_mon(mtmp, "%s is covered in blood.", Monnam(mtmp));
        make_mdripping(mtmp, -1 * mtmp->mnum);
        FALLTHROUGH;
        /*FALLTHRU*/
    case MCAST_HASTE_SELF:
        mon_adjust_speed(mtmp, 1, (struct obj *) 0);
        dmg = 0;
        break;
    case MCAST_CURE_SELF:
        dmg = m_cure_self(mtmp, dmg);
        break;
    case MCAST_PSI_BOLT:
        dmg = mcast_psi_bolt(dmg);
        break;
    case MCAST_GEYSER:
        dmg = mcast_geyser(dmg);
        break;
    case MCAST_FIRE_PILLAR:
        adtyp = AD_FIRE;
        dmg = mcast_fire_pillar(mtmp, dmg);
        break;
    case MCAST_LIGHTNING:
        adtyp = AD_ELEC;
        dmg = mcast_lightning(mtmp, dmg);
        break;
    case MCAST_INSECTS:
        mcast_insects(mtmp);
        dmg = 0;
        break;
    case MCAST_BLIND_YOU:
        mcast_blind_you();
        dmg = 0;
        break;
    case MCAST_PARALYZE:
        dmg = mcast_paralyze(mtmp);
        break;
    case MCAST_CONFUSE_YOU:
        mcast_confuse_you(mtmp);
        dmg = 0;
        break;
    case MCAST_LEVITATE_YOU:
        mcast_levitate_you();
        dmg = 0;
        break;
    case MCAST_FORCE_FIELD:
        pline("A wall of force slams down around you!");
        create_force_field(u.ux, u.uy, 2, (long) rn1(5, 5));
        dmg = 0;
        break;
    case MCAST_OPEN_WOUNDS:
        dmg = mcast_open_wounds(dmg);
        break;
    case MCAST_BLOOD_RAIN:
    case MCAST_CHAOS_RAIN:
        dmg = mcast_weird_rain(spellnum);
        break;
    case MCAST_GRAVITY: {
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
    }
    case MCAST_GREASE:
        make_glib((int) (Glib & TIMEOUT) + rn1(4, 5));
        pline("Grease splatters you!");
        potion_splatter(u.ux, u.uy, POT_OIL, NON_PM);
        dmg = 0;
        break;
    case MCAST_DISGUISE:
        if (canseemon(mtmp))
            pline_mon(mtmp, "%s %s.", Monnam(mtmp),
                Role_if(PM_ROGUE) ? "magically disguises itself" : "transforms");
        mtmp->m_ap_type = M_AP_MONSTER;
        mtmp->mappearance = rndmonnum();
        newsym(mtmp->mx, mtmp->my);
        dmg = 0;
        break;
    case MCAST_DARKNESS:
        litroom(FALSE, (struct obj *) 0);
        dmg = 0;
        break;
    }
    if (adtyp)
        adjust_damage(&gy.youmonst, &dmg, adtyp);
    if (dmg)
        mdamageu(mtmp, dmg);
}

staticfn boolean
is_undirected_spell(int spellnum)
{
    if ((mcast_data[spellnum].flags & MCF_INDIRECT) != 0)
        return TRUE;
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
    NhRegion *ff;

    /* spell is only cast by hostile monsters */
    if ((mcast_data[spellnum].flags & MCF_HOSTILE) != 0) {
        if (mtmp->mpeaceful)
            return TRUE;
    }

    /* spell needs the monster to see hero */
    if ((mcast_data[spellnum].flags & MCF_SIGHT) != 0) {
        boolean mcouldseeu = couldsee(mtmp->mx, mtmp->my);

        if (!mcouldseeu)
            return TRUE;
    }

    switch (spellnum) {
    case MCAST_DEATH_TOUCH:
        if ((Antimagic || Hallucination) && !rn2(2))
            return TRUE;
        break;
    case MCAST_GEYSER:
        if (!rn2(5))
            return TRUE;
        break;
    case MCAST_CLONE_WIZ:
        /* only the Wizard is allowed to clone himself */
        if (!mtmp->iswiz || svc.context.no_of_wizards > 1)
            return TRUE;
        break;
    case MCAST_TELEPORT:
        if ((distu(mtmp->mx, mtmp->my) > 4) || mtmp->mhp * 3 < mtmp->mhpmax)
            return TRUE;
        break;
    case MCAST_LEVITATE_YOU:
        if (!(Levitation || Flying || Punished))
            return TRUE;
        break;
    case MCAST_AGGRAVATION:
        /* aggravation (global wakeup) when everyone is already active */
        /* if nothing needs to be awakened then this spell is useless
           but caster might not realize that [chance to pick it then
           must be very small otherwise caller's many retry attempts
           will eventually end up picking it too often] */
        if (!has_aggravatables(mtmp))
            return rn2(100) ? TRUE : FALSE;
        break;
    case MCAST_BLOODRUSH:
    case MCAST_HASTE_SELF:
        /* haste self when already fast */
        if (mtmp->permspeed == MFAST)
            return TRUE;
        break;
    case MCAST_MIRROR_IMAGE:
        return !iflags.debug_fuzzer;
    case MCAST_DISAPPEAR:
        /* invisibility when already invisible */
        if (mtmp->minvis || mtmp->invis_blkd)
            return TRUE;
        /* peaceful monster won't cast invisibility if you can't see
           invisible,
           same as when monsters drink potions of invisibility.  This doesn't
           really make a lot of sense, but lets the player avoid hitting
           peaceful monsters by mistake */
        if (mtmp->mpeaceful && !See_invisible)
            return TRUE;
        break;
    case MCAST_CURE_SELF:
        /* healing when already healed */
        if (mtmp->mhp == mtmp->mhpmax)
            return TRUE;
        break;
    case MCAST_BLIND_YOU:
        if (Blinded)
            return TRUE;
        break;
    default:
        break;
    }
    /* Cannot disguise if protected */
    if ((Protection_from_shape_changers || mtmp->mpeaceful)
        && (spellnum == MCAST_DISGUISE || spellnum == MCAST_MIRROR_IMAGE))
        return TRUE;
    if (spellnum == MCAST_FORCE_FIELD &&
        (mtmp->mpeaceful || distu(mtmp->mx, mtmp->my) <= 4
        || (((ff = visible_region_at(u.ux, u.uy)) != 0)
            && ff->glyph == S_force_field)))
        return TRUE;
    /* healing when already healed */
    if (mtmp->mhp == mtmp->mhpmax && spellnum == MCAST_CURE_SELF)
        return TRUE;
    /* don't summon insects if it doesn't think you're around */
    if ((spellnum == MCAST_INSECTS || spellnum == MCAST_CHAOS_RAIN
         || spellnum == MCAST_BLOOD_RAIN || spellnum == MCAST_BLOOD_SPEAR
         || spellnum == MCAST_BLOOD_BIND))
        return TRUE;
    /* blindness spell on blinded player */
    if (Blinded && spellnum == MCAST_BLIND_YOU)
        return TRUE;
    /* blood spear when player not on blood */
    if ((spellnum == MCAST_BLOOD_SPEAR || spellnum == MCAST_BLOOD_BIND)
        && (!has_coating(u.ux, u.uy, COAT_BLOOD)))
        return TRUE;
    /* blood rain if already bloody */
    if (spellnum == MCAST_BLOOD_RAIN
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
