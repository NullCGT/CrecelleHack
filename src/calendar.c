/* NetHack 3.7	calendar.c	$NHDT-Date: 1706213796 2024/01/25 20:16:36 $  $NHDT-Branch: NetHack-3.7 $:$NHDT-Revision: 1.116 $ */
/* Copyright (c) Stichting Mathematisch Centrum, Amsterdam, 1985. */
/*-Copyright (c) Michael Allison, 2007. */
/* Copyright (c) Robert Patrick Rankin, 1991                      */
/* NetHack may be freely redistributed.  See license for details. */

#include "hack.h"

/*
 * Time routines
 *
 * The time is used for:
 *  - seed for rand()
 *  - year on tombstone and yyyymmdd in record file
 *  - phase of the moon (various monsters react to NEW_MOON or FULL_MOON)
 *  - night and midnight (the undead are dangerous at midnight)
 *  - determination of what files are "very old"
 */

/* TIME_type: type of the argument to time(); we actually use &(time_t);
   you might need to define either or both of these to 'long *' in *conf.h */
#ifndef TIME_type
#define TIME_type time_t *
#endif
#ifndef LOCALTIME_type
#define LOCALTIME_type time_t *
#endif

staticfn struct tm *getlt(void);
staticfn void weather_effects(void);
staticfn void weather_messages(void);

static const char *weather_strings[] = {
    "Calm", "Drizzle", "Rain", "Downburst", "Acid Rain", "Hail"
};

/* wizweather_precips must match weather_strings */
static struct weather wizweather_precips[] = {
    { 0, WTHM_ALL_PRECIPS, 300, 575 },
    { WTH_DRIZZLE, WTHM_ALL_PRECIPS, 200, 200 },
    { WTH_RAIN, WTHM_ALL_PRECIPS, 100, 200 },
    { WTH_DOWNBURST, WTHM_ALL_PRECIPS, 50, 10 },
    { WTH_ACIDRAIN, WTHM_ALL_PRECIPS, 50, 5 },
    { WTH_HAIL, 0, 20, 10 },
};

static struct weather dungeon_precips[] = {
    { 0, WTHM_ALL_PRECIPS, 300, 800 },
    { WTH_DRIZZLE, WTHM_ALL_PRECIPS, 200, 100 },
    { WTH_RAIN, WTHM_ALL_PRECIPS, 100, 95 },
    { WTH_DOWNBURST, WTHM_ALL_PRECIPS, 50, 5 },
};

static struct weather dungeon_winds[] = {
    { 0, WTHM_ALL_WINDS, 200, 400 },
    { WTH_BREEZE, WTH_GUST, 300, 250 },
    { WTH_WIND, 0, 150, 200 },
    { WTH_GUST, WTH_BREEZE, 150, 150 }
};

static struct weather harassment_precip[] = {
    { WTH_ACIDRAIN, 0, 100, 350 },
    { WTH_FIRERAIN, 0, 100, 350 },
    { WTH_HAIL, 0, 20, 200 },
    { WTH_TORNADO | WTH_GUST, 0, 50, 100 },
};

time_t
getnow(void)
{
    time_t datetime = 0;

    (void) time((TIME_type) &datetime);
    return datetime;
}

staticfn struct tm *
getlt(void)
{
    time_t date = getnow();

    return localtime((LOCALTIME_type) &date);
}

int
getyear(void)
{
    return (1900 + getlt()->tm_year);
}


long
yyyymmdd(time_t date)
{
    long datenum;
    struct tm *lt;

    if (date == 0)
        lt = getlt();
    else
        lt = localtime((LOCALTIME_type) &date);

    /* just in case somebody's localtime supplies (year % 100)
       rather than the expected (year - 1900) */
    if (lt->tm_year < 70)
        datenum = (long) lt->tm_year + 2000L;
    else
        datenum = (long) lt->tm_year + 1900L;
    /* yyyy --> yyyymm */
    datenum = datenum * 100L + (long) (lt->tm_mon + 1);
    /* yyyymm --> yyyymmdd */
    datenum = datenum * 100L + (long) lt->tm_mday;
    return datenum;
}

