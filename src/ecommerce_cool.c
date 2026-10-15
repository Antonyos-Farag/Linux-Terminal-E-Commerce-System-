/*
 * Terminal Commerce — Cool Edition
 * ================================
 * An educational C project: the same learning goals as the original
 * `ecommerce.c` (do…while, switch, if/else, scanf + getchar, system()
 * and shell commands, file operations, permissions, links, find),
 * wrapped in a themed terminal UI:
 *
 *   - ANSI colors + box-drawing menus (automatically disabled when the
 *     output is not a terminal, e.g. piped to a file)
 *   - animated boot sequence, progress bars and action spinners
 *   - ASCII-art banner with a rainbow gradient + typewriter tagline
 *   - real shopping touches: order IDs, a printed receipt, confetti,
 *     star-rated feedback
 *   - runtime base path instead of a hard-coded one:
 *         ECOMMERCE_HOME env var  ->  $HOME/ecommerce  ->  /tmp/ecommerce
 *     (the data tree is auto-created on first run)
 *
 * Build:   make            (or: gcc -Wall -Wextra -Wpedantic -std=c11 -O2)
 * Run:     make run-cool
 */

#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <errno.h>

/* Compile-time override:  make CFLAGS="-Wall -std=c11 -DBASE_PATH=/tmp/ec" */
#ifndef BASE_PATH
#define BASE_PATH ""
#endif

#define PRODUCT_NAME  "Retro Mech Keyboard"
#define PRODUCT_PRICE "$89.99"

/* ------------------------------------------------------------------ */
/*  ANSI palette                                                       */
/* ------------------------------------------------------------------ */

static int t_on;   /* colors/animations enabled (stdout+stdin are a TTY) */
static int t_256;  /* 256-color support (used for the gradient + confetti) */

/* Color accessors: return the ANSI code, or "" when colors are off, so
 * every printf can use them unconditionally (plain mode stays clean). */
static inline const char *RED(void)     { return t_on ? "\033[31m" : ""; }
static inline const char *GREEN(void)   { return t_on ? "\033[32m" : ""; }
static inline const char *YELLOW(void)  { return t_on ? "\033[33m" : ""; }
static inline const char *BLUE(void)    { return t_on ? "\033[34m" : ""; }
static inline const char *MAGENTA(void) { return t_on ? "\033[35m" : ""; }
static inline const char *CYAN(void)    { return t_on ? "\033[36m" : ""; }
static inline const char *WHITE(void)   { return t_on ? "\033[97m" : ""; }
static inline const char *GRAY(void)    { return t_on ? "\033[90m" : ""; }
static inline const char *BOLD(void)    { return t_on ? "\033[1m" : ""; }
static inline const char *DIM(void)     { return t_on ? "\033[2m" : ""; }
static inline const char *RESET(void)   { return t_on ? "\033[0m" : ""; }

/* 8-color rainbow for the banner (used only when t_on is true). */
static const char *PAL8[] = {
    "\033[31m", "\033[33m", "\033[32m", "\033[36m", "\033[34m", "\033[35m", "\033[97m"
};
static const int PAL256[] = { 196, 202, 208, 214, 220, 118, 51, 46, 45, 201 };

/* ------------------------------------------------------------------ */
/*  Small utilities                                                    */
/* ------------------------------------------------------------------ */

static void sleep_ms(int ms)
{
    struct timespec ts;
    ts.tv_sec = ms / 1000;
    ts.tv_nsec = (long)(ms % 1000) * 1000000L;
    nanosleep(&ts, NULL);
}

static void clear_screen(void)
{
    if (t_on)
        printf("\033[H\033[2J");
}

static void init_term(void)
{
    const char *term = getenv("TERM");
    const char *cterm = getenv("COLORTERM");

    t_on = isatty(1) && isatty(0) && term && strcmp(term, "dumb") != 0;
    t_256 = t_on &&
        ((cterm && (strstr(cterm, "truecolor") || strstr(cterm, "24bit"))) ||
         strstr(term, "256") || strstr(term, "xterm") || strstr(term, "screen") ||
         strstr(term, "tmux") || strstr(term, "alacritty") || strstr(term, "kitty"));
}

/* The store's data directory. Chosen once at startup:
 *   1. BASE_PATH (compile-time)      2. $ECOMMERCE_HOME
 *   3. $HOME/ecommerce               4. /tmp/ecommerce
 */
static char g_base[512];

