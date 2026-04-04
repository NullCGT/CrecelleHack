/* NetHack 3.7	earlyarg.c	$NHDT-Date: 1771213100 2026/02/15 19:38:20 $  $NHDT-Branch: NetHack-3.7 $:$NHDT-Revision: 1.286 $ */
/* Copyright (c) Robert Patrick Rankin, 2012. */
/* NetHack may be freely redistributed.  See license for details. */

#include "hack.h"

staticfn void debug_fields(char *);
#ifndef NODUMPENUMS
staticfn void dump_enums(void);
#endif

/*
 * Argument processing helpers - for xxmain() to share
 * and call.
 *
 * These should return TRUE if the argument matched,
 * whether the processing of the argument was
 * successful or not.
 *
 * Most of these do their thing, then after returning
 * to xxmain(), the code exits without starting a game.
 *
 */

static const struct early_opt earlyopts[] = {
    { ARG_DEBUG, "debug", 5, TRUE },
    { ARG_VERSION, "version", 4, TRUE },
    { ARG_SHOWPATHS, "showpaths", 8, FALSE },
#ifndef NODUMPENUMS
    { ARG_DUMPENUMS, "dumpenums", 9, FALSE },
#endif
    { ARG_DUMPGLYPHIDS, "dumpglyphids", 12, FALSE },
    { ARG_DUMPMONGEN, "dumpmongen", 10, FALSE },
    { ARG_DUMPWEIGHTS, "dumpweights", 11, FALSE },
    { ARG_DUMPWEAPONS, "dumpweapons", 11, FALSE },
#ifdef WIN32
    { ARG_WINDOWS, "windows", 4, TRUE },
#endif
#if defined(CRASHREPORT)
    { ARG_BIDSHOW, "bidshow", 7, FALSE },
#endif
};

#ifdef WIN32
extern int windows_early_options(const char *);
#endif

/*
 * Returns:
 *    0 = no match
 *    1 = found and skip past this argument
 *    2 = found and trigger immediate exit
 */
int
argcheck(int argc, char *argv[], enum earlyarg e_arg)
{
    int i, idx;
    boolean match = FALSE;
    char *userea = (char *) 0;
    const char *dashdash = "";

    for (idx = 0; idx < SIZE(earlyopts); idx++) {
        if (earlyopts[idx].e == e_arg){
            break;
        }
    }
    if (idx >= SIZE(earlyopts) || argc < 1)
        return 0;

    for (i = 0; i < argc; ++i) {
        if (argv[i][0] != '-')
            continue;
        if (argv[i][1] == '-') {
            userea = &argv[i][2];
            dashdash = "-";
        } else {
            userea = &argv[i][1];
        }
        match = match_optname(userea, earlyopts[idx].name,
                              earlyopts[idx].minlength,
                              earlyopts[idx].valallowed);
        if (match)
            break;
    }

    if (match) {
        const char *extended_opt = strchr(userea, ':');

        if (!extended_opt)
            extended_opt = strchr(userea, '=');
        switch(e_arg) {
        case ARG_DEBUG:
            if (extended_opt) {
                char *cpy_extended_opt;

                cpy_extended_opt = dupstr(extended_opt);
                debug_fields(cpy_extended_opt + 1);
                free((genericptr_t) cpy_extended_opt);
            }
            return 1;
        case ARG_VERSION: {
            boolean insert_into_pastebuf = FALSE;

            if (extended_opt) {
                extended_opt++;
                    /* Deprecated in favor of "copy" - remove no later
                       than  next major version */
                if (match_optname(extended_opt, "paste", 5, FALSE)) {
                    insert_into_pastebuf = TRUE;
                } else if (match_optname(extended_opt, "copy", 4, FALSE)) {
                    insert_into_pastebuf = TRUE;
                } else if (match_optname(extended_opt, "dump", 4, FALSE)) {
                    /* version number plus enabled features and sanity
                       values that the program compares against the same
                       thing recorded in save and bones files to check
                       whether they're being used compatibly */
                    dump_version_info();
                    return 2; /* done */
                } else if (!match_optname(extended_opt, "show", 4, FALSE)) {
                    raw_printf("-%sversion can only be extended with"
                               " -%sversion:copy or :dump or :show.\n",
                               dashdash, dashdash);
                    /* exit after we've reported bad command line argument */
                    return 2;
                }
            }
            early_version_info(insert_into_pastebuf);
            return 2;
        }
        case ARG_SHOWPATHS:
            return 2;
#ifndef NODUMPENUMS
        case ARG_DUMPENUMS:
            dump_enums();
            return 2;
#endif
        case ARG_DUMPGLYPHIDS:
            dump_glyphids();
            return 2;
        case ARG_DUMPMONGEN:
            dump_mongen();
            return 2;
        case ARG_DUMPWEIGHTS:
            dump_weights();
            return 2;
        case ARG_DUMPWEAPONS:
            dump_weapons();
            return 2;
#ifdef CRASHREPORT
        case ARG_BIDSHOW:
            crashreport_bidshow();
            return 2;
#endif
#ifdef WIN32
        case ARG_WINDOWS:
            if (extended_opt) {
                extended_opt++;
                return windows_early_options(extended_opt);
            }
        FALLTHROUGH;
        /*FALLTHRU*/
#endif
        default:
            break;
        }
    };
    return 0;
}

