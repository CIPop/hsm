#include <stdio.h>
#include <stdint.h>
#include <assert.h>

#include <test_assert.h>

#include <hfsm_stack.h>

// Event Data Structure, specific to TestHFSM.
typedef struct evt_struct
{
  uint8_t type;
  char string[10];
} evt_struct;

static hfsm test_hfsm;

// TestHFSM States
static int S01(hfsm* me, hfsm_event event, int(** super_state)());
static int S02(hfsm* me, hfsm_event event, int(** super_state)());
static int S11(hfsm* me, hfsm_event event, int(** super_state)());
static int S12(hfsm* me, hfsm_event event, int(** super_state)());
static int S21(hfsm* me, hfsm_event event, int(** super_state)());
static int S22(hfsm* me, hfsm_event event, int(** super_state)());

// TestHFSM-specific events.
typedef enum
{
  T_INTERNAL_0 = HFSM_EVENT(0),
  T_INTERNAL_1 = HFSM_EVENT(1),
  T_INTERNAL_2 = HFSM_EVENT(2),
  T_SUPER_1 = HFSM_EVENT(3),
  T_SUPER_2 = HFSM_EVENT(4),
  T_PEER_0 = HFSM_EVENT(5),
  T_PEER_1 = HFSM_EVENT(6),
  T_PEER_2 = HFSM_EVENT(7),
} test_hfsm_event_type;

static int ref01 = 0;
static int ref02 = 0;
static int ref11 = 0;
static int ref12 = 0;
static int ref21 = 0;
static int ref22 = 0;
static int tinternal0 = 0;
static int tinternal1 = 0;
static int tinternal2 = 0;

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
      ret = hfsm_transition_substate(me, S01, S11);
      break;

    case HFSM_EXIT:
      ref01--;
      break;

    case T_PEER_0:
      ret = hfsm_transition_peer(me, S01, S02);
      break;

    case T_INTERNAL_0:
      tinternal0++;
      break;

    default:
      printf("Unknown event %d", event.type);
      assert(0);
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
      printf("Unknown event %d", event.type);
      assert(0);
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
      ret = hfsm_transition_substate(me, S11, S21);
      break;

    case HFSM_EXIT:
      ref11--;
      break;

    case T_PEER_1:
      ret = hfsm_transition_peer(me, S11, S12);
      break;

    case T_INTERNAL_1:
      tinternal1++;
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

    case T_SUPER_1:
      ret = hfsm_transition_superstate(me, S12, S01);
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
      ref21++;
      break;

    case HFSM_EXIT:
      ref21--;
      break;

    case T_PEER_2:
      ret = hfsm_transition_peer(me, S21, S22);
      break;

    case T_INTERNAL_2:
      tinternal2++;
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

    case T_SUPER_2:
      ret = hfsm_transition_superstate(me, S22, S11);
      break;

    default:
      ret = S11(me, event, NULL);
  }

  return ret;
}


int test_hfsm_stack_internal()
{
  ASSERT_TRUE(!hfsm_init(&test_hfsm, S01));

  return ret;
}

int main()
{

  int ret = 0;
  ret += test_hfsm_stack_internal();

  printf("Test %s\n", (ret > 0) ? "FAILED" : "PASSED");

  return ret;
}