long
hhmmss(time_t date)
{
    long timenum;
    struct tm *lt;

    if (date == 0)
        lt = getlt();
    else
        lt = localtime((LOCALTIME_type) &date);

    timenum = lt->tm_hour * 10000L + lt->tm_min * 100L + lt->tm_sec;
    return timenum;
}

char *
yyyymmddhhmmss(time_t date)
{
    long datenum;
    static char datestr[15];
    struct tm *lt;

    if (date == 0)
        lt = getlt();
    else
        lt = localtime((LOCALTIME_type) &date);

    /* just in case somebody's localtime supplies (year % 100)
       rather than the expected (year - 1900) */
    if (lt->tm_year < 70)
        datenum = (long) lt->tm_year + 2000L;
    else
        datenum = (long) lt->tm_year + 1900L;
    Snprintf(datestr, sizeof datestr, "%04ld%02d%02d%02d%02d%02d",
             datenum, lt->tm_mon + 1,
             lt->tm_mday, lt->tm_hour, lt->tm_min, lt->tm_sec);
    //debugpline1("yyyymmddhhmmss() produced date string %s", datestr);
    return datestr;
}

time_t
time_from_yyyymmddhhmmss(char *buf)
{
    int k;
    time_t timeresult = (time_t) 0;
    struct tm t, *lt;
    char *d, *p, y[5], mo[3], md[3], h[3], mi[3], s[3];

    if (buf && strlen(buf) == 14) {
        d = buf;
        p = y; /* year */
        for (k = 0; k < 4; ++k)
            *p++ = *d++;
        *p = '\0';
        p = mo; /* month */
        for (k = 0; k < 2; ++k)
            *p++ = *d++;
        *p = '\0';
        p = md; /* day */
        for (k = 0; k < 2; ++k)
            *p++ = *d++;
        *p = '\0';
        p = h; /* hour */
        for (k = 0; k < 2; ++k)
            *p++ = *d++;
        *p = '\0';
        p = mi; /* minutes */
        for (k = 0; k < 2; ++k)
            *p++ = *d++;
        *p = '\0';
        p = s; /* seconds */
        for (k = 0; k < 2; ++k)
            *p++ = *d++;
        *p = '\0';
        lt = getlt();
        if (lt) {
            t = *lt;
            t.tm_year = atoi(y) - 1900;
            t.tm_mon = atoi(mo) - 1;
            t.tm_mday = atoi(md);
            t.tm_hour = atoi(h);
            t.tm_min = atoi(mi);
            t.tm_sec = atoi(s);
            timeresult = mktime(&t);
        }
        if (timeresult == (time_t) -1)
            ;
#if 0
TODO: set_debugpline1, debugpline1 -> function pointer
            debugpline1("time_from_yyyymmddhhmmss(%s) would have returned -1",
                        buf ? buf : "");
#endif
        else
            return timeresult;
    }
    return (time_t) 0;
}

/*
 * moon period = 29.53058 days ~= 30, year = 365.2422 days
 * days moon phase advances on first day of year compared to preceding year
 *      = 365.2422 - 12*29.53058 ~= 11
 * years in Metonic cycle (time until same phases fall on the same days of
 *      the month) = 18.6 ~= 19
 * moon phase on first day of year (epact) ~= (11*(year%19) + 29) % 30
 *      (29 as initial condition)
 * current phase in days = first day phase + days elapsed in year
 * 6 moons ~= 177 days
 * 177 ~= 8 reported phases * 22
 * + 11/22 for rounding
 */
int
phase_of_the_moon(void) /* 0-7, with 0: new, 4: full */
{
    struct tm *lt = getlt();
    int epact, diy, goldn;

    diy = lt->tm_yday;
    goldn = (lt->tm_year % 19) + 1;
    epact = (11 * goldn + 18) % 30;
    if ((epact == 25 && goldn > 11) || epact == 24)
        epact++;

    return ((((((diy + epact) * 6) + 11) % 177) / 22) & 7);
}

boolean
friday_13th(void)
{
    struct tm *lt = getlt();

    /* tm_wday (day of week; 0==Sunday) == 5 => Friday */
    return (boolean) (lt->tm_wday == 5 && lt->tm_mday == 13);
}

