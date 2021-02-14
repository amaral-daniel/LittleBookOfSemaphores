//6.2 unisex bathroom problem
#include "unisexBathroom.h"

int main()
{
  srand(time(0));
  int initialWomenQueue = rand()%MAX_PEOPLE + 1;
  int initialMenQueue = MAX_PEOPLE - initialWomenQueue;
  state.womenQueue = initialWomenQueue;
  state.menQueue = initialMenQueue;
  state.womenInside = 0;
  state.menInside = 0;

  pthread_t people[MAX_PEOPLE];
  for(int i = 0 ; i < initialWomenQueue; i++)
  {
    pthread_create(&people[i],NULL,womanThread,NULL);
  }
  for(int i = initialWomenQueue; i < initialMenQueue + initialWomenQueue; i++)
  {
    pthread_create(&people[i],NULL,manThread,NULL);
  }
  pthread_mutex_lock(&mutex);
  //finish program only we cannot send boats or if the last combination is
  //invalid (maybe it would be nice to plan for this last case)

  //do not continue while we can still have valid combinations
  while (state.womenQueue + state.menQueue != 0)
      pthread_cond_wait(&cond, &mutex);
  pthread_mutex_unlock(&mutex);

  printf("All the employees have succcessfully used the bathroom \n");

   printf("All possible passengers have completed their trips \n");
   printf("***********************\n");
   printf("* %i  women           *\n",initialWomenQueue);
   printf("* %i  men             *\n",initialMenQueue);
   printf("***********************\n");
  return(0);

}
