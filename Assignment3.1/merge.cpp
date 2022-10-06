/*
 * TODO: remove and replace this file header comment
 * You will edit and turn in this file.
 * Remove starter comments and add your own
 * comments on each function and on complex code sections.
 */
#include <iostream>    // for cout, endl
#include "queue.h"
#include "testing/SimpleTest.h"
#include "recursion.h"
using namespace std;

/*
 * This function receives two sorted integer Queues
 * in which the elements are arranged from the smallest to
 * the largest. It returns a merged Queue sorted in the same way.
 */
Queue<int> binaryMerge(Queue<int> a, Queue<int> b) {
    if(!sortedCheck(a) || !sortedCheck(b)){
        error("The input Queues are not well sorted!");
    }
    Queue<int> result;
    while(!a.isEmpty() && !b.isEmpty()){
        if(a.peek() <= b.peek()){
            result.enqueue(a.dequeue());
        }
        else{
            result.enqueue(b.dequeue());
        }
    }
    while(!a.isEmpty()){
         result.enqueue(a.dequeue());
    }
    while(!b.isEmpty()){
         result.enqueue(b.dequeue());
    }
    return result;
}

/*
 * This fucntion check if a Queue is sorted from the smallest to
 * the largest;
 */
bool sortedCheck(Queue<int> a){
    if(a.size() <= 1){
        return true;
    }
    while(a.size() > 1){
        if(a.dequeue() > a.peek())
            return false;
    }
    return true;
}

/*
 * This function assumes correct functionality of the previously
 * defined binaryMerge function and makes use of this function to
 * iteratively merge a collection of sequences, starting off with
 * an empty sequence and progressively merging in one sequence at
 * a time. This function is provided for you fully implemented –
 * it does not need to modified at all.
 */
Queue<int> naiveMultiMerge(Vector<Queue<int>>& all) {
    Queue<int> result;

    for (Queue<int>& q : all) {
        result = binaryMerge(q, result);
    }
    return result;
}

/*
 * This function receives a Vector of sorted Queues and merge them
 * in a recursive way.
 */
Queue<int> recMultiMerge(Vector<Queue<int>>& all) {
    //base case
    if(all.size() == 0){ //handle empty input
        return {};
    }
    else if(all.size() == 1){
        return all[0];
    }
    else if(all.size() == 2){
        return binaryMerge(all[0], all[1]);
    }
    else{
        int midIndex = all.size() / 2;
        Vector<Queue<int>> front = all.subList(0, midIndex);
        Vector<Queue<int>> back = all.subList(midIndex, all.size() - midIndex);
        // separate all into two parts, merge each part and finally combine the two parts.
        return binaryMerge(recMultiMerge(front), recMultiMerge(back));
    }
}


/* * * * * * Test Cases * * * * * */

Queue<int> createSequence(int size);
void distribute(Queue<int> input, Vector<Queue<int>>& all);

PROVIDED_TEST("binaryMerge, two short sequences") {
    Queue<int> a = {2, 4, 5};
    Queue<int> b = {1, 3, 3};
    Queue<int> expected = {1, 2, 3, 3, 4, 5};
    EXPECT_EQUAL(binaryMerge(a, b), expected);
    EXPECT_EQUAL(binaryMerge(b, a), expected);
}

PROVIDED_TEST("naiveMultiMerge, small collection of short sequences") {
    Vector<Queue<int>> all = {{3, 6, 9, 9, 100},
                             {1, 5, 9, 9, 12},
                             {5},
                             {},
                             {-5, -5},
                             {3402}
                            };
    Queue<int> expected = {-5, -5, 1, 3, 5, 5, 6, 9, 9, 9, 9, 12, 100, 3402};
    EXPECT_EQUAL(naiveMultiMerge(all), expected);
}

PROVIDED_TEST("recMultiMerge, compare to naiveMultiMerge") {
    int n = 20;
    Queue<int> input = createSequence(n);
    Vector<Queue<int>> all(n);
    distribute(input, all);
    EXPECT_EQUAL(recMultiMerge(all), naiveMultiMerge(all));
}

PROVIDED_TEST("Time binaryMerge operation") {
    int n = 1000000;
    Queue<int> a = createSequence(n);
    Queue<int> b = createSequence(n);
    TIME_OPERATION(a.size() + b.size(), binaryMerge(a, b));
}

PROVIDED_TEST("Time naiveMultiMerge operation") {
    int n = 11000;
    int k = n/10;
    Queue<int> input = createSequence(n);
    Vector<Queue<int>> all(k);
    distribute(input, all);
    TIME_OPERATION(input.size(), naiveMultiMerge(all));
}
PROVIDED_TEST("Time recMultiMerge operation") {
    int n = 90000;
    int k = n/10;
    Queue<int> input = createSequence(n);
    Vector<Queue<int>> all(k);
    distribute(input, all);
    TIME_OPERATION(input.size(), recMultiMerge(all));
}

STUDENT_TEST("sortedCheck with sorted and unsorted Queues"){
    Queue<int> sorted = {1, 2, 2, 7, 9, 10};
    Queue<int> unsorted = {1, 8, 2, 7, 9, 10};
    EXPECT_EQUAL(sortedCheck(sorted), true);
    EXPECT_EQUAL(sortedCheck(unsorted), false);
}

STUDENT_TEST("binaryMerge, unsorted inputs") {
    Queue<int> a = {2, 4, 5};
    Queue<int> b = {1, 9, 3};
    EXPECT_ERROR(binaryMerge(a, b));
}

STUDENT_TEST("naiveMultiMerge, empty queues to be merged") {
    Queue<int> inputOne = {};
    Queue<int> inputTwo = {};
    Queue<int> inputThree = {};
    Vector<Queue<int>> all = {inputOne, inputTwo, inputThree};
    Queue<int> expected = {};
    EXPECT_EQUAL(naiveMultiMerge(all), expected);
}

STUDENT_TEST("naiveMultiMerge, empty Vector to be merged") {
    Vector<Queue<int>> all = {};
    Queue<int> expected = {};
    EXPECT_EQUAL(naiveMultiMerge(all), expected);
}

STUDENT_TEST("recMultiMerge, with odd number size Vector") {
    int n = 11;
    Queue<int> input = createSequence(n);
    Vector<Queue<int>> all(n);
    distribute(input, all);
    EXPECT_EQUAL(recMultiMerge(all), naiveMultiMerge(all));
}

STUDENT_TEST("recMultiMerge, with empty Vector") {
    Vector<Queue<int>> all = {};
    Queue<int> expected = {};
    EXPECT_EQUAL(recMultiMerge(all), expected);
}

STUDENT_TEST("recMultiMerge, with empty queues to be merged") {
    Vector<Queue<int>> all = {{}, {}, {}};
    Queue<int> expected = {};
    EXPECT_EQUAL(naiveMultiMerge(all), expected);
}

/* Test helper to fill queue with sorted sequence */
Queue<int> createSequence(int size) {
    Queue<int> q;
    for (int i = 0; i < size; i++) {
        q.enqueue(i);
    }
    return q;
}

/* Test helper to distribute elements of sorted sequence across k sequences,
   k is size of Vector */
void distribute(Queue<int> input, Vector<Queue<int>>& all) {
    while (!input.isEmpty()) {
        all[randomInteger(0, all.size()-1)].enqueue(input.dequeue());
    }
}