int
rt_night(void)
{
    int hour = getlt()->tm_hour;

    return (hour < 6 || hour > 21);
}

int
night(void)
{
    return (u.uenvirons.tod == TOD_EARLYNIGHT
            || u.uenvirons.tod == TOD_LATENIGHT);
}

int
rt_midnight(void)
{
    return (getlt()->tm_hour == 0);
}

int
midnight(void)
{
    return ((u.uenvirons.tod == TOD_EARLYNIGHT 
                && u.uenvirons.tod_cnt <= 100)
            || (u.uenvirons.tod == TOD_LATENIGHT 
                && u.uenvirons.tod_cnt >= TOD_QUARTER - 100));
}

int
midday(void)
{
    return ((u.uenvirons.tod == TOD_MORNING
                && u.uenvirons.tod_cnt <= 100)
            || (u.uenvirons.tod == TOD_EVENING
                    && u.uenvirons.tod_cnt >= TOD_QUARTER - 100));
}

int
calc_dt_vis(void)
{
    int amt;
    if (night()) return u.nv_range;
    amt = (u.uenvirons.tod == TOD_MORNING) 
            ? (TOD_QUARTER - u.uenvirons.tod_cnt) : u.uenvirons.tod_cnt;
    if (u.uenvirons.curr_weather & WTH_DRIZZLE) amt -= 100;
    if (u.uenvirons.curr_weather & (WTH_RAIN | WTH_ACIDRAIN)) {
         amt -= 200;
    }
    if (u.uenvirons.curr_weather & WTH_ASHES) amt -= 300;
    return max(1, 3 + (amt / 100));
}

struct weather *roll_precip(void)
{
    int i;
    int x = 0;
    int total_prob = 0;
    for (i = 0; i < SIZE(dungeon_precips); i++) {
        x += dungeon_precips[i].prob;
    }
    if (x < 0) x = 2;
    x = rn2(x);
    for (i = 0; i < SIZE(dungeon_precips); i++) {
        total_prob += dungeon_precips[i].prob;
        if (x < total_prob) {
            u.uenvirons.inc_precip = dungeon_precips[i];
            return &dungeon_precips[i];
        }
    }
    panic("Like tears in the rain... (%d %d)", x, total_prob);
}

struct weather *roll_wind(void)
{
    int i;
    int x = 0;
    int total_prob = 0;
    for (i = 0; i < SIZE(dungeon_winds); i++) {
        x += dungeon_winds[i].prob;
    }
    if (x < 0) x = 2;
    x = rn2(x);
    for (i = 0; i < SIZE(dungeon_winds); i++) {
        total_prob += dungeon_winds[i].prob;
        if (x < total_prob) {
            u.uenvirons.inc_wind = dungeon_winds[i];
            return &dungeon_winds[i];
        }
    }
    panic("A black wind blows through you... (%d %d)", x, total_prob);
}

void
doenvirons(void)
{
    u.uenvirons.tod_cnt--;
    u.uenvirons.precip_cnt--;
    u.uenvirons.wind_cnt--;
    u.uenvirons.dt_vis = calc_dt_vis();
    weather_messages();
    if (!u.uenvirons.tod_cnt) {
        u.uenvirons.tod_cnt = TOD_QUARTER;
        u.uenvirons.tod += 1;
        if (u.uenvirons.tod > TOD_LATENIGHT) u.uenvirons.tod = TOD_MORNING;
        timechange_message(FALSE);
    }
    if (!u.uenvirons.precip_cnt) {
        weatherchange_message(TRUE);
        u.uenvirons.curr_weather &= ~u.uenvirons.inc_precip.overwrite;
        u.uenvirons.curr_weather |= u.uenvirons.inc_precip.def;
        u.uenvirons.precip_cnt = rn1(u.uenvirons.inc_precip.timeout, u.uenvirons.inc_precip.timeout);
        roll_precip();
    }
    if (!u.uenvirons.wind_cnt) {
        weatherchange_message(FALSE);
        if (INC_WIND(WTH_TORNADO)) {
            (void) makemon(&mons[PM_TORNADO], 0, 0, NO_MM_FLAGS);
        }
        u.uenvirons.curr_weather &= ~u.uenvirons.inc_wind.overwrite;
        u.uenvirons.curr_weather |= u.uenvirons.inc_wind.def;
        u.uenvirons.wind_cnt = rn1(u.uenvirons.inc_wind.timeout, u.uenvirons.inc_wind.timeout);
        roll_wind();
    }
    weather_effects();
}

