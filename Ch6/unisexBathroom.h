
#include <stdio.h>
#include <pthread.h>
#include <stdbool.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>

#define MAX_PEOPLE 30
#define MAX_TIME_BATHROOM 30
//This struct stores the complete state at any time
struct State
{
  //how many people waiting to go in the bathroom:
  int womenQueue;
  int menQueue;
  //how many people inside the bathroom
  int menInside;
  int womenInside;

} state;

void printState()
{
  printf("*******************************\n");
  printf("*State:                       *\n");
  printf("* %i  women's queue           *\n",state.womenQueue);
  printf("* %i  men's queue             *\n",state.menQueue);
  printf("* %i  men in the bathroom     *\n",state.menInside);
  printf("* %i  women in the bathroom   *\n",state.womenInside);
  printf("*******************************\n");
}

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

bool womanCanEnter()
{
  if (state.womenInside < 3 && state.menInside == 0)
     return true;
  return false;
}

bool manCanEnter()
{
  if (state.menInside < 3 && state.womenInside == 0)
     return true;
  return false;
}

void enterWoman()
{
  printf("A woman has just entered the bathroom \n");
  state.womenInside+=1;
  state.womenQueue-=1;
}

void enterMan()
{
  printf("A man has just entered the bathroom \n");
  state.menInside+=1;
  state.menQueue-=1;
}

void leaveWoman()
{
  printf("A woman has just left the bathroom \n");
  state.womenInside-=1;
}

void leaveMan()
{
  printf("A man has just left the bathroom \n");
  state.menInside-=1;
}

void useBathroom()
{
  printf("Using bathroom.... \n");
  sleep(rand()%8);
}

//thread to represent a man using the bathroom
void *manThread()
{
    //sleep for up to 2s to add some randomness
    usleep(rand()%2000000);
    pthread_mutex_lock(&mutex);
    //thread does not continue until it can board
    while (!manCanEnter())
        pthread_cond_wait(&cond, &mutex);
    enterMan();
    printState();
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&mutex);

    useBathroom();

    pthread_mutex_lock(&mutex);
    leaveMan();
    printState();
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&mutex);

    //hacker thread has finished its work, it can modify the cond variable and
    //unlock the mutex

    return NULL;
}

//thread to represent a woman using the bathroom
void *womanThread()
{
    //sleep for up to 2s to add some randomness
    usleep(rand()%2000000);
    pthread_mutex_lock(&mutex);
    //thread does not continue until the woman can enter
    while (!womanCanEnter())
        pthread_cond_wait(&cond, &mutex);
    enterWoman();
    printState();
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&mutex);

    useBathroom();

    pthread_mutex_lock(&mutex);
    leaveWoman();
    printState();
    //woman has finished her business, cond can be triggered and mutex can
    //be unlocked
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&mutex);

    return NULL;
}