static void init_base_path(void)
{
    const char *env = getenv("ECOMMERCE_HOME");
    const char *home = getenv("HOME");
    char setup[8192];

    if (BASE_PATH[0] != '\0')
        snprintf(g_base, sizeof g_base, "%s", BASE_PATH);
    else if (env && *env && strlen(env) < 512)
        snprintf(g_base, sizeof g_base, "%s", env);
    else if (home && *home && strlen(home) < 490)
        snprintf(g_base, sizeof g_base, "%s/ecommerce", home);
    else
        snprintf(g_base, sizeof g_base, "/tmp/ecommerce");

    /* Auto-create the demo tree (idempotent) so `make setup-dirs` is
     * optional — first run just works. */
    snprintf(setup, sizeof setup,
             "mkdir -p %s/admin %s/sellers %s/customers 2>/dev/null; "
             "[ -f %s/admin/logs.txt ] || touch %s/admin/logs.txt; "
             "[ -f %s/sellers/product1.txt ] || "
             "printf 'Product: %s\\nPrice: %s\\nStock: 42\\nCondition: New\\n' '%s' '%s' > %s/sellers/product1.txt",
             g_base, g_base, g_base,
             g_base, g_base, g_base,
             PRODUCT_NAME, PRODUCT_PRICE, PRODUCT_NAME, PRODUCT_PRICE, g_base);
    (void)system(setup);
}

/* Print the base path with a leading ~ when it lives under $HOME. */
static void disp_path(char *out, size_t n)
{
    const char *home = getenv("HOME");
    if (home && strncmp(g_base, home, strlen(home)) == 0)
        snprintf(out, n, "~%s", g_base + strlen(home));
    else
        snprintf(out, n, "%s", g_base);
}

/* ------------------------------------------------------------------ */
/*  Decorations: banner, typewriter, progress, spinner, confetti       */
/* ------------------------------------------------------------------ */

/* Big ANSI-shadow ASCII art (6 rows each). */
static const char *ART_TERMINAL[6] = {
    "████████╗███████╗██████╗ ███╗   ███╗██╗███╗   ██╗ █████╗ ██╗     ",
    "╚══██══╝██╔════╝██╔══██╗████╗ ████║██║████╗  ██║██╔══██╗██║     ",
    "   ██║   █████╗  ██████╔╝██╔████╔██║██║██╔██╗ ██║███████║██║     ",
    "   ██║   ██╔══╝  ██╔══██╗██║╚██╔╝██║██║██║╚██╗██║██╔══██║██║     ",
    "   ██║   ███████╗██║  ██║██║ ╚═╝ ██║██║██║ ╚████║██║  ██║███████╗",
    "   ╚═╝   ╚══════╝╚═╝  ╚═╝╚═╝     ╚═╝═╝╚═╝  ╚═══╝╚═╝  ╚═╝╚══════╝"
};
static const char *ART_COMMERCE[6] = {
    " ██████╗ ██████╗ ███╗   ███╗███╗   ███╗███████╗██████╗  ██████╗███████╗",
    "██╔════╝██╔═══██╗████╗ ████║████╗ ████║██╔════╝██╔══██╗██╔════╝██╔════╝",
    "██║     ██║   ██║██╔████╔██║██╔████╔██║█████╗  ██████╔╝██║     █████╗  ",
    "██║     ██║   ██║██║╚██╔╝██║██║╚██╔╝██║██╔══╝  ██╔══██╗██║     ██╔══╝  ",
    "╚██████╗╚██████╔╝██║ ╚═╝ ██║██║ ╚═╝ ██║███████╗██║  ██║╚██████╗███████╗",
    " ╚═════╝ ╚═════╝ ╚═╝     ╚═╝╚═╝     ╚═╝╚══════╝╚═╝  ╚═╝ ╚═════╝╚══════╝"
};

/* Paint one art row; each non-space character gets a rainbow hue.
 * The row is UTF-8 (box-drawing chars are 3 bytes) — we must color
 * whole characters, not bytes. */
static void paint_row(const char *row, int shift)
{
    int x = 0;  /* character index (drives the hue) */
    int i = 0;  /* byte index */
    while (row[i] != '\0') {
        unsigned char c = (unsigned char)row[i];
        int len;
        if (c == ' ') {
            putchar(' ');
            i++;
            continue;
        }
        if (c >= 0xF0)
            len = 4;
        else if (c >= 0xE0)
            len = 3;
        else if (c >= 0xC0)
            len = 2;
        else
            len = 1;
        if (t_256)
            printf("\033[38;5;%dm", PAL256[(x + shift) % 10]);
        else if (t_on)
            printf("%s", PAL8[(x + shift) % 7]);
        fwrite(row + i, 1, (size_t)len, stdout);
        i += len;
        x++;
    }
    if (t_on)
        printf("%s", RESET());
    putchar('\n');
}

static void print_banner(void)
{
    int r;
    printf("\n");
    for (r = 0; r < 6; r++)
        paint_row(ART_TERMINAL[r], r * 3);
    printf("\n");
    for (r = 0; r < 6; r++)
        paint_row(ART_COMMERCE[r], 5 + r * 3);
    printf("\n");
}

