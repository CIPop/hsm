#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <test_assert.h>

#include <hfsm_stack.h>

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
  T_INTERNAL_0 = HFSM_EVENT(1),
  T_INTERNAL_1 = HFSM_EVENT(2),
  T_INTERNAL_2 = HFSM_EVENT(3),
  T_SUPER_1 = HFSM_EVENT(4),
  T_SUPER_2 = HFSM_EVENT(5),
  T_PEER_0 = HFSM_EVENT(6),
  T_PEER_1 = HFSM_EVENT(7),
  T_PEER_2 = HFSM_EVENT(8),
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
      ref22++;
      break;

    case HFSM_EXIT:
      ref22--;
      break;

    case T_SUPER_2:
      ret = hfsm_transition_superstate(me, S22, S11);
      break;

    default:
      ret = S11(me, event, NULL);
  }

  return ret;
}

static hfsm_event tinternal0_evt = { T_INTERNAL_0, NULL };
static hfsm_event tinternal1_evt = { T_INTERNAL_1, NULL };
static hfsm_event tinternal2_evt = { T_INTERNAL_2, NULL };

static hfsm_event tpeer0_evt = { T_PEER_0, NULL };
static hfsm_event tpeer1_evt = { T_PEER_1, NULL };
static hfsm_event tpeer2_evt = { T_PEER_2, NULL };

static hfsm_event tsuper1_evt = { T_SUPER_1, NULL };
static hfsm_event tsuper2_evt = { T_SUPER_2, NULL };

int test_hfsm_stack_internal_transitions()
{
  // Init, TSub0, TSub1, TSub2
  ASSERT_TRUE(!hfsm_init(&test_hfsm, S01));
  ASSERT_TRUE(test_hfsm.current_state == S21);
  ASSERT_TRUE(ref01 == 1 && ref11 == 1 && ref21 == 1);

  // TInternal2
  ASSERT_TRUE(!hfsm_post_event(&test_hfsm, tinternal2_evt));
  ASSERT_TRUE(tinternal2 == 1);

  // TInternal1 S21
  ASSERT_TRUE(!hfsm_post_event(&test_hfsm, tinternal1_evt));
  ASSERT_TRUE(tinternal2 == 1 && tinternal1 == 1);

  // TInternal0 S21
  ASSERT_TRUE(!hfsm_post_event(&test_hfsm, tinternal0_evt));
  ASSERT_TRUE(tinternal2 == 1 && tinternal1 == 1 && tinternal0 == 1);

  // TPeer2 S21
  ASSERT_TRUE(!hfsm_post_event(&test_hfsm, tpeer2_evt));
  ASSERT_TRUE(test_hfsm.current_state == S22);
  ASSERT_TRUE(ref01 == 1 && ref11 == 1 && ref22 == 1);
  ASSERT_TRUE(ref21 == 0);

  // TInternal1 S22
  ASSERT_TRUE(!hfsm_post_event(&test_hfsm, tinternal1_evt));
  ASSERT_TRUE(tinternal1 == 2);

  // TInternal0 S22
  ASSERT_TRUE(!hfsm_post_event(&test_hfsm, tinternal0_evt));
  ASSERT_TRUE(tinternal1 == 2 && tinternal0 == 2);
  ASSERT_TRUE(tinternal2 == 1);

  // TSuper2 S22
  ASSERT_TRUE(!hfsm_post_event(&test_hfsm, tsuper2_evt));
  ASSERT_TRUE(test_hfsm.current_state == S11);
  ASSERT_TRUE(ref01 == 1 && ref11 == 1);
  ASSERT_TRUE(ref21 == 0 && ref22 == 0);

  // TInternal1 S11
  ASSERT_TRUE(!hfsm_post_event(&test_hfsm, tinternal1_evt));
  ASSERT_TRUE(tinternal1 == 3);

  // TInternal0 S11
  ASSERT_TRUE(!hfsm_post_event(&test_hfsm, tinternal0_evt));
  ASSERT_TRUE(tinternal1 == 3 && tinternal0 == 3);

  // TPeer1 S11
  ASSERT_TRUE(!hfsm_post_event(&test_hfsm, tpeer1_evt));
  ASSERT_TRUE(test_hfsm.current_state == S12);
  ASSERT_TRUE(ref01 == 1 && ref12 == 1);
  ASSERT_TRUE(ref11 == 0 && ref21 == 0 && ref22 == 0);

  // TInternal0 S12
  ASSERT_TRUE(!hfsm_post_event(&test_hfsm, tinternal0_evt));
  ASSERT_TRUE(tinternal0 == 4);

  // TSuper1 S12
  ASSERT_TRUE(!hfsm_post_event(&test_hfsm, tsuper1_evt));
  ASSERT_TRUE(test_hfsm.current_state == S01);
  ASSERT_TRUE(ref01 == 1);
  ASSERT_TRUE(ref11 == 0 && ref12 == 0 && ref21 == 0 && ref22 == 0);

  // TInternal0 S01
  ASSERT_TRUE(!hfsm_post_event(&test_hfsm, tinternal0_evt));
  ASSERT_TRUE(tinternal0 == 5);

  return 0;
}

