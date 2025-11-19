#include <stdint.h>

unsigned dig(unsigned offset, unsigned gap);
extern const unsigned char *island;

#ifdef LOCAL_TESTING
#include <assert.h>

static const unsigned char test_island[] = {0b00000000, 0b00111100, 0b00001001};
const unsigned char *island = test_island;

int main(void) {
    unsigned result = dig(7, 4);
    assert(result == 19);

    result = dig(0, 6);
    assert(result == 16);

    return 0;
}
#endif

unsigned dig(unsigned offset, unsigned gap) {
#define GET_CHEST(p) (island[(p) / 8u] & (1u << ((p) % 8u)))

    if (GET_CHEST(offset) == 0u) {
        return dig(offset + 1u, gap);
    }

    offset += gap;

    if (GET_CHEST(offset) != 0u) {
        return offset;
    }

    return dig(offset + 1u, gap - 1u);
}