staticfn void
weather_effects(void)
{
    int x, y;
    if (!exposed_to_elements(&u.uz)) return;
    /* Rain and acid rain */
    if (IS_RAINING) {
        if (rn2(CURR_WEATHER(WTH_DRIZZLE) ? 2 
                : CURR_WEATHER(WTH_DOWNBURST) ? 8 : 4)) {
            x = rn2(COLNO);
            y = rn2(ROWNO);
            floor_spillage(x, y, CURR_WEATHER(WTH_ACIDRAIN) ? POT_ACID 
                                                          : POT_WATER, 0);
        }
    }
    /* Ash fall */
    if (CURR_WEATHER(WTH_ASHES)) {
        x = rn2(COLNO);
        y = rn2(ROWNO);
        add_coating(x, y, COAT_ASHES, 0);
    }
    /* Hailstones */
    if (CURR_WEATHER(WTH_HAIL)) {
        if (!rn2(4)) {
            You("are pounded by hailstones!");
            losehp(Maybe_Half_Phys(d(1, 4)), "pounded by hailstones",
                NO_KILLER_PREFIX);
        }
    }
    /* Rain of Fire */
    if (CURR_WEATHER(WTH_FIRERAIN)) {
        x = rn2(COLNO);
        y = rn2(ROWNO);
        if (isok(x, y)) {
            clear_heros_fault(create_bonfire(x, y, 1, 1));
        }
    }
    /* Tornados */
    if (CURR_WEATHER(WTH_TORNADO) && !rn2(150)) {
        (void) makemon(&mons[PM_TORNADO], 0, 0, NO_MM_FLAGS);
    }
}

staticfn void
weather_messages(void)
{
    int hallu = Hallucination ? 1 : 0;
    /* Windy winds */
    if (CURR_WEATHER(WTH_BREEZE) && !rn2(200))
        You("feel a %s breeze.",
                (svl.level.flags.temperature == 1) ? "hot"
                : Is_oracle_level(&u.uz) ? "strange"
                : (svl.level.flags.temperature == -1) ? "chill" : "cool");
    if (CURR_WEATHER(WTH_WIND) && !Deaf && !rn2(200))
        You_hear1("the whispering of the wind.");
    if (CURR_WEATHER(WTH_GUST) && !Deaf && !rn2(200))
        pline("The wind is howling.");
    /* Incoming weather messages */
    if (INC_PRECIP(WTH_ACIDRAIN) && !CURR_WEATHER(WTH_ACIDRAIN) && !rn2(200)) {
        static const char *const acidrain_msg[4] = {
            "turn green", "boil",
            "release an acrid smell", "get mean",
        };
        pline("The clouds are starting to %s.", acidrain_msg[rn2(3) + hallu]);
    }
    if (INC_PRECIP(WTH_RAIN) && !CURR_WEATHER(WTH_RAIN) && !rn2(200)) {
        static const char *const incrain_msg[4] = {
            "It smells like rain.", "The clouds are darkening.",
            "A storm begins to roll in.", "Get ready for a shower!",
        };
        pline("%s", incrain_msg[rn2(3) + hallu]);
    }
    if (INC_WIND(WTH_TORNADO) && !CURR_WEATHER(WTH_TORNADO) && !rn2(200)) {
        static const char *const tornado_msg[4] = {
            "The sky is turning orange.", "The temperature suddenly plumets!",
            "The clouds overhead are rotating.", "That tornado is carrying a car!",
        };
        pline("%s", tornado_msg[rn2(3) + hallu]);
    }
}

