//3. DYNAMIC SCHEDULING WITH WORKER THREADS
#include <iostream>
#include <chrono>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

using namespace std;
#ifdef __cplusplus
extern "C" {
#endif

float f1(float x, int intensity);
float f2(float x, int intensity);
float f3(float x, int intensity);
float f4(float x, int intensity);


pthread_mutex_t mutex_iter, mutex_chunk, task_getter_mutex;
long next_available_beginning, next_available_end;
pthread_mutex_t thread_number_mutex;
int numberThread=0;
int nbthreads;

#ifdef __cplusplus
}
#endif
bool task_done = false;
int current_start = 0;
float summed_value = 0;

struct args  //Defines the Structure of arguments to be passed and dealt with during the course of the threading program.
{
  int f;
  float a;
  float b;
  int start;
  int end;
  int intensity;
  float pre_product;
  float inside_sum;
  float granularity;
  int n;
}current_args;

bool addThread() //addThread but don't allow exceed nbthreads
{
  pthread_mutex_lock(&thread_number_mutex); // Lock adding of the thread
  if(numberThread >= nbthreads)
  {
    pthread_mutex_unlock(&thread_number_mutex); // Unlock adding of thread
    return false;
  }
  numberThread +=1;
  pthread_mutex_unlock(&thread_number_mutex); // Unlock adding of thread
  return true;
}
float summation_iteration(int begin,int end)  //Summation function when iteration sync is passed
{
  //Variables for computing integration taken from globally set values (settings)
  int i;
  float function_eval;
  int f = current_args.f;
  float a = current_args.a;
  float b= current_args.b;
  int start = begin;
  int intensity = current_args.intensity;
  float pre_product = current_args.pre_product;
  float mysummed_valuei = 0;
  for(i=start;i<end;i++)  //Loop to deal with integration calculation and switch between functions
  {
    switch(f)
    {
      case 1:
        function_eval = f1(a + (i+0.5)*pre_product,intensity);
	break;
      case 2:
        function_eval = f2(a + (i+0.5)*pre_product,intensity);
        break;
      case 3:
        function_eval = f3(a + (i+0.5)*pre_product,intensity);
        break;
      case 4:
        function_eval = f4(a + (i+0.5)*pre_product,intensity);
        break;
    }

  pthread_mutex_lock(&mutex_iter);  //locking before each iteration summing to the global total sum
  summed_value += function_eval;   // adding to the global sum value
  pthread_mutex_unlock(&mutex_iter); // unlocking the above lock
  }	
  
  
  return summed_value;
}

float summation_thread(int begin,int end)  //Summation function when thread sync is passed
{
  int i;
  float function_eval;
  int f = current_args.f;
  float a = current_args.a;
  float b= current_args.b;
  int start = begin;
  int intensity = current_args.intensity;
  float pre_product = current_args.pre_product;
  float mysummed_value = 0;
  for(i=start;i<end;i++)
  {
    switch(f)
    {
      case 1:
        function_eval = f1(a + (i+0.5)*pre_product,intensity);
	break;
      case 2:
        function_eval = f2(a + (i+0.5)*pre_product,intensity);
        break;
      case 3:
        function_eval = f3(a + (i+0.5)*pre_product,intensity);
        break;
      case 4:
        function_eval = f4(a + (i+0.5)*pre_product,intensity);
        break;
    }
  mysummed_value += function_eval;
  }	
  return mysummed_value;  //coordinate using return value, for thread since the values are computed and combined in the end while joining.
}

