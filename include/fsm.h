#ifndef FSM_H
#define FSM_H

typedef enum {
  SLEEPING,
  READING_INPUT,
  ACCESS_GRANTED
} State;

void updateState();

void processUARTCommand();

#endif // FSM_H