static void typewriter(const char *s)
{
    if (!t_on) {
        printf("%s\n", s);
        return;
    }
    printf("%s", DIM());
    for (; *s; s++) {
        putchar(*s);
        fflush(stdout);
        sleep_ms(13);
    }
    printf("%s\n", RESET());
}

/* Animated progress bar, finished with a green "ok". */
static void progress(const char *label, int steps)
{
    enum { W = 22 };
    static const char full[] = "██████████████████████";
    int i, j;

    if (!t_on)
        return;

    for (i = 1; i <= steps; i++) {
        int filled = (W * i) / steps;
        printf("\r  %s[", GREEN());
        for (j = 0; j < W; j++)
            fputs(j < filled ? "█" : "░", stdout);
        printf("%s] %3d%%  %s%s%s", RESET(), (100 * i) / steps, GRAY(), label, RESET());
        fflush(stdout);
        sleep_ms(26);
    }
    printf("\r  %s[%s]%s 100%%  %s%s%s  %s✓ ok%s\n",
           GREEN(), full, RESET(), GRAY(), label, RESET(), GREEN(), RESET());
}

static void boot_sequence(void)
{
    char path[600];
    char label[700];

    if (!t_on)
        return;
    clear_screen();
    printf("  %s●%s %sterminal commerce%s %sv2.0%s\n\n",
           GREEN(), RESET(), BOLD(), RESET(), GRAY(), RESET());
    disp_path(path, sizeof path);
    snprintf(label, sizeof label, "mounting %s", path);
    progress(label, 7);
    progress("loading shell bridge", 6);
    progress("painting terminal theme", 8);
}

/* Braille spinner while an action runs. */
static void spin(const char *msg, int ms)
{
    static const char *frames[] = { "⠋", "⠙", "", "⠸", "⠼", "⠴", "", "⠧" };
    int f = 0;

    if (!t_on)
        return;
    while (ms > 0) {
        int step = ms > 55 ? 55 : ms;
        printf("\r  %s%s%s%s  %s", CYAN(), BOLD(), frames[f], RESET(), msg);
        fflush(stdout);
        sleep_ms(step);
        ms -= step;
        f = (f + 1) % 8;
    }
    printf("\r\033[K");
}

/*
 * Run a shell command with a short spinner, then report success/failure.
 * Returns 1 on success, 0 on failure (educational: system() + WIFEXITED).
 */
static int run_cmd(const char *cmd, const char *verb)
{
    int rc;

    spin(verb, 450);
    rc = system(cmd);
    int ok = (rc == 0) || (rc > 0 && WIFEXITED(rc) && WEXITSTATUS(rc) == 0);

    if (ok)
        printf("  %s✓%s  %s%s%s\n", GREEN(), RESET(), DIM(), verb, RESET());
    else
        printf("  %s✗%s  %s%s%s\n", RED(), RESET(), DIM(), verb, RESET());
    return ok;
}

/* A little confetti burst after a purchase. */
static void confetti(void)
{
    static const char *glyphs[] = { "*", "+", "x", "•", "✦", "✧", "·", "o" };
    static const int colors[] = { 196, 208, 220, 46, 51, 141, 201, 213, 198, 226 };
    int r, c;

    if (!t_256)
        return;
    printf("\n");
    for (r = 0; r < 3; r++) {
        for (c = 0; c < 44; c++) {
            if (rand() % 4 == 0)
                putchar(' ');
            else
                printf("\033[38;5;%dm%s\033[0m",
                       colors[rand() % 10], glyphs[rand() % 8]);
        }
        putchar('\n');
        fflush(stdout);
        sleep_ms(60);
    }
    printf("  %s🎉 Order placed — thanks for shopping!%s\n\n", BOLD(), RESET());
}

/* ------------------------------------------------------------------ */
/*  Menu chrome: status line, header box, items, prompt, input        */
/* ------------------------------------------------------------------ */

static void status_line(const char *role, const char *role_color)
{
    char ts[64];
    char path[4096];
    time_t t = time(NULL);
    struct tm lt;

    localtime_r(&t, &lt);
    strftime(ts, sizeof ts, "%a %b %d %H:%M:%S", &lt);
    disp_path(path, sizeof path);
    printf("  %s%s%s  %s·%s  %srole: %s%s%s  %s·%s  %s%s%s\n\n",
           GRAY(), path, RESET(),
           GRAY(), RESET(),
           GRAY(), role_color, role, RESET(),
           GRAY(), RESET(),
           GRAY(), ts, RESET());
}

/* Header box, e.g.:
 *   ╭────────────────────────────╮
 *   │  ADMIN CONSOLE      ops   │
 *   ╰────────────────────────────╯
 */