/*
 * These are internal controls to aid developers with
 * testing and debugging particular aspects of the code.
 * They are not player options and the only place they
 * are documented is right here. No gameplay is altered.
 *
 * test             - test whether this parser is working
 * ttystatus        - TTY:
 * immediateflips   - WIN32: turn off display performance
 *                    optimization so that display output
 *                    can be debugged without buffering.
 * fuzzer           - enable fuzzer without debugger intervention.
 */
staticfn void
debug_fields(char *opts)
{
    char *op;
    boolean negated = FALSE;

    while ((op = strchr(opts, ',')) != 0) {
        *op++ = 0;
        /* recurse */
        debug_fields(op);
    }
    if (strlen(opts) > BUFSZ / 2)
        return;


    /* strip leading and trailing white space */
    while (isspace((uchar) *opts))
        opts++;
    op = eos((char *) opts);
    while (--op >= opts && isspace((uchar) *op))
        *op = '\0';

    if (!*opts) {
        /* empty */
        return;
    }
    while ((*opts == '!') || !strncmpi(opts, "no", 2)) {
        if (*opts == '!')
            opts++;
        else
            opts += 2;
        negated = !negated;
    }
    if (match_optname(opts, "test", 4, FALSE))
        iflags.debug.test = negated ? FALSE : TRUE;
#ifdef TTY_GRAPHICS
    if (match_optname(opts, "ttystatus", 9, FALSE))
        iflags.debug.ttystatus = negated ? FALSE : TRUE;
#endif
#ifdef WIN32
    if (match_optname(opts, "immediateflips", 14, FALSE))
        iflags.debug.immediateflips = negated ? FALSE : TRUE;
#endif
    if (match_optname(opts, "fuzzer", 4, FALSE))
        iflags.fuzzerpending = TRUE;
    return;
}

#if !defined(NODUMPENUMS)
/* monsdump[] and objdump[] are also used in utf8map.c */

#define DUMP_ENUMS
#define UNPREFIXED_COUNT (5)
struct enum_dump monsdump[] = {
#include "monsters.h"
    { NUMMONS, "NUMMONS" },
    { NON_PM, "NON_PM" },
    { LOW_PM, "LOW_PM" },
    { HIGH_PM, "HIGH_PM" },
    { SPECIAL_PM, "SPECIAL_PM" }
};
struct enum_dump objdump[] = {
#include "objects.h"
    { NUM_OBJECTS, "NUM_OBJECTS" },
};

#define DUMP_ENUMS_PCHAR
static struct enum_dump defsym_cmap_dump[] = {
#include "defsym.h"
    { MAXPCHARS, "MAXPCHARS" },
};
#undef DUMP_ENUMS_PCHAR

#define DUMP_ENUMS_MONSYMS
static struct enum_dump defsym_mon_syms_dump[] = {
#include "defsym.h"
    { MAXMCLASSES, "MAXMCLASSES" },
};
#undef DUMP_ENUMS_MONSYMS

#define DUMP_ENUMS_MONSYMS_DEFCHAR
static struct enum_dump defsym_mon_defchars_dump[] = {
#include "defsym.h"
};
#undef DUMP_ENUMS_MONSYMS_DEFCHAR

#define DUMP_ENUMS_OBJCLASS_DEFCHARS
static struct enum_dump objclass_defchars_dump[] = {
#include "defsym.h"
};
#undef DUMP_ENUMS_OBJCLASS_DEFCHARS

#define DUMP_ENUMS_OBJCLASS_CLASSES
static struct enum_dump objclass_classes_dump[] = {
#include "defsym.h"
    { MAXOCLASSES, "MAXOCLASSES" },
};
#undef DUMP_ENUMS_OBJCLASS_CLASSES

#define DUMP_ENUMS_OBJCLASS_SYMS
static struct enum_dump objclass_syms_dump[] = {
#include "defsym.h"
};
#undef DUMP_ENUMS_OBJCLASS_SYMS

#define DUMP_ARTI_ENUM
static struct enum_dump arti_enum_dump[] = {
#include "artilist.h"
    { AFTER_LAST_ARTIFACT, "AFTER_LAST_ARTIFACT" }
};
#undef DUMP_ARTI_ENUM