const char *
tod_string(void)
{
    if (midnight()) return "Midnight";
    if (night()) return "Night";
    if (midday()) return "Midday";
    if (u.uenvirons.tod == TOD_MORNING) return "Morning";
    return "Evening";
}

void
weatherchange_message(boolean rain)
{
    boolean notice_it = exposed_to_elements(&u.uz);
    boolean hear_it = !Deaf && !notice_it;
    if (rain) {
        if (u.uenvirons.curr_weather & u.uenvirons.inc_precip.def)
            return;
        /* Drizzle */
        if (INC_PRECIP(WTH_DRIZZLE)) {
            if (CURR_WEATHER(WTHM_ALL_PRECIPS))
                pline("The rain lessens.");
            else if (hear_it)
                You_hear("dripping stone.");
            else if (notice_it)
                pline("It starts drizzling.");
        }
        /* Rain */
        if (INC_PRECIP(WTH_RAIN) && !IS_RAINING) {
            if (notice_it)
                pline("It starts to rain.");
            else if (hear_it)
                You_hear("rain on the dungeon roof.");
        } else if (!INC_PRECIP(WTHM_RAINS) && IS_RAINING) {
            if (hear_it)
                You("no longer hear the rain.");
            else if (notice_it)
                pline("It stops raining.");
        }
        /* Hail */
        if (INC_PRECIP(WTH_HAIL)) {
            if (notice_it) {
                pline("It starts to hail!");
                stop_occupation();
            } else if (hear_it)
                You_hear("the pounding of hailstones.");
        } else if (!INC_PRECIP(WTH_HAIL) && CURR_WEATHER(WTH_HAIL)) {
            pline("It stops hailing.");
        }
        /* Weird Weather */
        if (INC_PRECIP(WTH_DOWNBURST)) {
            if (hear_it)
                You_hear("pounding rain.");
            else
                pline("The sky opens up! The rain is pounding!");
        }
        if (INC_PRECIP(WTH_ACIDRAIN) && notice_it) {
            if (IS_RAINING) {
                pline("The rain suddenly begins to burn your %s!", body_part(NOSE));
            } else {
                pline("It's raining acid!");
            }
            stop_occupation();
        } else if (INC_PRECIP(WTH_ACIDRAIN)) {
            You_hear("a distant sizzling above you.");
        }
        if (INC_PRECIP(WTH_FIRERAIN) && notice_it) {
            pline("Fire falls from the sky!");
            stop_occupation();
        }
        if (INC_PRECIP(WTH_ASHES) && notice_it) {
            pline("Flakes of ash begin falling from the sky.");
        }
    } else {
        if (u.uenvirons.curr_weather & u.uenvirons.inc_wind.def)
            return;
        if (INC_WIND(WTHM_WINDY)
            && !CURR_WEATHER(WTHM_WINDY)) {
            pline("The wind picks up.");
        }
        if (!INC_WIND(WTHM_WINDY)
            && CURR_WEATHER(WTHM_WINDY)) {
            pline("The wind dies down.");
        }
        /* Twisters */
        if (INC_WIND(WTH_TORNADO) && notice_it) {
            if (Blind)
                pline("The air pressure begins to fluctuate!");
            else
                urgent_pline("You see a funnel cloud touch down!");
            stop_occupation();
        } else if (INC_WIND(WTH_TORNADO) && hear_it) {
            You_hear("a distant sucking noise.");
        }
    }
}

