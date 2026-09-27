/*
 * main.c
 *
 * AI-GENERATED CODE
 * Model: Claude Sonnet 5 (model id: claude-sonnet-5), Anthropic.
 * Generated: 2026-09-27, for EECS 581 AI Assignment 1.
 * See PROMPT_LOG.md for the prompts that produced this file.
 *
 * Repeatedly reads a line of input, hands it to extractIPv4(), and
 * formats the result. Loops until the user enters the exact, case
 * sensitive line "END".
 *
 * Output format assumptions (not fully pinned down by the assignment
 * text, flagged here rather than guessed silently):
 *   - On success: "Extracted IPv4 address: A.B.C.D (decimal value: N, port: P)"
 *     where P is the literal outPort value, i.e. "-1" when no port was
 *     present in the input (matching extractIPv4's own no-port sentinel).
 *   - On failure: "No valid IPv4 address found."
 */

#include <stdio.h>
#include <string.h>
#include "ipv4_extract.h"

#define INPUT_BUFFER_SIZE 1024

int main(void) {
    char line[INPUT_BUFFER_SIZE];

    printf("Enter a line of text (or END to quit):\n");

    while (1) {
        size_t len;
        unsigned long address;
        int port;

        printf("> ");
        if (fgets(line, sizeof(line), stdin) == NULL) {
            break;
        }

        len = 0;
        while (line[len] != '\0') {
            len++;
        }
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }

        if (strcmp(line, "END") == 0) {
	    printf("Program terminated.");  // Added by Luke Reicherter
            break;
        }

        if (extractIPv4(line, &address, &port)) {
            unsigned int a = (unsigned int)((address >> 24) & 0xFF);
            unsigned int b = (unsigned int)((address >> 16) & 0xFF);
            unsigned int c = (unsigned int)((address >> 8) & 0xFF);
            unsigned int d = (unsigned int)(address & 0xFF);

            printf("Extracted IPv4 address: %u.%u.%u.%u (decimal value: %lu, port: %d)\n",
                   a, b, c, d, address, port);
        } else {
            printf("No valid IPv4 address found.\n");
        }
    }

    return 0;
}