static void menu_box(const char *title, const char *tag)
{
    enum { W = 46 };
    static const char bar[] = "──────────────────────────────────────────────";
    int pad = W - (int)strlen(title) - (int)strlen(tag) - 3;
    if (pad < 1)
        pad = 1;

    printf("  %s╭%s%s\n", MAGENTA(), bar, RESET());
    printf("  %s│%s  %s%s%s%*s%s%s%s %s│%s\n",
           MAGENTA(), RESET(), BOLD(), WHITE(), title,
           pad, "", DIM(), tag, RESET(), MAGENTA(), RESET());
    printf("  %s╰%s╯%s\n\n", MAGENTA(), bar, RESET());
}

static void menu_item(int n, const char *icon, const char *label, const char *hint)
{
    printf("   %s%s[%d]%s  %s  %s%-20s  %s%s%s\n",
           CYAN(), BOLD(), n, RESET(),
           icon, WHITE(), label,
           GRAY(), hint, RESET());
}

static void menu_exit(const char *text)
{
    printf("   %s[0]%s   %s%s%s\n", RED(), RESET(), DIM(), text, RESET());
}

static void prompt(int max)
{
    printf("\n  %s%s➜%s %schoose 0..%d%s: ",
           CYAN(), BOLD(), RESET(), DIM(), max, RESET());
    fflush(stdout);
}

/*
 * Read an integer in [0..max], like the original's scanf + getchar
 * pattern, but friendly: rejects out-of-range input and, importantly,
 * exits cleanly on EOF instead of looping forever (piped input).
 */
static int read_int(int max)
{
    int c;
    for (;;) {
        if (scanf("%d", &c) != 1) {
            int ch = getchar();
            while (ch != '\n' && ch != EOF)
                ch = getchar();
            if (ch == EOF) {
                putchar('\n');
                return 0;   /* input exhausted — leave gracefully */
            }
            printf("  %s✗%s  not a number — pick 0..%d\n", RED(), RESET(), max);
            continue;
        }
        while (getchar() != '\n')
            ;
        if (c >= 0 && c <= max)
            return c;
        printf("  %s✗%s  out of range — pick 0..%d\n", YELLOW(), RESET(), max);
    }
}

/* Read an integer in [lo..hi] (used for the feedback rating). */
static int read_range(int lo, int hi)
{
    int c;
    for (;;) {
        if (scanf("%d", &c) != 1) {
            int ch = getchar();
            while (ch != '\n' && ch != EOF)
                ch = getchar();
            if (ch == EOF)
                return lo;
            printf("  %s✗%s  not a number — pick %d..%d\n", RED(), RESET(), lo, hi);
            continue;
        }
        while (getchar() != '\n')
            ;
        if (c >= lo && c <= hi)
            return c;
        printf("  %s✗%s  out of range — pick %d..%d\n", YELLOW(), RESET(), lo, hi);
    }
}

/* ------------------------------------------------------------------ */
/*  Receipt                                                            */
/* ------------------------------------------------------------------ */

static void center_c(int w, const char *color, const char *s)
{
    int len = (int)strlen(s);
    int pad = (w - len) / 2;
    if (pad < 0)
        pad = 0;
    printf("%*s%s%s%s\n", pad, "", color, s, RESET());
}

static void dotted_line(const char *left, const char *right)
{
    enum { W = 38 };
    char dots[128];
    int d = W - (int)strlen(left) - (int)strlen(right);
    int i;
    if (d < 2)
        d = 2;
    for (i = 0; i < d; i++)
        dots[i] = '.';
    dots[d] = '\0';
    printf("%s%s%s  %s%s%s%s\n",
           GRAY(), left, RESET(), dots, BOLD(), right, RESET());
}

static void receipt(const char *order, const char *when)
{
    enum { W = 38 };
    static const char bar[] = "──────────────────────────────────────";

    printf("\n");
    center_c(W, CYAN(), "TERMINAL COMMERCE");
    center_c(W, GRAY(), "official receipt");
    printf("  %s%s%s\n", GRAY(), bar, RESET());
    printf("  %s%s%s  %s·%s  %s%s%s\n",
           BOLD(), order, RESET(), GRAY(), RESET(), GRAY(), when, RESET());
    printf("  %s%s%s\n", GRAY(), bar, RESET());
    printf("  %s%s%s\n", WHITE(), PRODUCT_NAME, RESET());
    dotted_line("  x1", PRODUCT_PRICE);
    printf("  %s%s%s\n", GRAY(), bar, RESET());
    dotted_line("  TOTAL", PRODUCT_PRICE);
    printf("  %s%s%s\n", GRAY(), bar, RESET());
    center_c(W, GRAY(), "**** thank you ****");
    printf("\n");
}

/* ------------------------------------------------------------------ */
/*  ADMIN menu                                                         */
/* ------------------------------------------------------------------ */

