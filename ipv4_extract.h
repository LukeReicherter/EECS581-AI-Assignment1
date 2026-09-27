/*
 * ipv4_extract.h
 *
 * AI-GENERATED CODE
 * Model: Claude Sonnet 5 (model id: claude-sonnet-5), Anthropic.
 * Generated: 2026-09-27, for EECS 581 AI Assignment 1.
 * See PROMPT_LOG.md for the prompts that produced this file.
 *
 * Declares the extractIPv4() entry point plus the single helper,
 * accumulateDigit(), that the assignment requires to be written by hand
 * (see digit_accum.c). extractIPv4() itself is implemented in
 * ipv4_extract.c and calls accumulateDigit() internally.
 */

#ifndef IPV4_EXTRACT_H
#define IPV4_EXTRACT_H

/*
 * Scans str for the first substring matching the grammar:
 *
 *   octet    := digit | digit digit | digit digit digit
 *               (value 0-255; no leading zero unless the octet is exactly "0")
 *   address  := octet "." octet "." octet "." octet
 *   port     := 1 to 5 digits (value 0-65535; no leading zero unless "0")
 *   token    := address [ ":" port ]
 *
 * A candidate is any maximal run of characters drawn only from
 * {'0'-'9', '.', ':'}. Each such run must match "token" in full -- no
 * partial matches and no truncating digits to force a piece into range.
 * A run that fails to match in full is rejected outright and scanning
 * resumes after it; the next candidate run is tried next.
 *
 * Returns 1 (true) if a matching token was found:
 *   *outAddress receives the address as a 32-bit value (octet 0 in the
 *   high-order byte, matching standard network byte order left-to-right),
 *   *outPort receives the port number, or -1 if the token had no port.
 *
 * Returns 0 (false) if no token in str matches the grammar:
 *   *outAddress is set to 0 and *outPort is set to -1.
 */
int extractIPv4(const char* str, unsigned long* outAddress, int* outPort);

/*
 * STUDENT-WRITTEN CONTRACT -- implemented by hand in digit_accum.c, not AI.
 *
 * Accumulates one more decimal digit into a running integer value, as if
 * digitChar were appended to the decimal representation of currentValue.
 * The caller guarantees digitChar is '0'-'9' and that the result cannot
 * overflow int for the callers in this program (accumulation is capped at
 * 3 digits for octets and 5 digits for ports before this is called again).
 *
 * Example: accumulateDigit(0, '1') == 1
 *          accumulateDigit(1, '0') == 10
 *          accumulateDigit(10, '5') == 105
 */
int accumulateDigit(int currentValue, char digitChar);

#endif /* IPV4_EXTRACT_H */