void
timechange_message(boolean new_game)
{
    struct monst *mtmp;
    boolean guards_called = FALSE;
    /* Monster reactions. */
    for (mtmp = fmon; mtmp; mtmp = mtmp->nmon) {
        if (mtmp->mfrozen || !mtmp->mcanmove)
            continue;
        if (u.uenvirons.tod == TOD_EARLYNIGHT
                && !guards_called && is_mercenary(mtmp->data)
                && mtmp->data != &mons[PM_GUARD] && mtmp->mpeaceful
                && in_town(u.ux, u.uy)) {
            guards_called = TRUE;
            if (Deaf && canseemon(mtmp))
                pline_mon(mtmp, "%s waves to you.", Monnam(mtmp));
            else
                verbalize("Sunset come and all's well!");
        } else if (u.uenvirons.tod == TOD_MORNING 
                    && !mtmp->female && mtmp->data->mlet == S_COCKATRICE) {
            if (canseemon(mtmp))
                pline_mon(mtmp, "%s crows at the rising sun.", Monnam(mtmp));
            else
                You_hear("a rooster crowing.");
            wake_nearto(mtmp->mx, mtmp->my, 5 * 5);
        }
    }
    close_shops(TRUE);
    /* Messages */
    if (u.uenvirons.tod == TOD_MORNING) {
        if (Blind && exposed_to_elements(&u.uz))
            You_feel("the warmth of the sun on your %s.", body_part(FACE));
        if (!new_game)
            pline(Hallucination ? "The morning sun has vanquished the horrible night."
                                : "It is morning.");
    } else if (u.uenvirons.tod == TOD_EVENING) {
        if (Blind && exposed_to_elements(&u.uz))
            pline("The sun beats down atop your %s.", body_part(HEAD));
        else pline("It is midday.");
    } else if (u.uenvirons.tod == TOD_EARLYNIGHT) {
        if (Blind && exposed_to_elements(&u.uz))
            pline("The warmth of the sun disappears.");
        else
            pline("The sun has set.");
        /* Moon messages */
        if (flags.moonphase == FULL_MOON && !new_game
            && exposed_to_elements(&u.uz)) {
            pline("The full moon rises overhead.");
        } else if (flags.moonphase == NEW_MOON
                    && Hallucination && !new_game) {
            pline("There is no moon.");
        }
    } else {
        pline("It is midnight.");
    }
    /* object messages? */
    if (uwep && uwep->material == NIGHTIRON){
        if (u.uenvirons.tod == TOD_MORNING) {
            pline("%s wickedly.", Tobjnam(uwep, "gleam"));
        } else if (u.uenvirons.tod == TOD_EARLYNIGHT) {
            pline("%s dully.", Tobjnam(uwep, "glint"));
        }
    }
}

void
harassment_weather(void)
{
    int x = rn2(1000);
    int total_prob = 0;
    for (int i = 0; i < SIZE(harassment_precip); i++) {
        total_prob += harassment_precip[i].prob;
        if (x < total_prob) {
            u.uenvirons.inc_precip = harassment_precip[i];
            return;
        }
    }
}

void
init_environs(void)
{
    u.uenvirons.tod_cnt = TOD_QUARTER;
    roll_precip();
    do {
        roll_wind();
    } while (INC_WIND(WTH_TORNADO));
    roll_wind();
    u.uenvirons.precip_cnt = rn1(1000, 500);
    u.uenvirons.wind_cnt = rn1(500, 500);
    u.uenvirons.tod = TOD_MORNING;
    /* sometimes we start with a little breeze */
    if (!rn2(2))
        u.uenvirons.curr_weather |= WTH_BREEZE;
}

void
weather_choice_menu(void)
{
    winid win;
    anything any;
    menu_item *pick_list = (menu_item *) 0;
    int n, i, j;
    any = cg.zeroany;
    win = create_nhwindow(NHW_MENU);
    start_menu(win, MENU_BEHAVE_STANDARD);
    for (i = 0; i < SIZE(weather_strings); i++) {
        any.a_int = i + 1;
        add_menu(win, &nul_glyphinfo, &any, 0, 0, ATR_NONE, NO_COLOR, weather_strings[i], MENU_ITEMFLAGS_NONE);
    }
    end_menu(win, "Create what kind of weather?");
    n = select_menu(win, PICK_ANY, &pick_list);
    u.uenvirons.inc_precip.def = 0;
    u.uenvirons.inc_precip.overwrite = WTHM_ALL_PRECIPS;
    u.uenvirons.inc_precip.timeout = rn1(500, 500);
    for (j = 0; j < n; ++j) {
        i = pick_list[j].item.a_int - 1;
        u.uenvirons.inc_precip.def |= wizweather_precips[i].def;
    }
    free(pick_list);
    destroy_nhwindow(win);
    u.uenvirons.precip_cnt = 1;
}
/* calendar.c */