static void admin_menu(void)
{
    char cmd[8192];
    int choice;

    do {
        status_line("admin", MAGENTA());
        menu_box("ADMIN CONSOLE", "ops & file magic");
        menu_item(1, "📄", "List files", "ls -la the store root");
        menu_item(2, "🔒", "Change permissions", "chmod admin dir (700/755)");
        menu_item(3, "📁", "Create/delete directory", "mkdir / rmdir");
        menu_item(4, "🔗", "Create symbolic link", "ln -s sellers link");
        menu_item(5, "📋", "Copy files", "cp sellers -> admin");
        menu_item(6, "📝", "Update logs", "append timestamped entry");
        menu_item(7, "⌨️", "Create alias", "listfiles in ~/.bashrc");
        menu_item(8, "🔎", "Find files", "find product1.txt");
        menu_item(9, "👀", "View file content", "cat / head / tail");
        menu_exit("exit the admin console");
        prompt(9);
        choice = read_int(9);

        switch (choice) {
        case 1: /* list files */
            snprintf(cmd, sizeof cmd, "ls -la %s", g_base);
            run_cmd(cmd, "listing files");
            break;
        case 2: { /* change permissions */
            int sub;
            printf("   %s[1]%s  🔒  %schmod 700%s — owner only\n",
                   CYAN(), BOLD(), WHITE(), RESET());
            printf("   %s[2]%s  🔓  %schmod 755%s — open to everyone\n",
                   CYAN(), BOLD(), WHITE(), RESET());
            prompt(2);
            sub = read_int(2);
            if (sub == 1) {
                snprintf(cmd, sizeof cmd, "chmod 700 %s/admin", g_base);
                run_cmd(cmd, "locking admin dir (700)");
                printf("  %sheads-up: the admin dir is now owner-only —\n"
                       "  option 2 → 2 opens it again (755)%s\n",
                       DIM(), RESET());
            } else if (sub == 2) {
                snprintf(cmd, sizeof cmd, "chmod 755 %s/admin", g_base);
                run_cmd(cmd, "opening admin dir (755)");
            } else {
                printf("  %s✗%s  invalid choice\n", RED(), RESET());
            }
            break;
        }
        case 3: { /* create/delete directories */
            int sub;
            printf("   %s[1]%s  📁  %smkdir%s  admin/new_directory\n",
                   CYAN(), BOLD(), WHITE(), RESET());
            printf("   %s[2]%s  🗑️  %srmdir%s  admin/new_directory\n",
                   CYAN(), BOLD(), WHITE(), RESET());
            prompt(2);
            sub = read_int(2);
            if (sub == 1) {
                snprintf(cmd, sizeof cmd, "mkdir -p %s/admin/new_directory", g_base);
                run_cmd(cmd, "creating admin/new_directory");
            } else if (sub == 2) {
                snprintf(cmd, sizeof cmd, "rmdir %s/admin/new_directory", g_base);
                run_cmd(cmd, "deleting admin/new_directory");
            } else {
                printf("  %s✗%s  invalid choice\n", RED(), RESET());
            }
            break;
        }
        case 4: /* create symbolic link (-f: re-runnable) */
            snprintf(cmd, sizeof cmd, "ln -sf %s/sellers %s/admin/sellers_link",
                     g_base, g_base);
            run_cmd(cmd, "linking admin/sellers_link -> sellers");
            break;
        case 5: /* copy files */
            snprintf(cmd, sizeof cmd, "cp %s/sellers/product1.txt %s/admin/",
                     g_base, g_base);
            run_cmd(cmd, "copying product1.txt into admin/");
            break;
        case 6: /* update logs + show the tail */
            snprintf(cmd, sizeof cmd,
                     "printf \"[$(date)] admin: state snapshot recorded\\n\" >> %s/admin/logs.txt && tail -n 5 %s/admin/logs.txt",
                     g_base, g_base);
            run_cmd(cmd, "appending to admin/logs.txt");
            break;
        case 7: /* create alias (guarded: no duplicate lines) */
            snprintf(cmd, sizeof cmd,
                     "grep -qxF \"alias listfiles='ls -la %s'\" ~/.bashrc 2>/dev/null || "
                     "printf \"alias listfiles='ls -la %s'\\n\" >> ~/.bashrc; "
                     "grep 'alias listfiles' ~/.bashrc 2>/dev/null || echo '(no alias line in ~/.bashrc yet)'",
                     g_base, g_base);
            if (run_cmd(cmd, "creating alias: listfiles"))
                printf("  %s(run 'source ~/.bashrc' to activate it)%s\n",
                       DIM(), RESET());
            break;
        case 8: /* find files */
            snprintf(cmd, sizeof cmd, "find %s -name 'product1.txt'", g_base);
            run_cmd(cmd, "finding product1.txt");
            break;
        case 9: { /* view file content */
            int sub;
            printf("   %s[1]%s  %scat%s   whole log\n", CYAN(), BOLD(), WHITE(), RESET());
            printf("   %s[2]%s  %shead%s  first 10 lines\n", CYAN(), BOLD(), WHITE(), RESET());
            printf("   %s[3]%s  %stail%s  last 10 lines\n", CYAN(), BOLD(), WHITE(), RESET());
            prompt(3);
            sub = read_int(3);
            if (sub == 1)
                snprintf(cmd, sizeof cmd, "cat %s/admin/logs.txt", g_base);
            else if (sub == 2)
                snprintf(cmd, sizeof cmd, "head -n 10 %s/admin/logs.txt", g_base);
            else if (sub == 3)
                snprintf(cmd, sizeof cmd, "tail -n 10 %s/admin/logs.txt", g_base);
            else {
                printf("  %s✗%s  invalid choice\n", RED(), RESET());
                break;
            }
            run_cmd(cmd, "viewing admin/logs.txt");
            break;
        }
        case 0:
            break;
        default:
            printf("  %s✗%s  invalid choice\n", RED(), RESET());
        }
    } while (choice != 0);
}

