
#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <test_assert.h>

#include <hfsm_hardcoded.h>

typedef struct evt_struct
{
  uint8_t type;
  char string[10];
} evt_struct;

static hfsm test_hfsm;

static int S01(hfsm* me, hfsm_event event);
static int S02(hfsm* me, hfsm_event event);
static int S11(hfsm* me, hfsm_event event);
static int S12(hfsm* me, hfsm_event event);
static int S21(hfsm* me, hfsm_event event);
static int S22(hfsm* me, hfsm_event event);

static int ref01 = 0;
static int ref02 = 0;
static int ref11 = 0;
static int ref12 = 0;
static int ref21 = 0;
static int ref22 = 0;

// Hardcoded TestHFSM hierarchy structure
static state_handler test_hfsm_get_parent(state_handler child_state)
{
  if ((child_state == S01) || (child_state == S02))
  {
    return NULL;
  }

  if ((child_state == S11) || (child_state == S12))
  {
    return S01;
  }

  if ((child_state == S21) || (child_state == S22))
  {
    return S11;
  }

  // Unknown state.
  assert(0);
  return NULL;
}

// TestHFSM/S01
static int S01(hfsm* me, hfsm_event event)
{
  int ret = 0;

  switch ((int)event.type)
  {
    case HFSM_ENTRY:
      ref01++;
      break;

    case HFSM_EXIT:
      ref01--;
      break;

    default:
      // TOP level - ignore unknown events.
      ASSERT_TRUE(0);
  }

  return ret;
}

// TestHFSM/S02
static int S02(hfsm* me, hfsm_event event)
{
  int ret = 0;

  switch ((int)event.type)
  {
    case HFSM_ENTRY:
      ref02++;
      break;

    case HFSM_EXIT:
      ref02--;
      break;

    default:
      // TOP level - ignore unknown events.
      ASSERT_TRUE(0);
  }

  return ret;
}

// TestHFSM/S01/S11
static int S11(hfsm* me, hfsm_event event)
{
  int ret = 0;

  switch ((int)event.type)
  {
    case HFSM_ENTRY:
      ref11++;
      break;

    case HFSM_EXIT:
      ref11--;
      break;

    default:
      ret = S01(me, event);
  }

  return ret;
}

// TestHFSM/S01/S12
static int S12(hfsm* me, hfsm_event event)
{
  int ret = 0;

  switch ((int)event.type)
  {
    case HFSM_ENTRY:
      ref12++;
      break;

    case HFSM_EXIT:
      ref12--;
      break;

    default:
      ret = S01(me, event);
  }

  return ret;
}

// TestHFSM/S01/S11/S21
static int S21(hfsm* me, hfsm_event event)
{
  int ret = 0;

  switch ((int)event.type)
  {
    case HFSM_ENTRY:
      ref01++;
      break;

    case HFSM_EXIT:
      ref01--;
      break;

    default:
      ret = S11(me, event);
  }

  return ret;
}

// TestHFSM/S01/S11/S22
static int S22(hfsm* me, hfsm_event event)
{
  int ret = 0;

  switch ((int)event.type)
  {
    case HFSM_ENTRY:
      ref01++;
      break;

    case HFSM_EXIT:
      ref01--;
      break;

    default:
      ret = S11(me, event);
  }

  return ret;
}

int test_hfsm_hardcoded_internal()
{
  hfsm_init(&test_hfsm, S01, test_hfsm_get_parent);
  return 0;
}

int main()
{

  int ret = 0;
  ret += test_hfsm_hardcoded_internal();

  printf("Test %s\n", (ret > 0) ? "FAILED" : "PASSED");

  return ret;
}
