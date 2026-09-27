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
    /*
     * TODO(student): implement this yourself.
     *
     * Contract (see ipv4_extract.h for the full comment):
     *   - digitChar is guaranteed to be '0'-'9'.
     *   - Return the value you get by appending digitChar to the decimal
     *     representation of currentValue, e.g. accumulateDigit(12, '3')
     *     should return 123.
     *   - Do not use atoi/strtol/sscanf or any other string-to-number
     *     conversion function -- this must be plain arithmetic on
     *     digitChar's numeric value.
     *
     * The lines below are a deliberate placeholder, not a solution: they
     * make the failure to implement this function loud (a crash) instead
     * of silently returning wrong IP addresses. Delete them once you've
     * written your own return statement.
     */
	
}