/* ------------------------------------------------------------------ */
/*  SELLER menu                                                        */
/* ------------------------------------------------------------------ */

static void seller_menu(void)
{
    char cmd[8192];
    int choice;

    do {
        status_line("seller", YELLOW());
        menu_box("SELLER SHOP", "your storefront");
        menu_item(1, "📦", "List my products", "cat product*.txt");
        menu_item(2, "✏️", "Create product file", "write the product card");
        menu_item(3, "📋", "Copy product", "cp product1 -> copy");
        menu_item(4, "🚚", "Move product", "mv copy -> moved");
        menu_item(5, "👀", "View product", "cat / head / tail");
        menu_item(6, "🔎", "Find product", "by name or content");
        menu_item(7, "⌨️", "Use aliases", "run listfiles");
        menu_exit("exit the seller shop");
        prompt(7);
        choice = read_int(7);

        switch (choice) {
        case 1: /* list my products */
            snprintf(cmd, sizeof cmd,
                     "cat %s/sellers/product*.txt 2>/dev/null || echo '(no products yet — create one!)'",
                     g_base);
            run_cmd(cmd, "listing your products");
            break;
        case 2: /* create product file */
            snprintf(cmd, sizeof cmd,
                     "printf 'Product: %s\\nPrice: %s\\nStock: 42\\nCondition: New\\n' '%s' '%s' > %s/sellers/product1.txt",
                     PRODUCT_NAME, PRODUCT_PRICE, PRODUCT_NAME, PRODUCT_PRICE, g_base);
            run_cmd(cmd, "writing sellers/product1.txt");
            break;
        case 3: /* copy product */
            snprintf(cmd, sizeof cmd,
                     "cp %s/sellers/product1.txt %s/sellers/product1_copy.txt",
                     g_base, g_base);
            run_cmd(cmd, "copying product1 -> product1_copy");
            break;
        case 4: /* move product */
            snprintf(cmd, sizeof cmd,
                     "mv %s/sellers/product1_copy.txt %s/sellers/moved_product1.txt",
                     g_base, g_base);
            run_cmd(cmd, "moving product1_copy -> moved_product1");
            break;
        case 5: { /* view product */
            int sub;
            printf("   %s[1]%s  %scat%s   whole file\n", CYAN(), BOLD(), WHITE(), RESET());
            printf("   %s[2]%s  %shead%s  first 10 lines\n", CYAN(), BOLD(), WHITE(), RESET());
            printf("   %s[3]%s  %stail%s  last 10 lines\n", CYAN(), BOLD(), WHITE(), RESET());
            prompt(3);
            sub = read_int(3);
            if (sub == 1)
                snprintf(cmd, sizeof cmd, "cat %s/sellers/product1.txt", g_base);
            else if (sub == 2)
                snprintf(cmd, sizeof cmd, "head -n 10 %s/sellers/product1.txt", g_base);
            else if (sub == 3)
                snprintf(cmd, sizeof cmd, "tail -n 10 %s/sellers/product1.txt", g_base);
            else {
                printf("  %s✗%s  invalid choice\n", RED(), RESET());
                break;
            }
            run_cmd(cmd, "viewing sellers/product1.txt");
            break;
        }
        case 6: { /* find product */
            int sub;
            printf("   %s[1]%s  %sfind%s  by name\n", CYAN(), BOLD(), WHITE(), RESET());
            printf("   %s[2]%s  %sgrep%s  by content\n", CYAN(), BOLD(), WHITE(), RESET());
            prompt(2);
            sub = read_int(2);
            if (sub == 1) {
                snprintf(cmd, sizeof cmd, "find %s/sellers -type f", g_base);
                run_cmd(cmd, "finding products by name");
            } else if (sub == 2) {
                snprintf(cmd, sizeof cmd,
                         "grep -rn 'Product' %s/sellers || echo '(nothing matched)'", g_base);
                run_cmd(cmd, "searching product content");
            } else {
                printf("  %s✗%s  invalid choice\n", RED(), RESET());
            }
            break;
        }
        case 7: /* use aliases (falls back to a plain ls) */
            snprintf(cmd, sizeof cmd,
                     "bash -lc 'source ~/.bashrc 2>/dev/null; "
                     "if type listfiles >/dev/null 2>&1; then listfiles; else ls -la %s; fi'",
                     g_base);
            run_cmd(cmd, "running alias: listfiles");
            break;
        case 0:
            break;
        default:
            printf("  %s✗%s  invalid choice\n", RED(), RESET());
        }
    } while (choice != 0);
}

