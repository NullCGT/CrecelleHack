/* NetHack 3.7	version.c	$NHDT-Date: 1737622664 2025/01/23 00:57:44 $  $NHDT-Branch: NetHack-3.7 $:$NHDT-Revision: 1.105 $ */
/* Copyright (c) Stichting Mathematisch Centrum, Amsterdam, 1985. */
/*-Copyright (c) Michael Allison, 2018. */
/* NetHack may be freely redistributed.  See license for details. */

#include "hack.h"
#include "dlb.h"

#ifndef OPTIONS_AT_RUNTIME
#define OPTIONS_AT_RUNTIME
#endif

staticfn void insert_rtoption(char *) NONNULLARG1;

/* fill buffer with short version (so caller can avoid including date.h)
 * buf cannot be NULL */
char *
version_string(char *buf, size_t bufsz)
{
    Snprintf(buf, bufsz, "%s",
             ((nomakedefs.version_string && nomakedefs.version_string[0])
              ? nomakedefs.version_string
              /* in case we try to write a paniclog entry after releasing
                 the 'nomakedefs' data */
              : mdlib_version_string(buf, ".")));
    return buf;
}

/* fill and return the given buffer with the long nethack version string */
char *
getversionstring(char *buf, size_t bufsz)
{
    Strcpy(buf, nomakedefs.version_id);

    {
        int c = 0;
#if defined(RUNTIME_PORT_ID)
        char tmpbuf[BUFSZ], *tmp;
#endif
        char *p = eos(buf);
        boolean dotoff = (p > buf && p[-1] == '.');

        if (dotoff)
            --p;
        Strcpy(p, " (");
#if defined(RUNTIME_PORT_ID)
        tmp = get_port_id(tmpbuf);
        if (tmp)
            Snprintf(eos(buf), (bufsz - strlen(buf)) - 1,
                     "%s%s", c++ ? "," : "", tmp);
#endif
        if (nomakedefs.git_sha)
            Snprintf(eos(buf), (bufsz - strlen(buf)) - 1,
                     "%s%s", c++ ? "," : "", nomakedefs.git_sha);
#if (NH_DEVEL_STATUS != NH_STATUS_RELEASED)
        if (nomakedefs.git_branch)
            Snprintf(eos(buf), (bufsz - strlen(buf)) - 1,
                     "%sbranch:%s",
                     c++ ? "," : "", nomakedefs.git_branch);
#endif
        if (nomakedefs.git_prefix)
            Snprintf(eos(buf), (bufsz - strlen(buf)) - 1,
                     "%sprefix:%s",
                     c++ ? "," : "", nomakedefs.git_prefix);
        if (c)
            Snprintf(eos(buf), (bufsz - strlen(buf)) - 1,
                     "%s", ")");
        else /* if nothing has been added, strip " (" back off */
            *p = '\0';
        if (dotoff)
            Snprintf(eos(buf), (bufsz - strlen(buf)) - 1,
                     "%s", ".");
    }
    return buf;
}

/* version info that could be displayed on status lines;
     "<game name> <git branch name> <x.y.z version number>";
   if game name is a prefix of--or same as--branch name, it is omitted
     "<git branch name> <x.y.z version number>";
   after release--or if branch info is unavailable--it will be
     "<game name> <x.y.z version number>";
   game name or branch name or both can be requested via flags */
char *
status_version(char *buf, size_t bufsz, boolean indent)
{
    const char *name = NULL, *altname = NULL, *indentation;
    unsigned vflags = flags.versinfo;
    boolean shownum = ((vflags & VI_NUMBER) != 0),
            showname = ((vflags & VI_NAME) != 0),
            showbranch = ((vflags & VI_BRANCH) != 0);

    /* game's name {variants should use own name, not "CrecelleHack"} */
    if (showname) {
#ifdef VERS_GAME_NAME /* can be set to override default (base of filename) */
        name = VERS_GAME_NAME;
#else
        name = nh_basename(gh.hname, FALSE); /* hname is from xxxmain.c */
#endif
        if (!name || !*name) /* shouldn't happen */
            showname = FALSE;
    }
    /* git branch name, if available */
    if (showbranch) {
#if 1   /*#if (NH_DEVEL_STATUS != NH_STATUS_RELEASED)*/
        altname = nomakedefs.git_branch;
#endif
        if (!altname || !*altname)
            showbranch = FALSE;
    }
    if (showname && showbranch) {
        if (!strncmpi(name, altname, strlen(name)))
            showname = FALSE;
#if 0
        /* note: it's possible for branch name to be a prefix of game name
           but that's unlikely enough that we won't bother with it; having
           branch "crecellehack-3.7" be a superset of game "crecellehack" seems like
           including both is redundant, but having branch "net" be a subset
           of game "crecellehack" doesn't feel that way; optimizing "net" out
           seems like it would be a mistake */
        else if (!strncmpi(altname, name, strlen(altname)))
            showbranch = FALSE;
#endif
    } else if (!showname && !showbranch) {
        /* flags.versinfo could be set to only 'branch' but it might not
           be available */
        shownum = TRUE;
    }

    *buf = '\0';
    indentation = indent ? " " : "";
    if (showname) {
        Snprintf(eos(buf), bufsz - strlen(buf), "%s%s", indentation, name);
        indentation = " "; /* forced separator rather than optional indent */
    }
    if (showbranch) {
        Snprintf(eos(buf), bufsz - strlen(buf), "%s%s", indentation, altname);
        indentation = " ";
    }
    if (shownum) {
        /* x.y.z version number */
        Snprintf(eos(buf), bufsz - strlen(buf), "%s%s", indentation,
                 (nomakedefs.version_string && nomakedefs.version_string[0])
                     ? nomakedefs.version_string
                     : mdlib_version_string(buf, "."));
    }
    return buf;
}

