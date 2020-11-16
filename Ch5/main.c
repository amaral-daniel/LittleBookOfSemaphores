//Linux hackers and serfs (Microsoft employees) want to cross a river
//The ferry can only leave when there are exactly 4 people onboard
//It is not allowed to have neither of the following combinations: 3:1 and 1:3
//Each person is a thread and must call a function board(), all four threads must have
//invoked board() before the next threads of the next boatload do.

//After the boat is full, a function named rowBoat must be called by exactly
//1 thread (not more not fewer).

#include <stdio.h>
#include <pthread.h>
#include <stdbool.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>

#define MAX_PASSENGERS 100
//This struct stores the complete state at any time
struct State
{
  int hackersQueue;
  int serfsQueue;
  int hackersOnboard;
  int serfsOnboard;
  int freeSpots;
  int crossedHackers;
  int crossedSerfs;
} state;

void printState()
{
  printf("***********************\n");
  printf("*State:               *\n");
  printf("* %i  hackers queue   *\n",state.hackersQueue);
  printf("* %i  serfs queue     *\n",state.serfsQueue);
  printf("* %i  free spots      *\n",state.freeSpots);
  printf("* %i  serfs onboard   *\n",state.serfsOnboard);
  printf("* %i  hackers onboard *\n",state.hackersOnboard);
  printf("***********************\n");
}

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

void row_boat()
{
  printf("Boat is sailing \n");
  state.crossedHackers += state.hackersOnboard;
  state.crossedSerfs += state.serfsOnboard;
  state.serfsOnboard = 0;
  state.hackersOnboard = 0;
  state.freeSpots = 4;
}

bool hackerCanBoard()
{
  if ((state.hackersOnboard ==0 && state.serfsOnboard ==3)
      ||(state.hackersOnboard ==2 && state.serfsOnboard ==1)
    ||(state.freeSpots == 0))
        return false;
  return true;
}

bool serfCanBoard()
{
  if ((state.serfsOnboard ==0 && state.hackersOnboard ==3)
      ||(state.serfsOnboard ==2 && state.hackersOnboard ==1)
    ||(state.freeSpots == 0))
    {
      printf("Poor serf cannot board \n");
        return false;
    }
  return true;
}

void boardHacker()
{
  printf("A hacker has just boarded \n");
  state.hackersQueue-=1;
  state.freeSpots-=1;
  state.hackersOnboard+=1;
}

void boardSerf()
{
  printf("A serf has just boarded \n");
  state.serfsQueue-=1;
  state.freeSpots-=1;
  state.serfsOnboard+=1;
}

void *hackerThread()
{
  usleep(rand()%2000000);
  pthread_mutex_lock(&mutex);

  while (!hackerCanBoard())
      pthread_cond_wait(&cond, &mutex);
  boardHacker();
  printState();
  if(state.freeSpots == 0)
  {
    row_boat();
    printState();
  }
  pthread_cond_signal(&cond);
  pthread_mutex_unlock(&mutex);
  return NULL;
}

void *serfThread()
{
  usleep(rand()%2000000);
  pthread_mutex_lock(&mutex);

  while (!serfCanBoard())
      pthread_cond_wait(&cond, &mutex);
  boardSerf();
  printState();
  if(state.freeSpots == 0)
  {
    row_boat();
    printState();
  }
  pthread_cond_signal(&cond);
  pthread_mutex_unlock(&mutex);
  return NULL;
}


int main()
{

  int initialSerfsQueue = 6;
  int initialHackersQueue = 6;
  state.hackersQueue = initialSerfsQueue;
  state.serfsQueue = initialHackersQueue;
  state.hackersOnboard = 0;
  state.serfsOnboard = 0;
  state.freeSpots = 4;
  state.crossedHackers = 0;
  state.crossedSerfs = 0;



  pthread_t passengers[MAX_PASSENGERS];
  for(int i = 0 ; i < initialSerfsQueue; i++)
  {
    pthread_create(&passengers[i],NULL,hackerThread,NULL);
  }
  for(int i = state.hackersQueue; i < initialHackersQueue + initialSerfsQueue; i++)
  {
    pthread_create(&passengers[i],NULL,serfThread,NULL);
  }
  pthread_mutex_lock(&mutex);
  while (state.hackersQueue != 0 || state.serfsQueue != 0)
      pthread_cond_wait(&cond, &mutex);
  pthread_mutex_unlock(&mutex);
  printf("All possible passengers have completed their trips \n");
  return(0);

}