/* ------------------------------------------------------------------ */
/*  CUSTOMER menu                                                      */
/* ------------------------------------------------------------------ */

static void customer_menu(void)
{
    char cmd[8192];
    char order[16];
    char when[64];
    int choice;

    do {
        status_line("customer", CYAN());
        menu_box("CUSTOMER STORE", "browse & checkout");
        menu_item(1, "🛍️", "View products", "browse the catalog");
        menu_item(2, "🧾", "Purchase product", "checkout + receipt");
        menu_item(3, "📜", "View purchase history", "ls the orders folder");
        menu_item(4, "🚚", "View order status", "shipped?");
        menu_item(5, "✍️", "Add notes / preferences", "append to order notes");
        menu_item(6, "👀", "View order details", "cat / head / tail");
        menu_item(7, "🔎", "Search orders", "by ID or content");
        menu_item(8, "⭐", "Leave feedback", "rate 1-5 stars");
        menu_exit("exit the customer store");
        prompt(8);
        choice = read_int(8);

        switch (choice) {
        case 1: /* view products */
            printf("  %s─ product catalog ─%s\n", CYAN(), RESET());
            snprintf(cmd, sizeof cmd,
                     "cat %s/sellers/product*.txt 2>/dev/null || echo '(catalog is empty)'",
                     g_base);
            run_cmd(cmd, "showing the catalog");
            break;
        case 2: { /* purchase product: copy + order header + receipt */
            time_t t = time(NULL);
            struct tm lt;
            int packed;
            localtime_r(&t, &lt);
            strftime(when, sizeof when, "%a %b %d %H:%M:%S %Y", &lt);
            snprintf(order, sizeof order, "TC-%04X",
                     (unsigned)(t ^ ((time_t)getpid() << 8)) & 0xFFFF);

            snprintf(cmd, sizeof cmd,
                     "cp %s/sellers/product1.txt %s/customers/purchased_product1.txt",
                     g_base, g_base);
            packed = run_cmd(cmd, "packing your order");

            if (packed) {
                snprintf(cmd, sizeof cmd,
                         "{ printf '# Order %s purchased: %s\\n' '%s' '%s'; "
                         "cat %s/customers/purchased_product1.txt; } > %s/customers/.order.tmp && "
                         "mv %s/customers/.order.tmp %s/customers/purchased_product1.txt",
                         order, when, order, when,
                         g_base, g_base, g_base, g_base);
                run_cmd(cmd, "stamping the order header");

                snprintf(cmd, sizeof cmd,
                         "printf 'Order %s — %s\\nStatus: Shipped (arrives in 3-5 business days)\\n' '%s' '%s' > %s/customers/order_status.txt",
                         order, PRODUCT_NAME, order, PRODUCT_NAME, g_base);
                (void)system(cmd);

                receipt(order, when);
                confetti();
            }
            break;
        }
        case 3: /* view purchase history */
            snprintf(cmd, sizeof cmd, "ls -la %s/customers", g_base);
            run_cmd(cmd, "showing your purchase history");
            break;
        case 4: /* view order status (real file, not a hardcoded string) */
            snprintf(cmd, sizeof cmd,
                     "[ -f %s/customers/order_status.txt ] && cat %s/customers/order_status.txt "
                     "|| printf 'No orders yet — make your first purchase!\\n'",
                     g_base, g_base);
            run_cmd(cmd, "checking order status");
            break;
        case 5: /* add notes or preferences */
            snprintf(cmd, sizeof cmd,
                     "printf \"[$(date)] Customer preference: Express shipping\\n\" >> %s/customers/customer_notes.txt && tail -n 3 %s/customers/customer_notes.txt",
                     g_base, g_base);
            run_cmd(cmd, "adding notes to your order");
            break;
        case 6: { /* view order details */
            int sub;
            printf("   %s[1]%s  %scat%s   whole file\n", CYAN(), BOLD(), WHITE(), RESET());
            printf("   %s[2]%s  %shead%s  first 10 lines\n", CYAN(), BOLD(), WHITE(), RESET());
            printf("   %s[3]%s  %stail%s  last 10 lines\n", CYAN(), BOLD(), WHITE(), RESET());
            prompt(3);
            sub = read_int(3);
            if (sub == 1)
                snprintf(cmd, sizeof cmd, "cat %s/customers/purchased_product1.txt 2>/dev/null || echo '(no order yet — purchase one!)'", g_base);
            else if (sub == 2)
                snprintf(cmd, sizeof cmd, "head -n 10 %s/customers/purchased_product1.txt 2>/dev/null || echo '(no order yet)'", g_base);
            else if (sub == 3)
                snprintf(cmd, sizeof cmd, "tail -n 10 %s/customers/purchased_product1.txt 2>/dev/null || echo '(no order yet)'", g_base);
            else {
                printf("  %s✗%s  invalid choice\n", RED(), RESET());
                break;
            }
            run_cmd(cmd, "viewing your order");
            break;
        }
        case 7: { /* search orders */
            int sub;
            printf("   %s[1]%s  %sfind%s  by order file\n", CYAN(), BOLD(), WHITE(), RESET());
            printf("   %s[2]%s  %sgrep%s  by content\n", CYAN(), BOLD(), WHITE(), RESET());
            prompt(2);
            sub = read_int(2);
            if (sub == 1) {
                snprintf(cmd, sizeof cmd, "find %s/customers -type f -name 'purchased*'", g_base);
                run_cmd(cmd, "finding your orders");
            } else if (sub == 2) {
                snprintf(cmd, sizeof cmd,
                         "grep -rn 'Product' %s/customers || echo '(no matches — no orders yet?)'",
                         g_base);
                run_cmd(cmd, "searching order content");
            } else {
                printf("  %s✗%s  invalid choice\n", RED(), RESET());
            }
            break;
        }
        case 8: { /* feedback: rating + stars + optional comment */
            int rating;
            char comment[80];
            int i;
            printf("   %srate your experience: 1-5%s\n", DIM(), RESET());
            prompt(5);
            rating = read_range(1, 5);
            printf("   %s", YELLOW());
            for (i = 0; i < rating; i++)
                printf("★");
            for (i = rating; i < 5; i++)
                printf("☆");
            printf("%s\n", RESET());

            printf("  %s➜%s %sone-line comment (optional): %s",
                   CYAN(), BOLD(), DIM(), RESET());
            if (fgets(comment, sizeof comment, stdin) == NULL)
                strcpy(comment, "\n");
            comment[strcspn(comment, "\n")] = '\0';
            for (i = 0; comment[i]; i++)        /* keep the shell quote safe */
                if (comment[i] == '\'')
                    comment[i] = '`';
            comment[40] = '\0';
            if (comment[0] == '\0')
                strcpy(comment, "Great product!");

            snprintf(cmd, sizeof cmd,
                     "printf 'Customer feedback: %d stars - %s\\n' '%d' '%s' >> %s/customers/feedback.txt && tail -n 2 %s/customers/feedback.txt",
                     rating, comment, rating, comment, g_base, g_base);
            run_cmd(cmd, "recording your feedback");
            break;
        }
        case 0:
            break;
        default:
            printf("  %s✗%s  invalid choice\n", RED(), RESET());
        }
    } while (choice != 0);
}