int init_peer_transitions_state_s21()
{
  ref01 = 0;
  ref02 = 0;
  ref11 = 0;
  ref12 = 0;
  ref21 = 0;
  ref22 = 0;

    // Init, TSub0, TSub1, TSub2
  ASSERT_TRUE(!hfsm_init(&test_hfsm, S01));
  ASSERT_TRUE(test_hfsm.current_state == S21);
  ASSERT_TRUE(ref01 == 1 && ref11 == 1 && ref21 == 1);

  return 0;
}

int test_hfsm_stack_peer_transitions()
{
  // TPeer1 S21
  if(init_peer_transitions_state_s21()) return 1;
  ASSERT_TRUE(!hfsm_post_event(&test_hfsm, tpeer1_evt));
  ASSERT_TRUE(test_hfsm.current_state == S12);
  ASSERT_TRUE(ref01 == 1 && ref12 == 1);
  ASSERT_TRUE(ref11 == 0 && ref21 == 0 && ref22 == 0);

  // TPeer1 S22
  if(init_peer_transitions_state_s21()) return 1;
  ASSERT_TRUE(!hfsm_post_event(&test_hfsm, tpeer2_evt));
  ASSERT_TRUE(test_hfsm.current_state == S22);
  ASSERT_TRUE(!hfsm_post_event(&test_hfsm, tpeer1_evt));
  ASSERT_TRUE(test_hfsm.current_state == S12);
  ASSERT_TRUE(ref01 == 1 && ref12 == 1);
  ASSERT_TRUE(ref11 == 0 && ref21 == 0 && ref22 == 0);

  // TPeer0 S21
  if(init_peer_transitions_state_s21()) return 1;
  ASSERT_TRUE(!hfsm_post_event(&test_hfsm, tpeer0_evt));
  ASSERT_TRUE(test_hfsm.current_state == S02);
  ASSERT_TRUE(ref02 == 1);
  ASSERT_TRUE(ref01 == 0 && ref11 == 0 && ref12 == 0 && ref21 == 0 && ref22 == 0);

  // TPeer0 S22
  if(init_peer_transitions_state_s21()) return 1;
  ASSERT_TRUE(!hfsm_post_event(&test_hfsm, tpeer2_evt));
  ASSERT_TRUE(test_hfsm.current_state == S22);
  ASSERT_TRUE(!hfsm_post_event(&test_hfsm, tpeer0_evt));
  ASSERT_TRUE(test_hfsm.current_state == S02);
  ASSERT_TRUE(ref02 == 1);
  ASSERT_TRUE(ref01 == 0 && ref11 == 0 && ref12 == 0 && ref21 == 0 && ref22 == 0);

  // TPeer0 S11
  if(init_peer_transitions_state_s21()) return 1;
  ASSERT_TRUE(!hfsm_post_event(&test_hfsm, tpeer2_evt));
  ASSERT_TRUE(!hfsm_post_event(&test_hfsm, tsuper2_evt));
  ASSERT_TRUE(test_hfsm.current_state == S11);
  ASSERT_TRUE(!hfsm_post_event(&test_hfsm, tpeer0_evt));
  ASSERT_TRUE(test_hfsm.current_state == S02);
  ASSERT_TRUE(ref02 == 1);
  ASSERT_TRUE(ref01 == 0 && ref11 == 0 && ref12 == 0 && ref21 == 0 && ref22 == 0);

  // TPeer0 S12
  if(init_peer_transitions_state_s21()) return 1;
  ASSERT_TRUE(!hfsm_post_event(&test_hfsm, tpeer2_evt));
  ASSERT_TRUE(!hfsm_post_event(&test_hfsm, tsuper2_evt));
  ASSERT_TRUE(!hfsm_post_event(&test_hfsm, tpeer1_evt));
  ASSERT_TRUE(test_hfsm.current_state == S12);
  ASSERT_TRUE(!hfsm_post_event(&test_hfsm, tpeer0_evt));
  ASSERT_TRUE(test_hfsm.current_state == S02);
  ASSERT_TRUE(ref02 == 1);
  ASSERT_TRUE(ref01 == 0 && ref11 == 0 && ref12 == 0 && ref21 == 0 && ref22 == 0);

  // TPeer0 S01
  if(init_peer_transitions_state_s21()) return 1;
  ASSERT_TRUE(!hfsm_post_event(&test_hfsm, tpeer2_evt));
  ASSERT_TRUE(!hfsm_post_event(&test_hfsm, tsuper2_evt));
  ASSERT_TRUE(!hfsm_post_event(&test_hfsm, tpeer1_evt));
  ASSERT_TRUE(!hfsm_post_event(&test_hfsm, tsuper1_evt));
  ASSERT_TRUE(test_hfsm.current_state == S01);
  ASSERT_TRUE(!hfsm_post_event(&test_hfsm, tpeer0_evt));
  ASSERT_TRUE(test_hfsm.current_state == S02);
  ASSERT_TRUE(ref02 == 1);
  ASSERT_TRUE(ref01 == 0 && ref11 == 0 && ref12 == 0 && ref21 == 0 && ref22 == 0);

  return 0;
}

int main()
{

  int ret = 0;
  ret += test_hfsm_stack_internal_transitions();
  ret += test_hfsm_stack_peer_transitions();

  printf("Test %s\n", (ret > 0) ? "FAILED" : "PASSED");

  return ret;
}
