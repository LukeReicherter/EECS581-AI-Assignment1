/* AI-GENERATED CODE -- Claude Sonnet 5 (model id: claude-sonnet-5), Anthropic. */

#include <stdio.h>
#include "ipv4_extract.h"

typedef struct {
    const char* name;
    const char* input;
    int expectMatch;
    unsigned long expectAddress;
    int expectPort;
} TestCase;

static unsigned long pack(unsigned long a, unsigned long b, unsigned long c, unsigned long d) {
    return (a << 24) | (b << 16) | (c << 8) | d;
}

int main(void) {
    TestCase tests[] = {
        { "example_from_spec", "fbabsdbfiabsfbasdf100.10.2.9:80fasbdfkhashfbas", 1, pack(100,10,2,9), 80 },
        { "plain_address_no_port", "192.168.1.1", 1, pack(192,168,1,1), -1 },
        { "address_with_port", "10.0.0.1:8080", 1, pack(10,0,0,1), 8080 },
        { "address_embedded_in_text", "log: client 10.0.0.1:8080 connected", 1, pack(10,0,0,1), 8080 },
        { "all_zero_octets", "0.0.0.0", 1, pack(0,0,0,0), -1 },
        { "all_zero_with_port", "0.0.0.0:0", 1, pack(0,0,0,0), 0 },
        { "max_valid_address_and_port", "255.255.255.255:65535", 1, pack(255,255,255,255), 65535 },
        { "single_zero_octet_ok", "10.0.0.1", 1, pack(10,0,0,1), -1 },
        { "leading_zero_octet_rejected", "010.0.0.1", 0, 0, -1 },
        { "leading_zero_middle_octet_rejected", "10.00.0.1", 0, 0, -1 },
        { "leading_zero_port_rejected", "1.2.3.4:080", 0, 0, -1 },
        { "octet_out_of_range_999", "999.1.1.1", 0, 0, -1 },
        { "octet_out_of_range_256", "256.1.1.1", 0, 0, -1 },
        { "port_out_of_range_99999", "1.2.3.4:99999", 0, 0, -1 },
        { "port_out_of_range_65536", "1.2.3.4:65536", 0, 0, -1 },
        { "too_few_octets", "1.2.3", 0, 0, -1 },
        { "too_many_octets", "1.2.3.4.5", 0, 0, -1 },
        { "empty_octet_double_dot", "1..2.3", 0, 0, -1 },
        { "leading_dot", ".1.2.3.4", 0, 0, -1 },
        { "trailing_dot", "1.2.3.4.", 0, 0, -1 },
        { "trailing_colon_no_port", "1.2.3.4:", 0, 0, -1 },
        { "second_colon", "1.2.3.4:80:90", 0, 0, -1 },
        { "colon_before_fourth_octet", "1.2:3.4:80", 0, 0, -1 },
        { "colon_immediately_repeated", "1.2.3.4::80", 0, 0, -1 },
        { "four_digit_octet_rejected", "1234.2.3.4", 0, 0, -1 },
        { "six_digit_port_rejected", "1.2.3.4:123456", 0, 0, -1 },
        { "no_candidate_chars_at_all", "no ip address here at all", 0, 0, -1 },
        { "invalid_run_then_valid_run", "999.1.1.1 then 1.2.3.4", 1, pack(1,2,3,4), -1 },
        { "invalid_run_with_port_then_valid_run", "1.2.3.4:99999 then 5.6.7.8:9", 1, pack(5,6,7,8), 9 },
        { "only_one_match_returned_first_valid", "1.2.3.4 and 5.6.7.8", 1, pack(1,2,3,4), -1 },
        { "empty_string", "", 0, 0, -1 },
        { "just_a_colon", ":", 0, 0, -1 },
        { "just_dots", "...", 0, 0, -1 },
        { "port_zero_valid", "1.2.3.4:0", 1, pack(1,2,3,4), 0 },
        { "single_digit_octets_and_port", "1.2.3.4:5", 1, pack(1,2,3,4), 5 },
        { "letters_directly_adjacent_no_space", "abc1.2.3.4:80xyz", 1, pack(1,2,3,4), 80 },
    };

    size_t count = sizeof(tests) / sizeof(tests[0]);
    size_t passed = 0;
    size_t i;

    for (i = 0; i < count; i++) {
        unsigned long address = 0;
        int port = -1;
        int result = extractIPv4(tests[i].input, &address, &port);
        int ok = (result == tests[i].expectMatch);

        if (ok && tests[i].expectMatch) {
            ok = (address == tests[i].expectAddress) && (port == tests[i].expectPort);
        }
        if (ok && !tests[i].expectMatch) {
            ok = (address == 0) && (port == -1);
        }

        if (ok) {
            passed++;
            printf("PASS  %-38s\n", tests[i].name);
        } else {
            printf("FAIL  %-38s input=\"%s\"\n", tests[i].name, tests[i].input);
            printf("        got:      match=%d address=%lu port=%d\n", result, address, port);
            printf("        expected: match=%d address=%lu port=%d\n",
                   tests[i].expectMatch, tests[i].expectAddress, tests[i].expectPort);
        }
    }

    printf("\n%zu/%zu tests passed\n", passed, count);
    return (passed == count) ? 0 : 1;
}
