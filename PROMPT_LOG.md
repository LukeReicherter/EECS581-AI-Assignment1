# AI Prompt Log — EECS 581 AI Assignment 1
09/27/2026
## Model disclosure

All AI-generated code and text in this repository was produced by **Claude Sonnet 5**
(model id `claude-sonnet-5`, Anthropic), running inside Claude Code, on 2026-09-27.

## What is and isn't AI-generated

- **AI-generated**: `ipv4_extract.h`, `ipv4_extract.c`, `main.c` (as originally
  generated), `Makefile`, `.gitignore`, `tests.c`, this file. Each source file carries
  a header comment naming the model and generation date.
- **Note**: after generation, `main.c` received a direct hand edit (not via an AI
  prompt) adding a `"Program terminated."` message on the `END` path, marked inline
  with `// Added by Luke Reicherter`. That line is not AI-generated.
- **Note**: `digit_accum.c`'s `accumulateDigit()` has since been implemented by hand
  (replacing the crashing placeholder described above) — not AI-generated.
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

## Prompt 3

**User prompt (verbatim):**

> add a gitignore

**Response summary / changes made:**

Added `.gitignore` ignoring build artifacts (`*.o`, `*.obj`, the `ipv4prog` binary) and
common editor/OS cruft (`.DS_Store`, `*.swp`). No other files changed.

Also flagged, unprompted, that the user's own concurrent hand edit to `main.c` (adding
`printf("Program terminated.")` on the `END` path) was initially missing a trailing
semicolon and would fail to compile; the user fixed it before this prompt.

## Fix — output format for missing port

The full assignment handout specifies the success line must read
`port: P` where P is the port number **or the literal text `none`** when no port is
present. `main.c` had been printing the internal `-1` sentinel value instead (`port:
-1`), based on an earlier assumption made before this detail was pinned down.

**Change made:** `main.c`'s success branch now checks `port == -1` and prints
`port: none` in that case, `port: %d` otherwise. Rebuilt and manually verified:
`192.168.1.1` (no port) now prints `port: none`; ported inputs are unaffected.

## Prompt 4

**User prompt (verbatim):**

> Make a test suite the covers a bunch of test cases, this would be a separate
> executable that tries a ton of separate inputs and determines if it matches the
> expected output

**Response summary / changes made:**

Added `tests.c`: a standalone test suite with its own `main()`, built as a separate
executable (`ipv4test`) from the interactive program, so it doesn't collide with
`main.c`'s `main()`. It defines 36 test cases as `{name, input, expectMatch,
expectAddress, expectPort}` records and runs each one through `extractIPv4()`,
comparing the actual result against the expected one and printing PASS/FAIL per case
plus a final `passed/total` count; the process exits nonzero if any case fails.

Cases cover: the example from the original prompt, addresses with and without a port,
all-zero and max-value addresses/ports, leading-zero rejection on both octets and the
port, out-of-range octets and ports, wrong octet counts, empty octets (`1..2.3`),
stray leading/trailing `.` or `:`, a second colon, a colon before the fourth octet, a
repeated colon, oversized digit runs, no candidate characters at all, an invalid
candidate run followed by a later valid one (confirms rejection doesn't stop the
scan), only the first valid match being returned when two valid addresses are present,
and a few boundary strings (`""`, `":"`, `"..."`).

Wired `tests.c` into the `Makefile` as a `make test` target (builds `ipv4test` and runs
it) and added `ipv4test` to `.gitignore`. Built and ran it against the current
`ipv4_extract.c` and the student's own hand-written `accumulateDigit()`: all 36/36
tests passed.

## Prompt 5

**User prompt (verbatim):**

> Invalid input: no valid IPv4 address found

**Response summary / changes made:**

Interpreted as the exact wording wanted for the failure-case output, replacing the
previous placeholder text (`No valid IPv4 address found.`, flagged as an assumption in
Prompt 2/3's notes). Updated `main.c`'s failure branch to print
`Invalid input: no valid IPv4 address found` verbatim (no trailing period). Rebuilt
and manually verified against a no-match input; re-ran `make test` to confirm the
change didn't affect `extractIPv4()` behavior (36/36 still pass, since the test suite
checks return values/outAddress/outPort, not this printed string).

## Project Verification Statement

I, Luke Reicherter, verify that all code generated by AI has been reviewed to ensure requirements are met.
I understand each line of code written and can verify that the code is safe to run.