/* ------------------------------------------------------------------ */
/*  main                                                               */
/* ------------------------------------------------------------------ */

int main(void)
{
    int role;

    srand((unsigned)(time(NULL) ^ ((unsigned)getpid() << 8)));
    init_term();
    init_base_path();

    if (t_on) {
        boot_sequence();
        clear_screen();
        print_banner();
        printf("\n");
        typewriter("  the shell-powered store — where bash meets checkout.");
        printf("\n");
    } else {
        printf("terminal commerce v2.0 (cool edition)\n");
        printf("base: %s\n", g_base);
    }

    for (;;) {
        status_line("—", GRAY());
        menu_box("TERMINAL COMMERCE", "choose your role");
        menu_item(1, "👑", "Admin", "back office & file ops");
        menu_item(2, "🏪", "Seller", "manage the products");
        menu_item(3, "🛒", "Customer", "browse & checkout");
        menu_exit("exit the store");
        prompt(3);
        role = read_int(3);

        if (role == 0) {
            printf("\n  %s👋 thanks for shopping — see you in the shell!%s\n\n",
                   BOLD(), RESET());
            break;
        }

        if (t_on) {
            clear_screen();
            sleep_ms(120);
        }
        switch (role) {
        case 1:
            admin_menu();
            break;
        case 2:
            seller_menu();
            break;
        case 3:
            customer_menu();
            break;
        default:
            printf("  %s✗%s  unknown role\n", RED(), RESET());
        }
        if (t_on) {
            printf("\n  %s(returning to role select…)%s\n", DIM(), RESET());
            sleep_ms(400);
            clear_screen();
        }
    }

    return 0;
}
