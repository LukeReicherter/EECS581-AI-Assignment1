/*
 * digit_accum.c
 *
 * STUDENT-WRITTEN CODE -- NOT AI-generated. Write this function yourself.
 *
 * This is the one piece of arithmetic the assignment requires you to
 * implement by hand: turning a stream of decimal digit characters into
 * an integer value, one digit at a time. Everything that calls this
 * function (the octet/port scanning and validation in ipv4_extract.c)
 * was AI-generated -- see PROMPT_LOG.md.
 */

#include <stdio.h>
#include <stdlib.h>
#include "ipv4_extract.h"

int accumulateDigit(int currentValue, char digitChar) {
    return currentValue * 10 + (digitChar - '0');	
}
