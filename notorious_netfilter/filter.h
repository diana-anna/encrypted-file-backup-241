/**
 * Notorious Netfilter
 * CS 241 - Spring 2019
 */
#define TO_IP_ADDR(w, x, y, z) ((unsigned int *)((unsigned char[]){w, x, y, z}))

struct ip_range {
    unsigned int *start;
    unsigned int *end;
};

#define new_ip_range(w1, x1, y1, z1, _, w2, x2, y2, z2)        \
    (struct ip_range) {                                        \
        TO_IP_ADDR(w1, x1, y1, z1), TO_IP_ADDR(w2, x2, y2, z2) \
    }

#define IN_IP_RANGE(addr, range) \
    (ntohl(*(range.start)) <= ntohl(addr) && ntohl(addr) <= ntohl(*(range.end)))

struct ip_range google_ranges[] = {
    new_ip_range(35, 190, 247, 0, -, 35, 190, 247, 255),
    new_ip_range(35, 191, 0, 0, -, 35, 191, 255, 255),
    new_ip_range(64, 233, 160, 0, -, 64, 233, 191, 255),
    new_ip_range(66, 102, 0, 0, -, 66, 102, 15, 255),
    new_ip_range(66, 249, 80, 0, -, 66, 249, 95, 255),
    new_ip_range(72, 14, 192, 0, -, 72, 14, 255, 255),
    new_ip_range(74, 125, 0, 0, -, 74, 125, 255, 255),
    new_ip_range(108, 177, 8, 0, -, 108, 177, 15, 255),
    new_ip_range(108, 177, 96, 0, -, 108, 177, 127, 255),
    new_ip_range(130, 211, 0, 0, -, 130, 211, 3, 255),
    new_ip_range(172, 217, 0, 0, -, 172, 217, 31, 255),
    new_ip_range(172, 217, 32, 0, -, 172, 217, 47, 255),
    new_ip_range(172, 217, 128, 0, -, 172, 217, 159, 255),
    new_ip_range(172, 217, 160, 0, -, 172, 217, 175, 255),
    new_ip_range(172, 217, 192, 0, -, 172, 217, 223, 255),
    new_ip_range(173, 194, 0, 0, -, 173, 194, 255, 255),
    new_ip_range(209, 85, 128, 0, -, 209, 85, 255, 255),
    new_ip_range(216, 239, 32, 0, -, 216, 239, 63, 255),
    new_ip_range(216, 58, 192, 0, -, 216, 58, 223, 255),
};
size_t google_range_count = sizeof(google_ranges) / sizeof(struct ip_range);