float summation_chunk(int begin,int end) //when sync value is chunk
{
  int i;
  float function_eval;
  int f = current_args.f;
  float a = current_args.a;
  float b= current_args.b;
  int start = begin;
  int intensity = current_args.intensity;
  float pre_product = current_args.pre_product;
  float mysummed_value = 0;
  for(i=start;i<end;i++)
  {
    switch(f)
    {
      case 1:
        function_eval = f1(a + (i+0.5)*pre_product,intensity);
	break;
      case 2:
        function_eval = f2(a + (i+0.5)*pre_product,intensity);
        break;
      case 3:
        function_eval = f3(a + (i+0.5)*pre_product,intensity);
        break;
      case 4:
        function_eval = f4(a + (i+0.5)*pre_product,intensity);
        break;
    }
    mysummed_value += function_eval;
  }	

  pthread_mutex_lock(&mutex_chunk); //Locking to sum each of the chunks from each of the threads
  summed_value += mysummed_value;   //Summing the value to the global with each chunk
  pthread_mutex_unlock(&mutex_chunk); //unlocking above mutex
  return summed_value;
}

void* get_next_task(long &begin, long &end, bool &task_test)
{
  //TASK FETCHER FUNCTION
  //LOCK MUTEX before fetching next range to sum for any of the sync tasks
  pthread_mutex_lock(&task_getter_mutex);
  task_test = task_done; //Do a task Done or not Check
  begin = next_available_beginning; //Give asker the next available beginning
  end = next_available_end;  //Give the asker the next available end
  if(end == current_args.n)  //check if the end is equal the the n value
  {
    task_done = true;   //check whether task was done
    pthread_mutex_unlock(&task_getter_mutex);
    return NULL;
  }
  next_available_beginning = next_available_beginning + (int)current_args.granularity;
  if(current_args.n - (next_available_beginning + current_args.granularity) >= current_args.granularity)  //Do a check to see if the granularity overshoots, if so, get it to n alone and not beyond.
  {
    next_available_end = next_available_beginning + (int)current_args.granularity;
  }
  else
    next_available_end = current_args.n;

  pthread_mutex_unlock(&task_getter_mutex); //unlock mutex
  //UNLOCK MUTEX
  return NULL;
	 
}	

