/*
 * Simple queue template.
 */

#ifndef QUEUE_H
#define QUEUE_H

/* Simple queue parameters - these must be defined by the compilation unit prior to including the template. */
#ifndef Q_SIZE
#define Q_SIZE 5
#endif

#ifndef Q_TYPE
#include <stdint.h>
#define Q_TYPE int32_t
#endif

#ifndef Q_IDX_TYPE
#define Q_IDX_TYPE int32_t
#endif

typedef struct az_iot_queue
{
  Q_TYPE data[Q_SIZE];
  Q_IDX_TYPE start_idx;
  Q_IDX_TYPE end_idx;
  Q_IDX_TYPE count;
} az_iot_queue;

void queue_init(az_iot_queue* q)
{
  q->count = 0;
  q->start_idx = 0;
  q->end_idx = 0;
}

bool queue_enqueue(az_iot_queue* q, Q_TYPE* element)
{
  if (q->count < Q_SIZE)
  {
    q->data[q->end_idx] = *element;
    q->end_idx = (q->end_idx + 1) % Q_SIZE;
    q->count++;

    return true;
  }

  return false;
}

 Q_TYPE* queue_dequeue(az_iot_queue* q)
{
  Q_TYPE* ret = NULL;
  if (q->count > 0)
  {
    ret = &(q->data[q->start_idx]);
    q->start_idx = (q->start_idx + 1) % Q_SIZE;
    q->count--;
  }

  return ret;
}

#endif //!QUEUE_H
