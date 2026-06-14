#include <stdio.h>
#include <string.h>
#include <termios.h>
#include <time.h>
#include <unistd.h>
#include "race.h"

int
main(void) {
    struct termios og, new;
    if (tcgetattr(0, &og) != 0) {
        fprintf(stderr, "Sorry, Couldn't retrieve terminal info!\n");
        return 1;
    }
    new = og;
    new.c_lflag &= ~(ICANON | ECHO);

    if (tcsetattr(0, TCSANOW, &new) != 0) {
        fprintf(stderr, "Failed to apply the new setting!\n");
    }

    char *text = "one two three.";
    countdown(text);
    play(text);

    // Reset the terminal attribute.
    if (tcsetattr(0, TCSANOW, &og) != 0) {
        fprintf(stderr, "Failed to restore the original settings!\n");
    }
    return 0;
}

void
play(char *text) {
    if (text == NULL)
        return;
    int len = strlen(text), typed = 0;
    char *hitback = "\b \b", buf, cbuf[32];

    time_t start = time(NULL), end;
    while (typed != len) {
        read(0, &buf, sizeof(char));
        memset(cbuf, 0, sizeof(char) * 32);
        // erase in case of backspace.
        if (buf == 8 || buf == 127) {
            if (typed > 0) {
                write(1, hitback, sizeof(char) * 3);
                typed--;
            }
        } else {
            if (buf != text[typed]) {
                snprintf(cbuf, sizeof(char) * 32, "\x1b[31m%c\x1b[0m", buf);
                write(1, cbuf, sizeof(char) * strlen(cbuf));
            } else {
                snprintf(cbuf, sizeof(char) * 32, "\x1b[32m%c\x1b[0m", buf);
                write(1, cbuf, sizeof(char) * strlen(cbuf));
            }
            typed++;
        }
    }
    end = time(NULL);
    // Clear stdin.
    while ((buf = getchar()) != EOF && buf != '\n');
    printstats(len, start, end);
}

void
countdown(char *text) {
    // Print the text to be typed.
    printf("\x1b[31mLet's see how quickly you can type this...\x1b[0m\n\n\t");
    printf("\x1b[35m" "%s" "\x1b[0m" "\n\n", text);

    // Quick countdown.
    for (int i = COUNTDOWN; i > 0; i--) {
        printf("Starting in %d...\r", i);
        fflush(stdout);
        sleep(1);
    }
    printf("\a\x1b[32mGo......        \x1b[0m\n\n");
}

void
printstats(int len, time_t start, time_t end)
{
    double time = difftime(end, start);
    /* Simple wpm formula with the following constraints:
        * A word consists of 4 words.
        * Currently the formula doesn't account for the incorrect characters.
    */
    float wpm = (len * 60) / (time * 4);
    printf("\n\n");
    printf("\x1b[33m┌─────────────────────────────────────┐\x1b[0m\n");
    printf("\x1b[33m│ Stats:                              │\x1b[0m\n");
    printf("\x1b[33m├─────────────────────────────────────┤\x1b[0m\n");
    printf("\x1b[33m│ Calculated wpm: %7.2f             │\x1b[0m\n", wpm);
    printf("\x1b[33m│ Characters typed: %d                │\x1b[0m\n", len);
    printf("\x1b[33m│ Complete in %6.3lf seconds.         │\x1b[0m\n", time);
    printf("\x1b[33m└─────────────────────────────────────┘\x1b[0m\n");
}
