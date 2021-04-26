#include <stdio.h>
#include <stdint.h>
#include <test_assert.h>

#include <hfsm_stack.h>

typedef struct evt_struct
{
  uint8_t type;
  char string[10];
} evt_struct;

static hfsm test_hfsm;

static int S01(hfsm* me, hfsm_event event, int(** super_state)());
static int S02(hfsm* me, hfsm_event event, int(** super_state)());
static int S11(hfsm* me, hfsm_event event, int(** super_state)());
static int S12(hfsm* me, hfsm_event event, int(** super_state)());
static int S21(hfsm* me, hfsm_event event, int(** super_state)());
static int S22(hfsm* me, hfsm_event event, int(** super_state)());

static int ref01 = 0;
static int ref02 = 0;
static int ref11 = 0;
static int ref12 = 0;
static int ref21 = 0;
static int ref22 = 0;

// TestHFSM/S01
static int S01(hfsm* me, hfsm_event event, int(** super_state)())
{
  int ret = 0;
  if (super_state)
  {
    *super_state = NULL; // Top-level state.
  }

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
static int S02(hfsm* me, hfsm_event event, int(** super_state)())
{
  int ret = 0;
  if (super_state)
  {
    *super_state = NULL; // Top-level state.
  }

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
static int S11(hfsm* me, hfsm_event event, int(** super_state)())
{
  int ret = 0;
  if (super_state)
  {
    *super_state = S01;
  }

  switch ((int)event.type)
  {
    case HFSM_ENTRY:
      ref11++;
      break;

    case HFSM_EXIT:
      ref11--;
      break;

    default:
      ret = S01(me, event, NULL);
  }

  return ret;
}

// TestHFSM/S01/S12
static int S12(hfsm* me, hfsm_event event, int(** super_state)())
{
  int ret = 0;
  if (super_state)
  {
    *super_state = S01;
  }

  switch ((int)event.type)
  {
    case HFSM_ENTRY:
      ref12++;
      break;

    case HFSM_EXIT:
      ref12--;
      break;

    default:
      ret = S01(me, event, NULL);
  }

  return ret;
}

// TestHFSM/S01/S11/S21
static int S21(hfsm* me, hfsm_event event, int(** super_state)())
{
  int ret = 0;
  if (super_state)
  {
    *super_state = S11;
  }

  switch ((int)event.type)
  {
    case HFSM_ENTRY:
      ref01++;
      break;

    case HFSM_EXIT:
      ref01--;
      break;

    default:
      ret = S11(me, event, NULL);
  }

  return ret;
}

// TestHFSM/S01/S11/S22
static int S22(hfsm* me, hfsm_event event, int(** super_state)())
{
  int ret = 0;
  if (super_state)
  {
    *super_state = S11;
  }

  switch ((int)event.type)
  {
    case HFSM_ENTRY:
      ref01++;
      break;

    case HFSM_EXIT:
      ref01--;
      break;

    default:
      ret = S11(me, event, NULL);
  }

  return ret;
}


int test_hfsm_stack_internal()
{

  return 0;
}

int main()
{

  int ret = 0;
  ret += test_hfsm_stack_internal();

  printf("Test %s\n", (ret > 0) ? "FAILED" : "PASSED");

  return ret;
}
