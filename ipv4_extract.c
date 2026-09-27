/*
 * ipv4_extract.c
 *
 * AI-GENERATED CODE
 * Model: Claude Sonnet 5 (model id: claude-sonnet-5), Anthropic.
 * Generated: 2026-09-27, for EECS 581 AI Assignment 1.
 * See PROMPT_LOG.md for the prompts that produced this file and any
 * later revisions.
 *
 * Implements extractIPv4() per the grammar documented in ipv4_extract.h.
 * No string-to-number conversion functions (atoi/strtol/sscanf/etc.),
 * address-parsing libraries, or regex libraries are used anywhere here.
 * The one piece of arithmetic this file does NOT perform itself --
 * turning a run of digit characters into an integer -- is delegated to
 * accumulateDigit(), which is written by hand in digit_accum.c.
 */

#include <stddef.h>
#include "ipv4_extract.h"

#define OCTET_MAX_DIGITS 3
#define OCTET_MAX_VALUE  255
#define PORT_MAX_DIGITS  5
#define PORT_MAX_VALUE   65535

static int isDigitChar(char c) {
    return c >= '0' && c <= '9';
}

static int isCandidateChar(char c) {
    return isDigitChar(c) || c == '.' || c == ':';
}

/*
 * Parses a decimal number starting at str[*pos] (stopping before 'end'),
 * advancing *pos past the digits it consumes. Rejects (returns 0) rather
 * than truncating if there are more than maxDigits digits, if there is a
 * disallowed leading zero, if the value exceeds maxValue, or if there are
 * no digits at all.
 */
static int parseNumberAt(const char* str, size_t end, size_t* pos,
                          int maxDigits, int maxValue, int* outValue) {
    size_t start = *pos;
    int digitCount = 0;
    int value = 0;

    while (*pos < end && isDigitChar(str[*pos])) {
        if (digitCount == maxDigits) {
            return 0;
        }
        value = accumulateDigit(value, str[*pos]);
        digitCount++;
        (*pos)++;
    }

    if (digitCount == 0) {
        return 0;
    }
    if (digitCount > 1 && str[start] == '0') {
        return 0;
    }
    if (value > maxValue) {
        return 0;
    }

    *outValue = value;
    return 1;
}

/*
 * Attempts to match the full "token" grammar (address plus optional port)
 * against the candidate run str[start..end). Requires the entire run to
 * be consumed -- no partial matches.
 */
static int parseRun(const char* str, size_t start, size_t end,
                     unsigned long* outAddress, int* outPort) {
    size_t pos = start;
    int octets[4];
    int port = -1;
    int i;

    for (i = 0; i < 4; i++) {
        if (i > 0) {
            if (pos >= end || str[pos] != '.') {
                return 0;
            }
            pos++;
        }
        if (!parseNumberAt(str, end, &pos, OCTET_MAX_DIGITS, OCTET_MAX_VALUE, &octets[i])) {
            return 0;
        }
    }

    if (pos < end && str[pos] == ':') {
        pos++;
        if (!parseNumberAt(str, end, &pos, PORT_MAX_DIGITS, PORT_MAX_VALUE, &port)) {
            return 0;
        }
    }

    if (pos != end) {
        return 0;
    }

    *outAddress = ((unsigned long)octets[0] << 24)
                | ((unsigned long)octets[1] << 16)
                | ((unsigned long)octets[2] << 8)
                |  (unsigned long)octets[3];
    *outPort = port;
    return 1;
}

int extractIPv4(const char* str, unsigned long* outAddress, int* outPort) {
    size_t i;

    *outAddress = 0;
    *outPort = -1;

    if (str == NULL) {
        return 0;
    }

    i = 0;
    while (str[i] != '\0') {
        size_t runStart;
        size_t runEnd;
        unsigned long addr;
        int port;

        if (!isCandidateChar(str[i])) {
            i++;
            continue;
        }

        runStart = i;
        runEnd = i;
        while (str[runEnd] != '\0' && isCandidateChar(str[runEnd])) {
            runEnd++;
        }

        if (parseRun(str, runStart, runEnd, &addr, &port)) {
            *outAddress = addr;
            *outPort = port;
            return 1;
        }

        i = runEnd;
    }

    return 0;
}