/* the #versionshort command */
int
doversion(void)
{
    char buf[BUFSZ];

    pline("%s", getversionstring(buf, sizeof buf));
    return ECMD_OK;
}

/* the '#version' command; also a choice for '?' */
int
doextversion(void)
{
    int rtcontext = 0;
    const char *rtbuf;
    dlb *f = (dlb *) 0;
    char buf[BUFSZ], *p = 0;
    winid win = create_nhwindow(NHW_TEXT);
    boolean use_dlb = TRUE,
            done_rt = FALSE,
            done_dlb = FALSE,
            prolog;
    /* lua_info[] moved to util/mdlib.c and rendered via do_runtime_info() */

#if defined(OPTIONS_AT_RUNTIME)
    use_dlb = FALSE;
#else
    done_rt = TRUE;
#endif

    /* instead of using ``display_file(OPTIONS_USED,TRUE)'' we handle
       the file manually so we can include dynamic version info */

    (void) getversionstring(buf, sizeof buf);
    /* if extra text (git info) is present, put it on separate line
       but don't wrap on (x86) */
    if (strlen(buf) >= COLNO)
        p = strrchr(buf, '(');
    if (p && p > buf && p[-1] == ' ' && p[1] != 'x')
        p[-1] = '\0';
    else
        p = 0;
    putstr(win, 0, buf);
    if (p) {
        *--p = ' ';
        putstr(win, 0, p);
    }

    if (use_dlb) {
        f = dlb_fopen(OPTIONS_USED, "r");
        if (!f) {
            putstr(win, 0, "");
            Sprintf(buf, "[Configuration '%s' not available?]", OPTIONS_USED);
            putstr(win, 0, buf);
            done_dlb = TRUE;
        }
    }
    /*
     * already inserted above:
     * + outdented program name and version plus build date and time
     * dat/options; display contents with lines prefixed by '-' deleted:
     * - blank-line
     * -     indented program name and version
     *   blank-line
     *   outdented feature header
     * - blank-line
     *       indented feature list
     *       spread over multiple lines
     *   blank-line
     *   outdented windowing header
     * - blank-line
     *       indented windowing choices with
     *       optional second line for default
     * - blank-line
     * - EOF
     */

    prolog = TRUE; /* to skip indented program name */
    for (;;) {
        if (use_dlb && !done_dlb) {
            if (!dlb_fgets(buf, BUFSZ, f)) {
                done_dlb = TRUE;
                continue;
            }
        } else if (!done_rt) {
            if (!(rtbuf = do_runtime_info(&rtcontext))) {
                done_rt = TRUE;
                continue;
            }
            (void) strncpy(buf, rtbuf, BUFSZ - 1);
            buf[BUFSZ - 1] = '\0';
        } else {
            break;
        }
        (void) strip_newline(buf);
        if (strchr(buf, '\t') != 0)
            (void) tabexpand(buf);

        if (*buf && *buf != ' ') {
            /* found outdented header; insert a separator since we'll
               have skipped corresponding blank line inside the file */
            putstr(win, 0, "");
            prolog = FALSE;
        }
        /* skip blank lines and prolog (progame name plus version) */
        if (prolog || !*buf)
            continue;

        if (strchr(buf, ':'))
            insert_rtoption(buf);

        if (*buf)
            putstr(win, 0, buf);
    }
    if (use_dlb)
        (void) dlb_fclose(f);
    display_nhwindow(win, FALSE);
    destroy_nhwindow(win);
    return ECMD_OK;
}

