//7.3 Room Party problem
#include "roomParty.h"

int main()
{
  srand(time(0));
  state.studentsInTheRoom = 0;
  state.deanInTheRoom = false;

  pthread_t students[N_STUDENTS];
  pthread_t dean;
  pthread_create(&dean,NULL,deanThread,NULL);
  for(int i = 0 ; i < N_STUDENTS; i++)
  {
    pthread_create(&students[i],NULL,studentThread,NULL);
  }

  //to close the program ctrl+c
  while (1)
  {

  }

  return(0);
}