void* worker_threads_iteration(void* i) // iteration worker thread spawn
{
  bool task_test;  // check if the task was done
  bool nothread = addThread();
  pthread_mutex_lock(&task_getter_mutex);  // Lock task getter mutex
  task_test = task_done;
  pthread_mutex_unlock(&task_getter_mutex);// Lock before accessing task test
  pthread_t temp_thread;
  bool threadSpawned = false;
  std::chrono::time_point<std::chrono::system_clock> start_thread_clock, end_thread_clock;  //Start Clock
   
  if(task_test!= true)
  {
    threadSpawned = true;
    long i = (long)i;
  start_thread_clock = std::chrono::system_clock::now();
    
    pthread_create(&temp_thread,NULL,worker_threads_iteration,(void*)i);
  }
  
  while(task_test!=true)
  {
    long begin,end;
    float mysum;
    get_next_task(begin,end,task_test);    // Get next task - but this locks out the task test variable.
    if(task_test == true)
      return NULL;
    summation_iteration(begin,end);        // Iteration summation occurs here.	
    pthread_mutex_lock(&task_getter_mutex);  // Lock before getting task test
    task_test = task_done;
    pthread_mutex_unlock(&task_getter_mutex);  // Unlock task getter mutex
  }
  
  if(threadSpawned == true)
  {

     end_thread_clock = std::chrono::system_clock::now();
  std::chrono::duration<double> elapsed_seconds = end_thread_clock-start_thread_clock;  //End time Processed
  cout<<endl<<"SUB THREAD: "<<elapsed_seconds.count();  //Time difference
    pthread_join(temp_thread,NULL);
  }
  
  return NULL;

}
void* worker_threads_thread(void* mysum)
{
  bool task_test;  // check if the task was done
  bool nothread = addThread();
  pthread_mutex_lock(&task_getter_mutex);  // Lock task getter mutex
  task_test = task_done;
  pthread_mutex_unlock(&task_getter_mutex);// Lock before accessing task test
  pthread_t temp_thread;
  bool threadSpawned = false;
  float this_sum = 0; 
  std::chrono::time_point<std::chrono::system_clock> start_thread_clock, end_thread_clock;  //Start Clock
   
  if(task_test!= true)
  {
    threadSpawned = true;
  start_thread_clock = std::chrono::system_clock::now();
    pthread_create(&temp_thread,NULL,worker_threads_iteration,(void*)&this_sum);
  }


  //similar to worker thread iteration excepting allowance to get return value of summation.
  //bool task_test;
  pthread_mutex_lock(&task_getter_mutex);
  task_test = task_done;
  pthread_mutex_unlock(&task_getter_mutex);
  
  while(task_test!=true)
  {
    long begin,end;
    get_next_task(begin,end,task_test);
    if(task_test == true)
      return NULL;
      float temp_sum = summation_thread(begin,end);  //get summed value in variable
      (*((float*)mysum)) += temp_sum;               //Equate pointer output to variable to manifest in main as reference	
      pthread_mutex_lock(&task_getter_mutex);
      task_test = task_done;
      pthread_mutex_unlock(&task_getter_mutex);
  }
  
  if(threadSpawned == true)
  {
    pthread_join(temp_thread,NULL);
     end_thread_clock = std::chrono::system_clock::now();
  std::chrono::duration<double> elapsed_seconds = end_thread_clock-start_thread_clock;  //End time Processed
  cout<<endl<<"SUB THREAD THREAD: "<<elapsed_seconds.count();  //Time difference
    (*((float*)mysum)) += this_sum;  //Equate pointer to the threaded sum value

  }
  return NULL;

}
void* worker_threads_chunk(void* sum1)
{
  bool task_test;  // check if the task was done
  bool nothread = addThread();
  pthread_mutex_lock(&task_getter_mutex);  // Lock task getter mutex
  task_test = task_done;
  pthread_mutex_unlock(&task_getter_mutex);// Lock before accessing task test
  pthread_t temp_thread;
  bool threadSpawned = false;
  std::chrono::time_point<std::chrono::system_clock> start_thread_clock, end_thread_clock;  //Start Clock
   
  if(task_test!= true)
  {
    threadSpawned = true;
    long i = (long)i;
    
  start_thread_clock = std::chrono::system_clock::now();
    pthread_create(&temp_thread,NULL,worker_threads_iteration,(void*)i);
  }

  //Similar to iteration worker threads
  //bool task_test;
  pthread_mutex_lock(&task_getter_mutex);
  task_test = task_done;
  pthread_mutex_unlock(&task_getter_mutex);
  while(task_test!=true)
  {
    long begin,end;
    float mysum;
    get_next_task(begin,end,task_test);
    if(task_test == true)
      return NULL;
      summation_chunk(begin,end);	
      pthread_mutex_lock(&task_getter_mutex);
      task_test = task_done;
      pthread_mutex_unlock(&task_getter_mutex);
  }

  if(threadSpawned == true)
  {
    pthread_join(temp_thread,NULL);
     end_thread_clock = std::chrono::system_clock::now();
  std::chrono::duration<double> elapsed_seconds = end_thread_clock-start_thread_clock;  //End time Processed
  cout<<endl<<"SUB THREAD: "<<elapsed_seconds.count();  //Time difference
  }

  return NULL;

}
int main (int argc, char* argv[]) {
  if (argc < 9) {
    std::cerr<<"usage: "<<argv[0]<<" <functionid> <a> <b> <n> <intensity> <nbthreads> <sync> <granularity>"<<std::endl;
    return -1;
  }


  int functionid = atoi(argv[1]); //Converting function ID from char* to int
  float a = atof(argv[2]); //Converting a value to float as per question
  float b = atof(argv[3]); // converting b value to float as per question
  int n = atoi(argv[4]); //converting n to integer as per q
  int intensity = atoi(argv[5]);
  nbthreads = atoi(argv[6]); //number of threads to integer
  char* sync = argv[7];
  int granularity = atoi(argv[8]);

  pthread_mutex_init(&mutex_chunk,NULL); //Initializing all mutexes
  pthread_mutex_init(&mutex_iter,NULL);
  pthread_mutex_init(&task_getter_mutex,NULL);

  pthread_t *integral_threads = new pthread_t[nbthreads];

  current_args.f = functionid;  //Initializing all integration parameters
  current_args.a = a;
  current_args.b = b;
  current_args.n = n;
  current_args.intensity = intensity;


  float pre_product = (b-a)/n;  //Initialize pre product

  current_args.pre_product = pre_product;
  current_args.granularity = granularity;


  next_available_beginning = 0; //Set available beginning to 0
  float mysum[nbthreads];
  if(granularity<n)
    next_available_end = 0 + granularity;  //Set available end to granularity if n isn't smaller
  else
    next_available_end = n;  //else set to n


  std::chrono::time_point<std::chrono::system_clock> start_clock, end_clock;  //Start Clock
  start_clock = std::chrono::system_clock::now();                             //Get time for start

  cout<<endl<<"------------------GANTT CHART--------------------";

  if(strcmp(sync,"iteration")==0)  //Check for iteration and call iteration workers to start work on integral data
  {
    //cout<<"iteration called";
    pthread_t temp_thread;

    addThread();
    long i =0;
    std::chrono::time_point<std::chrono::system_clock> start_thread_clock, end_thread_clock;  //Start Clock
  start_thread_clock = std::chrono::system_clock::now();
    pthread_create(&temp_thread,NULL,worker_threads_iteration,(void*)i);

    
  
    pthread_join(temp_thread,NULL);
     end_thread_clock = std::chrono::system_clock::now();
  std::chrono::duration<double> elapsed_seconds = end_thread_clock-start_thread_clock;  //End time Processed
  cout<<endl<<"MAIN THREAD: "<<elapsed_seconds.count();  //Time difference


    cout<<endl<<summed_value*pre_product;
    
    
    
  }

  else if(strcmp(sync,"chunk")==0)  //Check for chunk and call iteration workers to start work on integral data
  {
    
    //cout<<"chunk called";
    pthread_t temp_thread;

    addThread();
    long i=0;
    std::chrono::time_point<std::chrono::system_clock> start_thread_clock, end_thread_clock;  //Start Clock
  start_thread_clock = std::chrono::system_clock::now();
    pthread_create(&integral_threads[i],NULL,worker_threads_chunk,(void*)&mysum);
    pthread_join(integral_threads[i],NULL);

     end_thread_clock = std::chrono::system_clock::now();
  std::chrono::duration<double> elapsed_seconds = end_thread_clock-start_thread_clock;  //End time Processed
  cout<<endl<<"MAIN THREAD: "<<elapsed_seconds.count();  //Time difference

    cout<<summed_value*pre_product;
  }

  else if(strcmp(sync,"thread")==0)   //Check for thread and call iteration workers to start work on integral data
  {
    //cout<<"thread called";
    pthread_t temp_thread;
    float mysum = 0;
    std::chrono::time_point<std::chrono::system_clock> start_thread_clock, end_thread_clock;  //Start Clock
  start_thread_clock = std::chrono::system_clock::now();
    pthread_create(&temp_thread,NULL,worker_threads_thread,(void*)&mysum);
    pthread_join(temp_thread,NULL);
     end_thread_clock = std::chrono::system_clock::now();
  std::chrono::duration<double> elapsed_seconds = end_thread_clock-start_thread_clock;  //End time Processed
  cout<<endl<<"MAIN THREAD: "<<elapsed_seconds.count();  //Time difference


    cout<<mysum*pre_product;



  }

  else  //If a valid value is not given for sync
  {
    cout<<endl<<" Give a valid sync value "<<endl;
    return -1;
  }
  end_clock = std::chrono::system_clock::now();
  std::chrono::duration<double> elapsed_seconds = end_clock-start_clock;  //End time Processed
  cerr<<elapsed_seconds.count();  //Time difference



  
  



  return 0;
  
}
