#include <iostream>
#include <string.h>
#include <array>
#include <tuple>
#include <vector>
#include <list>
#include <map>
#include "learnDataStructure.h"

struct doubleStruct
{
    double x;
    char y;
};

using namespace std;
//static int testStaticValue = 5;
namespace MySpace
{
    int a;
    void f1();
    class Hello
    {
        public:
            void hello ()
            { cout<<"\n Hello class"; }
    };
}
//using namespace MySpace;
void MySpace::f1()
{
    cout<<"\n InF1 fucn";
}
class Student;
class A
{
    private:
        int a, b;
    public:
        A() //Constructor
        {
            a=0;
            b=0;
            cout<<"\n I am A class constructor";
        }
        A(int x, int y)
        {
            a= x;
            b= y;
            cout<<"\n I am in A class paramterized constructor";
        }
        virtual ~A()
        {
            cout<<"\nHello I am A class destructor ";
            //cout<<"I am the last \n function get called of object life.";
        }
        friend void mainTeraDost ( A &objA, Student &objS);

};
class B:public A
{
    int *ptr;

    public:
        B(){
            ptr = new int;
            cout<<"\n B class Constructor called";
        }
        virtual ~B(){
            delete ptr;
            cout<<"\nB class destructor called due to runtime binding.";
        }
};

//Nested Class
class Student
{
    private:
        int rollNum;
        char name[20];
    public:
        Student(){}
        Student(int rn, char *ptr)
        {
          rollNum = rn;
          strcpy (name, ptr);
          cout<<"\n Student roll num is "<<rollNum<<"\n Student name is "<<name;
        }
        void showStudentInfo ()
        {
           cout<<"\n Student roll num is "<<rollNum<<"\n Student name is "<<name;
        }
        void getStudentInfo()
        {
            cout<<"\nEnter student name and roll number";
            cin>>name>>rollNum;
        }
    friend void mainTeraDost ( A &objA, Student &objS);
    friend ostream & operator<< (ostream &, Student&);
    friend istream & operator>> (istream &, Student&);
};

ostream & operator <<(ostream &OUT, Student &X)
{
    cout<<"\n Operator overloading";
    cout<<"\n Student data by operator overloading = ";
    X.showStudentInfo();
    return OUT;
}
istream & operator >> (istream &Input, Student &Xin)
{
    cout<<"\n Istream operotor is overloaded ";
    Xin.getStudentInfo();
    return Input;
}
//Friend function
void mainTeraDost ( A &objA, Student &objS)
{
    cout<<"\n A class private data dekho = "<<objA.a;
    cout<<"\n Student class private data dekho = "<<objS.rollNum;
}

void incrementStatic()
{
    testStaticValue++;
    cout<<"\ntestStaticValue1 = "<<testStaticValue;
}

void incrementStaticOnceMore()
{
    testStaticValue++;
    cout<<"\ntestStaticValue2 = "<<testStaticValue;
}

class Empty
{};

class Derived1 : public Empty
{};

class Derived2 : virtual public Empty
{};

class Derived3 : public Empty
{
    char c;
};

class Derived4 : virtual public Empty
{
    char c;
};

class Dummy
{
    char c;
};


