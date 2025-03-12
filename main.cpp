#include <iostream>
#include <thread>
#include <windows.h>
#include <string>

using namespace std;

// int hpe = 1000;
// int hph = 1000;
// int lock = 1;
//
// void fLock (){
//     while (lock != 1)
//         ;;
//
//     lock = 0;
// }
//
// void fUnlock(){
//     lock = 1;
// }
//
// void eFight(){
//     while (hph > 0 ) {
//         fLock();
//         hph = hph - 100;
//         if (hph <= 0){
//             fUnlock();
//             break;
//         }
//         else{
//             cout << hph << "- zycie bohatera" << endl;
//             fUnlock();
//             Sleep(200);
//         }
//         if (hpe <= 0){
//             cout << "bohater wygral z" <<hph  << " hp" << endl;
//             break;
//         }
//     }
// }
//
// void hFight(){
//     while (hpe > 0 ){
//         fLock();
//         hpe = hpe - 150;
//         if (hpe <= 0){
//             fUnlock();
//             break;
//         }
//         else{
//             cout << hpe << "- zycie enemy" << endl;
//             fUnlock();
//             Sleep(300);
//         }
//         if (hph <= 0){
//             cout << "enemy wygral z" << hpe << " hp" << endl;
//             break;
//         }
//     }
// }

// int N = 5;
#define N 5
string states[3] = { "Eating", "Hungry", "Thinking"};
string philosopher_state[N];
int philosophers[N] = {0, 1, 2, 3, 4};
int forks[N] = {1, 1, 1, 1, 1}; //1 - fork unlocked | 0 - fork locked

void lock(int fork_num){
    while (forks[fork_num] != 1)
        ;;

    forks[fork_num] = 0;
}

void unlock(int fork_num){
    forks[fork_num] = 1;
}

int left(int position) {return (position + 4) % N;}    //Returns position of left neighbour
int right(int position) {return (position + 1) % N;}   //Returns position of right neigbour

void check(int philosopher){
    if (philosopher_state[philosopher] == states[1] //Is hungry
        && philosopher_state[left(philosopher)] != states[0] //Left neighbour not eating
        && philosopher_state[right(philosopher)] != states[0]){ //Right neighbour not eating

        philosopher_state[philosopher] = states[0]; //Starts eating for 2 seconds and locks forks
        lock(right(philosopher));
        lock(left(philosopher));
        std::cout << "Philosopher " << philosopher << " takes fork " <<  left(philosopher) << " and " << right(philosopher) << std::endl;
        cout << "Philosopher " << philosopher << " is eating" << endl;
        Sleep(2000);
        }
}

void try_eat(int philosopher){
    philosopher_state[philosopher] = states[1];    //Mark, he's hungry
    cout << "Philosopher " << philosopher << " is hungry" << endl;

    check(philosopher); //Check if he can eat
    Sleep(1000); //If not, then wait for 1 second to next check
}

void think(int philosopher){
    if (philosopher_state[philosopher] == states[0]){ //If he ate, then he starts thinking
        unlock(left(philosopher));
        unlock(right(philosopher));
        philosopher_state[philosopher] = states[2]; //Mark, he's thinking
        std::cout << "Philosopher " << philosopher << " putting fork " << left(philosopher) << " and " << right(philosopher) << " down" << endl;
        cout << "Philosopher " << philosopher << " is thinking" << endl;

        check(left(philosopher));
        check(right(philosopher));
    }
}

void philosopher(int position){
    while (true){
        try_eat(position);
        think(position);
    }
}

int main(){

    thread threads[N];

    for (int i = 0; i < N ; i++){
        threads[i] = thread(philosopher, i);    //Pass function and argument, which is philosopher id
        //cout << "Philosopher " << i << " is thinking" << endl; //Everyone starts with thinking
    }
    for (int i = 0; i < N; i++) threads[i].join();  //Join all threads to process

    return 0;
}
