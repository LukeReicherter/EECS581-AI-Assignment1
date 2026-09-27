# AI Prompt Log — EECS 581 AI Assignment 1

## Model disclosure

All AI-generated code and text in this repository was produced by **Claude Sonnet 5**
(model id `claude-sonnet-5`, Anthropic), running inside Claude Code, on 2026-09-27.

## What is and isn't AI-generated

- **AI-generated**: `ipv4_extract.h`, `ipv4_extract.c`, `main.c`, `Makefile`, this file.
  Each source file carries a header comment naming the model and generation date.
- **Not AI-generated (student-written)**: `digit_accum.c` — specifically the body of
  `accumulateDigit()`, the function that turns a decimal digit character into part of
  an accumulated integer value. The AI-generated code in `ipv4_extract.c` calls this
  function but does not implement it; it ships as a stub that intentionally crashes
  with a message until the real implementation is written in.

## Prompt 1

**User prompt (verbatim, assignment spec):**

> I need to develop a C program that reads a line of text and extracts a single valid
> IP address, with the optional port field. The only valid chars are digits, ., and :.
> No partial matches allowed, and no truncating to form a valid piece. An example of
> an input would look like fbabsdbfiabsfbasdf100.10.2.9:80fasbdfkhashfbas. The function
> prototype is as follows: Returns true if valid address found, false otherwise. On
> success: *outAddress contains the 32 bit value, and *outPort holds the port number,
> or -1 if no port detected. On failure, *outAddress is set to 0 and *outport is set to
> -1. `int extractIPv4(const char* str, unsigned long* outAddress, int* outPort);` You
> may not use any of the following functions/libraries: string to num conversions
> funcs, address parsing libraries, regex parsing libraries. Only one valid address
> will be extracted per input line, reject anything that doesnt match the grammar, and
> on success print `Extracted IPv4 address: A.B.C.D (decimal value: N, port: P)`. the
> main function continuosly asks for the users input until the user enters END
> (case-sensitive). extractIPv4 must be exactly as prototyped, and the digit
> accumulation must be done by me (not AI). main recieves the result from extractIPv4
> and formats the output. Please clearly document all code that was AI generated and
> create a document to keep track of the prompts I give you, with a summary of the
> response and changes made. Disclose what model name and version you are. Do you need
> any further information to complete this request?

**Response summary:** Asked clarifying questions (via a tool the user declined) about
leading-zero handling, how to split AI vs. student-written code, port range rules, and
file layout.

## Prompt 2

**User prompt (verbatim):**

> no leading zeros, but 10.0.0.1 is fine
>
> An address is four octets separated by periods (octet.octet.octet.octet), each octet
> 1–3 digits, value 0–255, no leading zero unless the value is exactly 0. An optional
> :port may follow the fourth octet: 1–5 digits, value 0–65535, same leading-zero rule.
> If a colon is present, the port must be fully valid or the entire match — address
> included — is rejected.

**Response summary / changes made:**

Interpreted as: an octet or port field may not have a leading `'0'` unless the field is
the single digit `"0"` (so `10.0.0.1`, `0.0.0.0` are valid; `010.0.0.1`, `1.2.3.4:080`
are rejected). The port field explicitly follows the same leading-zero rule as octets,
and an invalid port after a colon invalidates the whole match, not just the port —
both now given directly by the user rather than assumed.

Made the remaining decisions myself, flagged rather than re-blocked on, since the
AskUserQuestion tool had just been declined:

- **Digit accumulation split**: `accumulateDigit(int currentValue, char digitChar)` is
  the one function the AI does not implement. Everything that calls it (run-scanning,
  digit-count limits, leading-zero checks, range checks) is AI-written in
  `ipv4_extract.c`. The stub in `digit_accum.c` deliberately crashes with a message
  instead of silently returning wrong values, so an unimplemented state is obvious
  when the program is run.
- **File layout**: split into `ipv4_extract.h`/`ipv4_extract.c` (AI, extraction logic),
  `digit_accum.c` (student, the one arithmetic function), `main.c` (AI, I/O loop), and
  a `Makefile`, rather than one monolithic file — so the human-written piece is
  isolated in its own translation unit rather than buried inside AI-written code.
- **Output format for cases the given template doesn't cover**: printed
  `port: -1` (the literal sentinel value) when no port was found, and
  `No valid IPv4 address found.` when no address matches — neither was specified
  verbatim in the prompt.

Implemented and tested `extractIPv4()`'s grammar:

- Only `0`–`9`, `.`, `:` are valid characters; a candidate token is a maximal run of
  such characters bounded by any other character (or start/end of string).
- Each candidate run must match `octet.octet.octet.octet[:port]` **in full** — if any
  part is malformed (wrong octet/port range, leading zero, too many digits, stray `.`
  or `:`, leftover characters), the *entire* run is rejected outright rather than
  truncated or partially matched, and scanning resumes after it for the next run. In
  particular, a colon with an invalid or missing port invalidates the whole run,
  address included — it is not retried as a bare address without the port.
- Verified against 15 cases in a throwaway test harness (not part of the deliverable)
  using a temporary reference `accumulateDigit`, including: the example from the
  prompt, leading-zero rejection, all-zero address, out-of-range octet/port,
  no-port address embedded in text, malformed runs (`1..2.3.4`, `1.2.3.4:`,
  `1.2.3.4:80:90`), max-value address+port (`255.255.255.255:65535`), and a
  second valid run following a rejected one on the same line. All matched
  expected behavior.
- Confirmed the real deliverable (with the intentionally-crashing student stub)
  compiles cleanly with `-Wall -Wextra`, handles `END` and no-match lines without
  needing `accumulateDigit`, and crashes with a clear message on any line containing
  an actual candidate IP, until `digit_accum.c` is filled in.

## Open items for the student

- `digit_accum.c`: implement `accumulateDigit()` yourself (see the contract comment in
  `ipv4_extract.h` and the TODO in `digit_accum.c`).
- The no-port / no-match output text was my assumption, not explicitly stated in the
  prompt — confirm it matches what's expected before submitting.