//About STL
int main() {
    try {
        cout<<"In Try Block";
        throw 10;
    }
    catch (int expNum)
    {
        cout<<"\n ExpNumber = "<<expNum;
    }
    //A o1,o2,o3(3,4);
    // your code goes here
    //a = 5;
    //f1();
    //Hello obj;
    //obj.hello();
    A o1;
    A *ptrA = new B;
    cout<<"\n Now deleting ptrA ";
    delete ptrA;

    Student s1(10, "Rajesh");
    //operator overloading
    //cout<<s1;
    //cin>>s1;
    //cout<<s1;
    mainTeraDost(o1, s1);
    array <int, 5> arrayList1 = {1,2,3,4,5};
    array <int, 5> arrayList2 = {6,7,8,9,0};
    arrayList1.swap(arrayList2);
    //arrayList1.fill(9.9);
    cout<<" \n\narrayList1 value at position = "<<arrayList1.at(4)<<"\n";
    cout<<" \n\narrayList1 value at position = "<<arrayList1.size();
    cout<<" \n\narrayList2 value at position = "<<arrayList2.at(4)<<"\n";
    //cout<<" \narrayList value at front "<<arrayList.back();
    pair <string, int>p1;
    pair <string, string>p2;
    pair <string, float>p3;
    pair <int, Student>p4;
    p1 = make_pair("RajSingh",25);
    p2 = make_pair("India","New Delhi");
    p3 = make_pair ("MastringCplusplus", 345.5f);
    p4 = make_pair (5,s1);
    cout<<"\n p1 pair value "<<p1.first<<" " <<p1.second;
    cout<<"\n p4 pair value "<<p4.first;
    Student s = (p4.second);
    s.showStudentInfo();
    tuple<string,int,int>t1;
    t1 = make_tuple("Tuple",4,6);
    cout<<"\n Show Tuple = "<<get<1>(t1);
    vector <int>v1{10,20,30,40,50};
    vector <string>v2{"Vector1","Vector2"};
    vector <char>v3 (4,'a');
    vector <int>v4(5,10);
    vector <string> v5(3,"Hello");
    cout<<" \n V 0 = "<<v2[0];
    cout<<" \n V 1 = "<<v2[1];
    for (int i =0;i<5;i++)
        cout<<"\n "<<v1[i];
    vector <int>v7;
    cout<<"\n Capaciy v7 = "<<v7.capacity();
    for(int i=0;i<9;i++)
    v7.push_back(10*(i+1));
    v7.pop_back();v7.pop_back();
    cout<<"\n Capaciy v7 = "<<v7.capacity();
    cout<<"\n Capaciy total number of elements v7 = "<<v7.size();
    for(int i=0;i<v7.size();i++)
        cout<<"\n "<<v7[i];
    cout<<"\n v7at 3 = "<<v7.at(4);
    //v7.clear();
    vector<int>:: iterator it = v7.begin();
    v7.insert(it+3, 35);
    for(int i=0;i<v7.size();i++)
        cout<<"\n "<<v7[i];

    testMe objTest;
    mujhePrintKaro();
    list <int> l1{11,22,33,44,55}; //Can access front to back or back to front
    list <int>::iterator ptr1 = l1.begin();
    while (ptr1!=l1.end())
    {
        cout<<" " <<*ptr1;
        ptr1++;
    }
    l1.pop_back();
    list <string>l2 {"Raj","Pragati","Shahsi","Namrta","Ajay","Deepti"};
    l2.push_back("Noida");
    l2.push_front("Gurgaon");
    list <string>::iterator listIt = l2.begin();
    while (listIt!=l2.end())
    {
        cout<<" " <<*listIt;
        listIt++;
    }
    cout<<"\n Total value of l2 = "<<l2.size();
    //Map Associative arrays its a key value pair and key acn not be altered and in shorted order
    map <int,string> customer;
    customer[100]="Rajesh";
    customer[110]="Vijay";
    customer[120]="Rajendra";
    cout<<"\nCustomer name at 100 key value is "<<customer[100];
    map <int,string> m2 = {
    {100,"Value1"},
    {110,"Value2"}
    };
    map <int,string>::iterator mapItr = customer.begin();
    while(mapItr!=customer.end())
    {
        cout<<"\n Key "<<mapItr->first<<" Value "<<mapItr->second;
        mapItr++;
    };
    m2.insert(pair<int,string>(120,"Value3"));
    m2.insert(pair<int,string>(130,"Value4"));
    cout<<"\n m2 map value inserted is "<<m2[120];

    //String class handling assign(),append(),insert(),replace(),erase(), find(), rfind(), compare(), c_str(), size()
    char str1[] = {"Hello"};
    char str2[20] = {"CompareStrings"};
    cout<<"\nSize of string array "<<sizeof(str1);
    string str3 = "Online cplusplus learning";
    string str4 = str3;
    str4 +=" ho rahi hai";
    //str4.assign("Assign karo value change karo string me");
    //str4.append("append karo value jodo karo string me");
    //str4.insert(20,"YE INSERT HUA");
    str4.replace(0,6,"RAJ_KI_ONLINE");
    //str4.erase();
    //int index = str4.find("RAJ");
    //int index = str4.rfind("RAJ");
    int index =  str4.compare(str3);
    //cout<<"\n str4 is "<<str4<<" Raj substring ka index "<<index;
    cout<<"\n Compare value is "<<index;
    //String class supports many constructors
    char charArray[50];
    strcpy(charArray, str4.c_str());
    cout<<"\n CharArray = "<<charArray;

    incrementStatic();
    incrementStaticOnceMore();
    //int sum = showNaturalNumber(5);
    //cout<<"\n Sum of natural number is "<<sum;
    //TOH(3,'B','A','E');
    //Double link list
    /*
    addAsFirstNode();
    addAsFirstNode();
    addAsFirstNode();
    addAsFirstNode();
    addAsFirstNode();
    addAsFirstNode();
    showDoubleLinkList();
    deleteThePerticiularNode(5);
    showDoubleLinkList(); */

    createStack(4);
    push_stack(5);
    push_stack(6);
    push_stack(7);
    push_stack(8);
    cout<<" SHOW STACK "<<pop_stack();cout<<" "<<pop_stack();cout<<" "<<pop_stack();cout<<" "<<pop_stack();cout<<" "<<pop_stack();
    int ary[] = {1,2,3,4,5,6,7,8,9};
    binarySearch(ary,9,5);

    //\a beep k liye, fir \b baclspace ke liye fir \r first chareter to
    int *intPtr = new int[5];
    char *charPtr = new char[5];
    cout<<"\n Size\r of int ptr\t = "<<sizeof(intPtr);
    cout<<"\n Size of char ptr\b = "<<sizeof(charPtr)<<"\a\a";
    doubleStruct *stNode = new doubleStruct;
    cout<<"\n size od double structure "<<sizeof(*stNode);

    cout << "sizeof(Empty) " << sizeof(Empty) << endl;
    cout << "sizeof(Derived1) " << sizeof(Derived1) << endl;
    cout << "sizeof(Derived2) " << sizeof(Derived2) << endl;
    cout << "sizeof(Derived3) " << sizeof(Derived3) << endl;
    cout << "sizeof(Derived4) " << sizeof(Derived4) << endl;
    cout << "sizeof(Dummy) " << sizeof(Dummy) << endl;

    return 0;
}

/*
void permutation(cahr *s, int i, int n)
{
    static int count;
    int j;
    if(i=n)
    {
        count++;
        cout<<"\n Count "<<count<<" "<<s;
    }
    else
    {
        for(j=i;j<=n;j++)
        {
            swap((s+i),(s+j));
            permutation(s,i+1,n);
            swap((s+i),(s+j));
        }
    }
}*/