/* the enums are not part of hack.h for this one */
#define DUMP_MCASTU_ENUM1
enum mcast_dumpenum_spells {
    #include "mcastu.h"
};
#undef DUMP_MCASTU_ENUM1

#define DUMP_MCASTU_ENUM2
static struct enum_dump mcastu_enum_dump[] = {
#include "mcastu.h"
};
#undef DUMP_MCASTU_ENUM2

#undef DUMP_ENUMS


#ifndef NODUMPENUMS

staticfn void
dump_enums(void)
{
    enum enum_dumps {
        monsters_enum,
        objects_enum,
        objects_misc_enum,
        defsym_cmap_enum,
        defsym_mon_syms_enum,
        defsym_mon_defchars_enum,
        objclass_defchars_enum,
        objclass_classes_enum,
        objclass_syms_enum,
        arti_enum,
	mcastu_enum,
        NUM_ENUM_DUMPS
    };

#define dump_om(om) { om, #om }
    static const struct enum_dump omdump[] = {
        dump_om(LAST_GENERIC),
        dump_om(OBJCLASS_HACK),
        dump_om(FIRST_OBJECT),
        dump_om(FIRST_AMULET),
        dump_om(LAST_AMULET),
        dump_om(FIRST_SPELL),
        dump_om(LAST_SPELL),
        dump_om(MAXSPELL),
        dump_om(FIRST_REAL_GEM),
        dump_om(LAST_REAL_GEM),
        dump_om(FIRST_GLASS_GEM),
        dump_om(LAST_GLASS_GEM),
        dump_om(NUM_REAL_GEMS),
        dump_om(NUM_GLASS_GEMS),
        dump_om(MAX_GLYPH),
    };
#undef dump_om

    static const struct enum_dump *const ed[NUM_ENUM_DUMPS] = {
        monsdump, objdump, omdump,
        defsym_cmap_dump, defsym_mon_syms_dump,
        defsym_mon_defchars_dump,
        objclass_defchars_dump,
        objclass_classes_dump,
        objclass_syms_dump,
        arti_enum_dump,
	mcastu_enum_dump,
    };

    static const struct de_params {
        const char *const title;
        const char *const pfx;
        int unprefixed_count;
        int dumpflgs;  /* 0 = dump numerically only, 1 = add 'char' comment */
        int szd;
    } edmp[NUM_ENUM_DUMPS] = {
        { "monnums", "PM_", UNPREFIXED_COUNT, 0, SIZE(monsdump) },
        { "objects_nums", "", 1, 0, SIZE(objdump) },
        { "misc_object_nums", "", 1, 0, SIZE(omdump) },
        { "cmap_symbols", "", 1, 0, SIZE(defsym_cmap_dump) },
        { "mon_syms", "", 1, 0, SIZE(defsym_mon_syms_dump) },
        { "mon_defchars", "", 1, 1, SIZE(defsym_mon_defchars_dump) },
        { "objclass_defchars", "", 1, 1, SIZE(objclass_defchars_dump) },
        { "objclass_classes", "", 1, 0, SIZE(objclass_classes_dump) },
        { "objclass_syms", "", 1, 0, SIZE(objclass_syms_dump) },
        { "artifacts_nums", "", 1, 0, SIZE(arti_enum_dump) },
        { "mcast_spells", "MCAST_", 0, 0, SIZE(mcastu_enum_dump) },
    };

    const char *nmprefix;
    int i, j, nmwidth;
    char comment[BUFSZ];

    for (i = 0; i < NUM_ENUM_DUMPS; ++ i) {
        raw_printf("enum %s = {", edmp[i].title);
        for (j = 0; j < edmp[i].szd; ++j) {
            nmprefix = (j >= edmp[i].szd - edmp[i].unprefixed_count)
                           ? "" : edmp[i].pfx; /* "" or "PM_" */
            nmwidth = 27 - (int) strlen(nmprefix); /* 27 or 24 */
            if (edmp[i].dumpflgs > 0) {
                Snprintf(comment, sizeof comment,
                         "    /* '%c' */",
                         (ed[i][j].val >= 32 && ed[i][j].val <= 126)
                         ? ed[i][j].val : ' ');
            } else {
                comment[0] = '\0';
            }
            raw_printf("    %s%*s = %3d,%s",
                       nmprefix, -nmwidth,
                       ed[i][j].nm, ed[i][j].val,
                       comment);
       }
        raw_print("};");
        raw_print("");
    }
    raw_print("");
}
#undef UNPREFIXED_COUNT
#endif /* NODUMPENUMS */

void
dump_glyphids(void)
{
    dump_all_glyphids(stdout);
}
#endif /* !NODUMPENUMS */

/*allmain.c*/