void
early_version_info(boolean pastebuf)
{
    char buf1[BUFSZ], buf2[BUFSZ];
    char *buf, *tmp;

    Snprintf(buf1, sizeof(buf1), "test");
    /* this is early enough that we have to do our own line-splitting */
    getversionstring(buf1, sizeof buf1);
    tmp = strstri(buf1, " ("); /* split at start of version info */
    if (tmp) {
        /* retain one buffer so that it all goes into the paste buffer */
        *tmp++ = '\0';
        Snprintf(buf2, sizeof(buf2), "%s\n%s", buf1, tmp);
        buf = buf2;
    } else {
        buf = buf1;
    }

    raw_printf("%s", buf);

    if (pastebuf) {
#if defined(RUNTIME_PASTEBUF_SUPPORT) && !defined(LIBNH)
        /*
         * Call a platform/port-specific routine to insert the
         * version information into a paste buffer. Useful for
         * easy inclusion in bug reports.
         */
        port_insert_pastebuf(buf);
#else
        raw_printf("%s", "Paste buffer copy is not available.\n");
#endif
    }
}

extern const char regex_id[];

/*
 * makedefs should put the first token into dat/options; we'll substitute
 * the second value for it.  The token must contain at least one colon
 * so that we can spot it, and should not contain spaces so that makedefs
 * won't split it across lines.  Ideally the length should be close to
 * that of the substituted value since we don't do phrase-splitting/line-
 * wrapping when displaying it.
 */
static struct rt_opt {
    const char *token, *value;
} rt_opts[] = {
    { ":PATMATCH:", regex_id },
    { ":LUAVERSION:", (const char *) gl.lua_ver },
    { ":LUACOPYRIGHT:", (const char *) gl.lua_copyright },
};

/*
 * 3.6.0
 * Some optional stuff is no longer available to makedefs because
 * it depends which of several object files got linked into the
 * game image, so we insert those options here.
 */
staticfn void
insert_rtoption(char *buf)
{
    int i;

    if (!gl.lua_ver[0])
        get_lua_version();

    for (i = 0; i < SIZE(rt_opts); ++i) {
        if (strstri(buf, rt_opts[i].token) && *rt_opts[i].value) {
            (void) strsubst(buf, rt_opts[i].token, rt_opts[i].value);
        }
        /* we don't break out of the loop after a match; there might be
           other matches on the same line */
    }
}

#ifdef MICRO
boolean
comp_times(long filetime)
{
    /* BUILD_TIME is constant but might have L suffix rather than UL;
       'filetime' is historically signed but ought to have been unsigned */
    return ((unsigned long) filetime < (unsigned long) nomakedefs.build_time);
}
#endif

boolean
check_version(
    struct version_info *version_data,
    const char *filename,
    boolean complain,
    unsigned long utdflags)
{
    if (!filename) {
#ifdef EXTRA_SANITY_CHECKS
        if (complain)
            impossible("check_version() called with"
                       " 'complain'=True but 'filename'=Null");
#endif
        complain = FALSE; /* 'complain' requires 'filename' for pline("%s") */
    }
    if (
#ifdef VERSION_COMPATIBILITY /* patchlevel.h */
        version_data->incarnation < VERSION_COMPATIBILITY
        || version_data->incarnation > nomakedefs.version_number
#else
        version_data->incarnation != nomakedefs.version_number
#endif
        ) {
        if (complain) {
            pline("Version mismatch for file \"%s\".", filename);
            if (WIN_MESSAGE != WIN_ERR)
                 display_nhwindow(WIN_MESSAGE, TRUE);
        }
        return FALSE;
    } else if (
        (version_data->feature_set & ~nomakedefs.ignored_features)
            != (nomakedefs.version_features & ~nomakedefs.ignored_features)
        || ((utdflags & UTD_SKIP_SANITY1) == 0
             && version_data->entity_count != nomakedefs.version_sanity1)
        || ((utdflags & UTD_CHECKSIZES) != 0
            && version_data->struct_sizes1 != nomakedefs.version_sanity2)
        || ((utdflags & UTD_CHECKSIZES) != 0
            && version_data->struct_sizes2 != nomakedefs.version_sanity3)
        ) {
        if (complain) {
            pline("Configuration incompatibility for file \"%s\".", filename);
            display_nhwindow(WIN_MESSAGE, TRUE);
        }
        return FALSE;
    }
    return TRUE;
}

/* this used to be based on file date and somewhat OS-dependent,
   but now examines the initial part of the file's contents */
