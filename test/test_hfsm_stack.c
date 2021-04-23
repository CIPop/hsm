#include <stdio.h>
#include <stdint.h>

#include <hfsm_stack.h>

typedef struct evt_struct
{
  uint8_t type;
  char string[10];
} evt_struct;

#define Q_SIZE 2
#define Q_TYPE evt_struct
#include <queue.h>

static hfsm test_hfsm;

static int idle(hfsm* me, hfsm_event event, int(** super_state)());
static int connected(hfsm* me, hfsm_event event, int(** super_state)());
static int subscribing(hfsm* me, hfsm_event event, int(** super_state)());
static int subscribed(hfsm* me, hfsm_event event, int(** super_state)());
static int disconnecting(hfsm* me, hfsm_event event, int(** super_state)());

typedef enum
{
  PAHO_IOT_hfsm_CONNECTED = _hfsm_EVENT(1),
  PAHO_IOT_hfsm_SUBSCRIBED = _hfsm_EVENT(2),
} paho_iot_hfsm_event_type;

//
// Logging
//
#define LOG_ERROR(...)                                                  \
  do                                                                               \
  {                                                                                \
    (void)fprintf(stderr, "ERROR:\t\t%s:%s():%d: ", __FILE__, __func__, __LINE__); \
    (void)fprintf(stderr, __VA_ARGS__);                                            \
    (void)fprintf(stderr, "\n");                                                   \
    fflush(stdout);                                                                \
    fflush(stderr);                                                                \
  } while (0)

#define LOG_SUCCESS(...) \
  do                                \
  {                                 \
    (void)printf("SUCCESS:\t");     \
    (void)printf(__VA_ARGS__);      \
    (void)printf("\n");             \
  } while (0)

// Testhfsm/Idle
static int idle(hfsm* me, hfsm_event event, int(** super_state)())
{
  int ret = AZ_OK;
  if (super_state)
  {
    *super_state = NULL; // Top-level state.
  }

  switch ((int)event.type)
  {
    case hfsm_ENTRY:
      LOG_SUCCESS("%s: event hfsm_ENTRY", __func__);
      break;

    case hfsm_EXIT:
      LOG_SUCCESS("%s: event hfsm_EXIT", __func__);
      break;

    case PAHO_IOT_hfsm_CONNECTED:
      LOG_SUCCESS("%s: event PAHO_IOT_hfsm_CONNECTED", __func__);
      ret = hfsm_transition(me, idle, connected);
      break;

    default:
      // TOP level - ignore unknown events.
      LOG_ERROR("%s: dropped unknown event: 0x%x", __func__, event.type);
      ret = AZ_OK;
  }

  return ret;
}

// Testhfsm/Connected
static int connected(hfsm* me, hfsm_event event, int(** super_state)())
{
  int ret = AZ_OK;
  if (super_state)
  {
    *super_state = NULL; // Top-level state.
  }

  switch (event.type)
  {
    case hfsm_ENTRY:
      LOG_SUCCESS("%s: event hfsm_ENTRY", __func__);
      // TODO: is the subsribing state really needed?
      ret = hfsm_transition_inner(me, connected, subscribing);
      break;

    case hfsm_EXIT:
      LOG_SUCCESS("%s: event hfsm_EXIT", __func__);
      break;

    case hfsm_ERROR:
      LOG_ERROR("%s: event hfsm_ERROR", __func__);
      // Handled for all inner states (i.e. subscribing, subscribed).
      ret = hfsm_transition(me, connected, disconnecting);

      // TODO: move to UT :Test transition from super-class to inner.
      //ret = hfsm_transition_inner(me, connected, subscribing);
      break;

    default:
      // TOP level - ignore unknown events.
      LOG_ERROR("%s: dropped unknown event: 0x%x", __func__, event.type);
      ret = AZ_OK;
  }

  return ret;
}

// Testhfsm/Connected/Subscribing
static int subscribing(hfsm* me, hfsm_event event, int(** super_state)())
{
  int ret = AZ_OK;
  if (super_state)
  {
    *super_state = connected;
  }

  switch ((int)event.type)
  {
    case hfsm_ENTRY:
      LOG_SUCCESS("%s: event hfsm_ENTRY", __func__);
      break;

    case hfsm_EXIT:
      LOG_SUCCESS("%s: event hfsm_EXIT", __func__);
      break;

    case PAHO_IOT_hfsm_SUBSCRIBED:
      LOG_SUCCESS("%s: event PAHO_IOT_hfsm_SUBSCRIBED", __func__);
      ret = hfsm_transition(me, subscribing, subscribed);
      break;

    default:
      // Handle using super-state.
      LOG_SUCCESS("%s: handling event using super-state", __func__);
      ret = connected(me, event, NULL);
  }

  return ret;
}

// Testhfsm/Connected/Subscribed
static int subscribed(hfsm* me, hfsm_event event, int(** super_state)())
{
  int ret = AZ_OK;
  if (super_state)
  {
    *super_state = connected;
  }

  switch (event.type)
  {
    case hfsm_ENTRY:
      LOG_SUCCESS("%s: event hfsm_ENTRY", __func__);
      break;

    case hfsm_EXIT:
      LOG_SUCCESS("%s: event hfsm_EXIT", __func__);
      break;

    default:
      // Handle using super-state.
      LOG_SUCCESS("%s: handling event using super-state", __func__);
      ret = connected(me, event, NULL);
  }

  return ret;
}

// Testhfsm/Disconnecting
static int disconnecting(hfsm* me, hfsm_event event, int(** super_state)())
{
  int ret = AZ_OK;
  if (super_state)
  {
    *super_state = NULL; // Top-level state.
  }

  (void)me;

  switch (event.type)
  {
    case hfsm_ENTRY:
      LOG_SUCCESS("%s: event hfsm_ENTRY", __func__);
      break;

    case hfsm_EXIT:
      LOG_SUCCESS("%s: event hfsm_EXIT", __func__);
      break;

    default:
      // TOP level - ignore unknown events.
      LOG_ERROR("%s: dropped unknown event: 0x%x", __func__, event.type);
      ret = AZ_OK;
  }

  return ret;
}

static void test_hfsm_succeeds(void **state)
{
  (void)state;

  LOG_SUCCESS("--> Started.");

  if (int_failed(hfsm_init(&test_hfsm, idle)))
  {
    LOG_ERROR("Failed to init hfsm");
  }

  LOG_SUCCESS("--> Connected event.");

  // Memory must be allocated for the duration of the transition:
  hfsm_event connected_evt = { PAHO_IOT_hfsm_CONNECTED, NULL };
  if (int_failed(hfsm_post_sync(&test_hfsm, connected_evt)))
  {
    LOG_ERROR("Failed to post connected_evt");
  }

  LOG_SUCCESS("--> Subscribed event.");

  hfsm_event subscribed_evt = { PAHO_IOT_hfsm_SUBSCRIBED, NULL };
  if (int_failed(hfsm_post_sync(&test_hfsm, subscribed_evt)))
  {
    LOG_ERROR("Failed to post subscribed_evt");
  }

  LOG_SUCCESS("--> Simulating ERROR event.");
  hfsm_event error_evt = { hfsm_ERROR, NULL };
  if (int_failed(hfsm_post_sync(&test_hfsm, error_evt)))
  {
    LOG_ERROR("Failed to post subscribed_evt");
  }
}

int test_hfsm()
{
  const struct CMUnitTest tests[] = {
    cmocka_unit_test(test_hfsm_succeeds),
  };
  return cmocka_run_group_tests_name("hfsm", tests, NULL, NULL);
}
