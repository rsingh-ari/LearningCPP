#include "learnSTL.h"


//Nested Class
class Teacher
{
    private:
        int rollNum;
        char name[20];
    public:
        Teacher(){}
        Teacher(int rn, char *ptr)
        {
          rollNum = rn;
          strcpy (name, ptr);
          cout<<"\n Teacher roll num is "<<rollNum<<"\n Teacher name is "<<name;
        }
        void showTeacherInfo ()
        {
           cout<<"\n Teacher roll num is "<<rollNum<<"\n Teacher name is "<<name;
        }
        void getTeacherInfo()
        {
            cout<<"\nEnter Teacher name and roll number";
            cin>>name>>rollNum;
        }
};

void testSTLVector()
{
    cout<<"\n=================Start "<<__FUNCTION__<<"=======================\n";
    vector <int>v1{10,20,30,40,50};
    vector <string>v2{"Vector1","Vector2"};
    vector <char>v3 (4,'a');
    vector <int>v4(5,10);
    vector <string> v5(3,"Hello");

    vector <string>v6 (4,"Raj");
    v6.push_back("mangal");
    cout<<"\n V6 values "<<v6[0]<<v6[1]<<v6[2]<<v6[3]<<v6[4];

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
    cout<<"\n=================END "<<__FUNCTION__<<"=======================\n";
}

void testSTLArray()
{
    cout<<"\n=================Start "<<__FUNCTION__<<"=======================\n";
    array <int, 5> arrayList1 = {1,2,3,4,5};
    array <int, 5> arrayList2 = {6,7,8,9,0};
    arrayList1.swap(arrayList2);
    //arrayList1.fill(9.9);
    cout<<" \n\narrayList1 value at position = "<<arrayList1.at(4)<<"\n";
    cout<<" \n\narrayList1 value at position = "<<arrayList1.size();
    cout<<" \n\narrayList2 value at position = "<<arrayList2.at(3)<<"\n";
    cout<<" \n\narrayList1 value at position = "<<arrayList1.at(3)<<"\n";
    //cout<<" \narrayList value at front "<<arrayList.back();
    cout<<"\n=================END "<<__FUNCTION__<<"=======================\n";
}

void testSTLPair()
{
    cout<<"\n=================Start "<<__FUNCTION__<<"=======================\n";
    Teacher s1(10, "Rajesh");
    pair <string, int>p1;
    pair <string, string>p2;
    pair <string, float>p3;
    pair <int, Teacher>p4;
    p1 = make_pair("RajSingh",25);
    p2 = make_pair("India","New Delhi");
    p3 = make_pair ("MastringCplusplus", 345.5f);
    p4 = make_pair (5,s1);
    cout<<"\n p1 pair value "<<p1.first<<" " <<p1.second;
    cout<<"\n p4 pair value "<<p4.first;
    Teacher s = (p4.second);
    s.showTeacherInfo();
    tuple<string,int,int>t1;
    t1 = make_tuple("Tuple",4,6);
    cout<<"\n Show Tuple = "<<get<1>(t1);
    //Making another tuple
    tuple<string, string, string,int> tupStudent;
    tupStudent = make_tuple("Raj", "Gurugram", "25-09-86", 31);
    cout<<"\n Show student Name " << get<0>(tupStudent);
    cout<<"\n Show student Address " << get<1>(tupStudent);
    cout<<"\n Show student DOB " << get<2>(tupStudent);

    cout<<"\n=================END "<<__FUNCTION__<<"=======================\n";

}

void testSTLList()
{
    cout<<"\n=================Start "<<__FUNCTION__<<"=======================\n";

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
    l2.sort();
    listIt = l2.begin();
    while (listIt!=l2.end())
    {
        cout<<" " <<*listIt;
        listIt++;
    }

    cout<<"\n=================END "<<__FUNCTION__<<"=======================\n";
}

void testSTLMap()
{
    cout<<"\n=================START "<<__FUNCTION__<<"=======================\n";
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
    cout<<"\n=================END "<<__FUNCTION__<<"=======================\n";
}