boolean
uptodate(NHFILE *nhfp, const char *name, unsigned long utdflags)
{
    ssize_t rlen = 0;
    int cmc = 0, filecmc = 0;
    struct version_info vers_info;
    boolean verbose = name ? TRUE : FALSE;
    char indicator;

    if (nhfp->structlevel) {
        rlen = read(nhfp->fd, (genericptr_t) &indicator, sizeof indicator);
        if (rlen != sizeof indicator)
            return FALSE;
        rlen = read(nhfp->fd, (genericptr_t) &filecmc, sizeof filecmc);
        if (rlen == 0)
            return FALSE;
    }
    if (cmc != filecmc)
        return FALSE;

    rlen = read(nhfp->fd, (genericptr_t) &vers_info, sizeof vers_info);
    minit();                /* ZEROCOMP */
    if (rlen == 0) {
        if (verbose) {
            pline("File \"%s\" is empty?", name);
            if ((utdflags & UTD_WITHOUT_WAITSYNCH_PERFILE) == 0)
                wait_synch();
        }
        return FALSE;
    }

    if (!check_version(&vers_info, name, verbose, utdflags)) {
        if (verbose) {
            if ((utdflags & UTD_WITHOUT_WAITSYNCH_PERFILE) == 0)
                wait_synch();
        }
        return FALSE;
    }
    return TRUE;
}

void
store_formatindicator(NHFILE *nhfp)
{
    char indicate = 'u';
    int cmc = 0;

    if (nhfp->mode & WRITING) {
        if (nhfp->structlevel) {
            indicate = 'h';     /* historical */
            bwrite(nhfp->fd, (genericptr_t) &indicate, sizeof indicate);
            bwrite(nhfp->fd, (genericptr_t) &cmc, sizeof cmc);
        }
    }
}

void
store_version(NHFILE *nhfp)
{
    struct version_info version_data = {
        0UL, 0UL, 0UL, 0UL, 0UL
    };

    /* actual version number */
    version_data.incarnation = nomakedefs.version_number;
    /* bitmask of config settings */
    version_data.feature_set = nomakedefs.version_features;
    /* # of monsters and objects */
    version_data.entity_count  = nomakedefs.version_sanity1;
    /* size of key structs */
    version_data.struct_sizes1 = nomakedefs.version_sanity2;
    /* size of more key structs */
    version_data.struct_sizes2 = nomakedefs.version_sanity3;

    if (nhfp->structlevel) {
        bufoff(nhfp->fd);
        /* bwrite() before bufon() uses plain write() */
        store_formatindicator(nhfp);
        bwrite(nhfp->fd, (genericptr_t) &version_data,
               (unsigned) (sizeof version_data));
        bufon(nhfp->fd);
    }
    return;
}

#ifdef AMIGA
const char amiga_version_string[] = AMIGA_VERSION_STRING;
#endif

unsigned long
get_feature_notice_ver(char *str)
{
    char buf[BUFSZ];
    int ver_maj, ver_min, patch;
    char *istr[3];
    int j = 0;

    if (!str)
        return 0L;
    str = strcpy(buf, str);
    istr[j] = str;
    while (*str) {
        if (*str == '.') {
            *str++ = '\0';
            j++;
            istr[j] = str;
            if (j == 2)
                break;
        } else if (strchr("0123456789", *str) != 0) {
            str++;
        } else
            return 0L;
    }
    if (j != 2)
        return 0L;
    ver_maj = atoi(istr[0]);
    ver_min = atoi(istr[1]);
    patch = atoi(istr[2]);
    return FEATURE_NOTICE_VER(ver_maj, ver_min, patch);
    /* macro from hack.h */
}

unsigned long
get_current_feature_ver(void)
{
    return FEATURE_NOTICE_VER(VERSION_MAJOR, VERSION_MINOR, PATCHLEVEL);
}

/*ARGUSED*/
const char *
copyright_banner_line(int indx)
{

#ifdef VARIANT_BANNER_A
    if (indx == 1)
        return VARIANT_BANNER_A;
#endif
#ifdef VARIANT_BANNER_B
    if (indx == 2)
        return VARIANT_BANNER_B;
#endif
#ifdef COPYRIGHT_BANNER_A
    if (indx == 4)
        return COPYRIGHT_BANNER_A;
#endif
#ifdef COPYRIGHT_BANNER_B
    if (indx == 5)
        return COPYRIGHT_BANNER_B;
#endif

    if (indx == 3)
        return nomakedefs.copyright_banner_c;

#ifdef COPYRIGHT_BANNER_D
    if (indx == 6)
        return COPYRIGHT_BANNER_D;
#endif
    return "";
}

/* called by argcheck(allmain.c) from early_options(sys/xxx/xxxmain.c) */
void
dump_version_info(void)
{
    char buf[BUFSZ];
    const char *hname = gh.hname ? gh.hname : "crecellehack";

    if (strlen(hname) > 33)
        hname = eos(nhStr(hname)) - 33; /* discard const for eos() */
    runtime_info_init();
    Snprintf(buf, sizeof buf, "%-12.33s %08lx %08lx %08lx %08lx %08lx",
             hname,
             nomakedefs.version_number,
             (nomakedefs.version_features & ~nomakedefs.ignored_features),
             nomakedefs.version_sanity1,
             nomakedefs.version_sanity2,
             nomakedefs.version_sanity3);
    raw_print(buf);
    release_runtime_info();
    return;
}

/*version.c*/
