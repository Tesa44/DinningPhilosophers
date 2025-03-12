#include <iostream>
#include <thread>
#include <windows.h>
#include <string>

using namespace std;

#define N 5
string philosopher_state[N];
int philosophers[N] = {0, 1, 2, 3, 4};

int locker = 1;
void lock(){
    while (locker != 1)
        Sleep(1);
    locker = 0;
}

void unlock(){
    locker = 1;
}

int left(int position) {return (position + N - 1) % N;}    //Returns position of left neighbour
int right(int position) {return (position + 1) % N;}   //Returns position of right neigbour

void check(int philosopher){
    if (philosopher_state[philosopher] == "Hungry" //Is hungry
        && philosopher_state[left(philosopher)] != "Eating" //Left neighbour not eating
        && philosopher_state[right(philosopher)] != "Eating"){ //Right neighbour not eating

        philosopher_state[philosopher] = "Eating"; //Starts eating for 2 seconds and locks forks
        std::cout << "Philosopher " << philosopher << " takes fork " <<  philosopher << " and " << right(philosopher) << std::endl;
        cout << "Philosopher " << philosopher << " is eating" << endl;
        }
}

void try_eat(int philosopher){
    lock();
    philosopher_state[philosopher] = "Hungry";    //Mark, he's hungry
    cout << "Philosopher " << philosopher << " is hungry" << endl;

    check(philosopher); //Check if he can eat
    unlock();

    while (philosopher_state[philosopher] != "Eating"){ //If he can't eat, he waits
        Sleep(1);
    }
}

void think(int philosopher){
    lock();
    if (philosopher_state[philosopher] == "Eating"){ //If he ate, then he starts thinking
        philosopher_state[philosopher] = "Thinking"; //Mark, he's thinking
        std::cout << "Philosopher " << philosopher << " putting fork " << philosopher << " and " << right(philosopher) << " down" << endl;
        cout << "Philosopher " << philosopher << " is thinking" << endl;

        check(left(philosopher));
        check(right(philosopher));
    }
    unlock();
    Sleep(1000);
}

void philosopher(int position){
    while (true){
        try_eat(position);
        Sleep(2000);    //Eating time
        think(position);
    }
}

int main(){

    thread threads[N];

    for (int i = 0; i < N; i++) {
        philosopher_state[i] = "Thinking"; // Everyone starts with thinking
    }

    for (int i = 0; i < N ; i++){
        threads[i] = thread(philosopher, i);    //Pass function and argument, which is philosopher id
    }
    for (int i = 0; i < N; i++) threads[i].join();  //Join all threads to process

    return 0;
}
