#ifndef LEARNDATASTRUCTURE_H_INCLUDED
#define LEARNDATASTRUCTURE_H_INCLUDED
#include <iostream>
#include <array>
#include <vector>
#include <string>
//#include <stdio.h>
using namespace std;

static int testStaticValue = 5;
class testMe
{
private:
    int testMeData;
public:
    testMe()
    {
        cout<<"\n Hello I am here testMe 111111111class";
    }
    testMe(int x)
    {
        testMeData = x;
        cout<<"\n testMeData = "<<testMeData;
    }
};
void mujhePrintKaro();

//Linear data structure : Link list, queue, array, stack, array
//Algorithm is step by step instructions to solve a given problem.
//Graph analysis : Worst case, average case, best case
//Asymptotic notations Big O notations , Omega notations and theta notations
//Recursion method
int showNaturalNumber (int);//Adding n natural numbers through recursive call of a function.
//Permutation of a given string
//void permutation(cahr *, int, int);
void TOH(int,char,char,char);
void addAsFirstNode();
void deleteThePerticiularNode(int data);
void showDoubleLinkList();

void createStack(int capacity);
void push_stack (int value);
int pop_stack ();
bool binarySearch(int *,int,int);


#endif // LEARNDATASTRUCTURE_H_INCLUDED
