#include <stdint.h>
#include <stdio.h>
#include <assert.h>

#define ASSERT_TRUE(expr) \
    if (!(expr)) \
    { \
        printf("Assert failed in function %s (%s:%d).\n", __func__, __FILE__, __LINE__); \
        return 1; \
    }

typedef struct test_queue_struct
{
  uint8_t type;
  char string[10];
} test_queue_struct;

// Must be defined before including queue.h.
#define Q_SIZE 2
#define Q_TYPE test_queue_struct

#include <queue.h>

test_queue_struct e1 = { 1, "Hello 1" };
test_queue_struct e2 = { 2, "Hello 2" };
test_queue_struct e3 = { 3, "Hello 3" };

static int test_queue_dequeue_succeeds()
{
  queue q;
  queue_init(&q);

  queue_enqueue(&q, &e1);
  queue_enqueue(&q, &e2);

  test_queue_struct* ret;
  ret = queue_dequeue(&q);
  ASSERT_TRUE(e1.type == ret->type);

  ret = queue_dequeue(&q);
  ASSERT_TRUE(e2.type == ret->type);

  queue_enqueue(&q, &e3);
  ret = queue_dequeue(&q);
  ASSERT_TRUE(e3.type == ret->type);

  queue_enqueue(&q, &e2);
  queue_enqueue(&q, &e1);
  ret = queue_dequeue(&q);
  ASSERT_TRUE(e2.type == ret->type);
  ret = queue_dequeue(&q);
  ASSERT_TRUE(e1.type == ret->type);

  return 0;
}

static int test_queue_enqueue_overflow_fails()
{
  queue q;
  queue_init(&q);

  ASSERT_TRUE(queue_enqueue(&q, &e1));
  ASSERT_TRUE(queue_enqueue(&q, &e2));
  ASSERT_TRUE(!queue_enqueue(&q, &e3));
  ASSERT_TRUE(!queue_enqueue(&q, &e1));

  test_queue_struct* ret;
  ret = queue_dequeue(&q);
  ASSERT_TRUE(e1.type == ret->type);

  ret = queue_dequeue(&q);
  ASSERT_TRUE(e2.type == ret->type);

  return 0;
}

static int test_queue_dequeue_underflow_fails()
{
  queue q;
  queue_init(&q);

  ASSERT_TRUE(queue_enqueue(&q, &e1));
  ASSERT_TRUE(queue_enqueue(&q, &e2));

  test_queue_struct* ret;
  ret = queue_dequeue(&q);
  ASSERT_TRUE(e1.type == ret->type);

  ret = queue_dequeue(&q);
  ASSERT_TRUE(e2.type == ret->type);

  ret = queue_dequeue(&q);
  ASSERT_TRUE(NULL == ret);

  return 0;
}

int main()
{

  int ret = 0;
  ret += test_queue_dequeue_succeeds();
  ret += test_queue_enqueue_overflow_fails();
  ret += test_queue_dequeue_underflow_fails();

  printf("Test %s\n", (ret > 0) ? "FAILED" : "PASSED");

  return ret;
}
