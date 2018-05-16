#include <iostream>
#include <stdlib.h>
#include <assert.h>
#include <ctime>
#include <thread>
#include "kdtree.h"

using namespace std;

static const int threadCount = 8;
static const int iterationCount = 128;
static kdtree * testTree;

void populateTree(int sampleSize, kdtree* tree){
    srand(10.0f);
    for(int i = 0; i < sampleSize; i++){
        assert(kd_insert3(tree, rand()/ RAND_MAX * 1000.0f, rand()/ RAND_MAX * 1000.0f, rand()/ RAND_MAX * 1000.0f, 0) == 0);
    }
}

//3 измерения
void initTree(){
    testTree = kd_create(3);
    populateTree(10000, testTree);
}

void insertSamplePoint(kdtree * tree, double * sample, void* data){
    assert(kd_insert3(tree, sample[0], sample[1], sample[2], data) == 0);
}

void findSamplePointTest(){

    initTree();
    void* checkptr = testTree;
    double resPos[3], sample[3] = {500.0, 500.0, 500.0};
    insertSamplePoint(testTree, sample, checkptr);
    clock_t begin = clock();
    kdres* res = kd_nearest_range(testTree, sample, 40.0f);
    clock_t end = clock();
    double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC * 1000.0;

    void* resultData = kd_res_item(res, resPos);
    assert(resultData == checkptr);
}

double findSamplePointGetTime(kdtree * testTree, double* sample){


    void* checkptr = testTree;
    double resPos[3];
    clock_t begin = clock();
    kdres* res = kd_nearest_range(testTree, sample, 40.0f);
    clock_t end = clock();
    double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC * 1000.0;

    void* resultData = kd_res_item(res, resPos);

    assert(resultData == checkptr);
    return elapsed_secs;
}
void findSamplePointGetTime(kdtree * testTree, double* sample, double *time){
    if(*time != 0)
        (*time) += findSamplePointGetTime(testTree, sample);
    else findSamplePointGetTime(testTree, sample);
}


void samplePointIteration(double** sample){
    for(int i = 0; i < iterationCount; i++){
            double * point = sample[rand()%128];
            findSamplePointGetTime(testTree, point);
    }
}

void findVariousSamplePointsTest(){
    int curThread = 0;
    initTree();
    double sample[128][3] = new double[128][3];
    for(int i =0; i < 128; i++){
        sample[i] = {rand()/ RAND_MAX * 1000.0f, rand()/ RAND_MAX * 1000.0f, rand()/ RAND_MAX * 1000.0f};
        insertSamplePoint(testTree, sample[i], (void*)testTree);
    }
    while(curThread < threadCount)
        thread t1(samplePointIteration);
}

void findElaspedTimeTest(){
    initTree();
    double times[4];
    atomic<double> time(0.0);

    for(int i =0; i < 8; i++){
        double sample[3] = {rand()/ RAND_MAX * 1000.0f, rand()/ RAND_MAX * 1000.0f, rand()/ RAND_MAX * 1000.0f};
        findSamplePointGetTime(testTree, sample, &time);
    }
    times[0] = time;
    time = 0.0;
    for(int i =0; i < 4; i++){
        double sample[3] = {rand()/ RAND_MAX * 1000.0f, rand()/ RAND_MAX * 1000.0f, rand()/ RAND_MAX * 1000.0f};
        thread t1(findSamplePointGetTime, &testTree, &sample, &(time));
        thread t2(findSamplePointGetTime, &testTree, &sample, &(time));
    }
    times[1] = time;
    time = 0.0;
    for(int i =0; i < 2; i++){
        double sample[3] = {rand()/ RAND_MAX * 1000.0f, rand()/ RAND_MAX * 1000.0f, rand()/ RAND_MAX * 1000.0f};
        thread t1(findSamplePointGetTime, &testTree, &sample, &(time));
        thread t2(findSamplePointGetTime, &testTree, &sample, &(time));
        thread t3(findSamplePointGetTime, &testTree, &sample, &(time));
        thread t4(findSamplePointGetTime, &testTree, &sample, &(time));
    }
    times[2] = time;
    time = 0.0;
        double sample[3] = {rand()/ RAND_MAX * 1000.0f, rand()/ RAND_MAX * 1000.0f, rand()/ RAND_MAX * 1000.0f};
        thread t1(findSamplePointGetTime, &testTree, &sample, &(time));
        thread t2(findSamplePointGetTime, &testTree, &sample, &(time));
        thread t3(findSamplePointGetTime, &testTree, &sample, &(time));
        thread t4(findSamplePointGetTime, &testTree, &sample, &(time));
        thread t5(findSamplePointGetTime, &testTree, &sample, &(time));
        thread t6(findSamplePointGetTime, &testTree, &sample, &(time));
        thread t7(findSamplePointGetTime, &testTree, &sample, &(time));
        thread t8(findSamplePointGetTime, &testTree, &sample, &(time));
    times[3] = time;
    time = 0.0;
    t1.join();
    t2.join();
    t3.join();
    t4.join();
    t5.join();
    t6.join();
    t7.join();
    t8.join();
    cout<< "TIMES : " << time[0] << " " << time[1] << " " << time[2] << " " << time[3] << " ";
}

int main()
{
    findSamplePointTest();
    findVariousSamplePointsTest();
    findElaspedTimeTest();
    return 0;
}
