#include <stdio.h>
#include <pthread.h>
#include <stdbool.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>

#define N_STUDENTS 60
#define MAX_SEARCH_TIME 10
#define MAX_PARTY_TIME 1
#define MAX_DEAN_BREAK_TIME 10

//This struct stores the complete state at any time
struct State
{
  bool deanInTheRoom;
  int studentsInTheRoom;

} state;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

void printState()
{
  printf("*******************************\n");
  printf("*State:                       *\n");
  printf("*The  dean is%s in the room   *\n",state.deanInTheRoom?"":" not");
  printf("* %i  students in the room    *\n",state.studentsInTheRoom);
  printf("*******************************\n");
}

bool deanCanEnter()
{
  if (state.studentsInTheRoom == 0 || state.studentsInTheRoom >= 50 )
     return true;
  return false;
}

bool studentCanEnter()
{
  //only enter if no dean
  if (!state.deanInTheRoom)
     return true;
  return false;
}

bool deanCanLeave()
{
  //only leaves if there are no students left
  if(state.studentsInTheRoom ==0)
    return true;
  return false;
}

void enterDean()
{
  printf("The dean has just entered the room\n");
  state.deanInTheRoom=true;
}

void enterStudent()
{
  printf("A student has just entered the room\n");
  state.studentsInTheRoom+=1;
}

void leaveStudent()
{
  printf("A student has just left the room\n");
  state.studentsInTheRoom-=1;
}

void leaveDean()
{
  printf("The dean has just left the room \n");
  state.deanInTheRoom=false;
}

void conductSearch()
{
  printf("The dean is conducting a thorough search in the room\n");
  sleep(rand()%MAX_SEARCH_TIME + 1);
}

void breakupParty()
{
  printf("The dean is breaking up the party\n");
}

void party()
{
  printf("The student is partying\n");
  sleep(rand()%MAX_PARTY_TIME + 1);
}

//thread to represent a man using the bathroom
void *deanThread()
{
    while(1)
    {
      //sleep for up to 2s to add some randomness
      usleep(rand()%2000000);
      //thread does not continue until the dean can enter
      pthread_mutex_lock(&mutex);
      while (!deanCanEnter())
            pthread_cond_wait(&cond, &mutex);
      enterDean();
      printState();
      pthread_cond_signal(&cond);
      pthread_mutex_unlock(&mutex);

      if(state.studentsInTheRoom == 0)
        conductSearch();
      if(state.studentsInTheRoom >=50)
        breakupParty();

    //we need a mutex here otherwise we might have bad values. Compare + sum is
    // not an atomic operation
    pthread_mutex_lock(&mutex);
    while(!deanCanLeave())
        pthread_cond_wait(&cond, &mutex);
    leaveDean();
    printState();
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&mutex);
    sleep(rand()%MAX_DEAN_BREAK_TIME);
  }
  return NULL;
}

//thread to represent a student
void *studentThread()
{
    while(1)
    {
      //sleep for up to 2s to add some randomness
      sleep(1);
      //thread does not continue until the student can enter the room
      pthread_mutex_lock(&mutex);
      while (!studentCanEnter())
          pthread_cond_wait(&cond, &mutex);
      enterStudent();
      printState();
      pthread_cond_signal(&cond);
      pthread_mutex_unlock(&mutex);
    //  pthread_cond_signal(&cond);
    //  pthread_mutex_unlock(&mutex);

      party();
      pthread_mutex_lock(&mutex);
    //  pthread_mutex_lock(&mutex);
      leaveStudent();
      printState();
      pthread_cond_signal(&cond);
      pthread_mutex_unlock(&mutex);
      continue;
    }
    return NULL;
}
