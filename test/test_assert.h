#ifndef TEST_ASSERT_H
#define TEST_ASSERT_H

#define ASSERT_TRUE(expr) \
    if (!(expr)) \
    { \
        printf("Assert failed in function %s (%s:%d).\n", __func__, __FILE__, __LINE__); \
        return 1; \
    }

#endif TEST_ASSERT_H
