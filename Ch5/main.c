//5.7.2 River crossing problem
#include "riverCrossing.h"

int main()
{
  srand(time(0));
  //initialize states. Better comments in riverCrossing.h
  int initialSerfsQueue = rand()%MAX_PASSENGERS;
  int initialHackersQueue = MAX_PASSENGERS - initialSerfsQueue;
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
  for(int i = initialSerfsQueue; i < initialHackersQueue + initialSerfsQueue; i++)
  {
    pthread_create(&passengers[i],NULL,serfThread,NULL);
  }
  pthread_mutex_lock(&mutex);

  //finish program only we cannot send boats or if the last combination is
  //invalid
  while (canFillNewBoat())
      pthread_cond_wait(&cond, &mutex);
  pthread_mutex_unlock(&mutex);

  printf("All possible passengers have completed their trips \n");
  printf("***********************\n");
  printf("* %i  hackers crossed *\n",state.crossedHackers);
  printf("* %i  serfs crossed   *\n",state.crossedSerfs);
  printf("***********************\n");
  return(0);

}
