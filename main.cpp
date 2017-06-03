#include <iostream>
#include <vector>
#include<cstring>

using namespace std;

class Friend
{
public:
    string Person_Name;
    vector <Friend*> Person_Friends;
    Friend()
    {
        Person_Name = "";
    }
    Friend(string);
    bool Make_Friendship(Friend *);
    int Contain(string);
    void Unfriend(Friend *);
    bool Is_Friend_With(Friend *);
    string Tag;
};

bool Friend::Is_Friend_With(Friend *Other)
{
    int I;
    for (I =0; I <Person_Friends.size(); I++)
    {
        if (Person_Friends[I] == Other)
        {
            return true;
        }
    }
    return false;
}

void Friend::Unfriend(Friend *Other)
{
    int I;
    for (I =0; I <Person_Friends.size(); I++)
    {
        if (Person_Friends[I] == Other)
        {
            Person_Friends.erase(Person_Friends.begin()+I);
        }
    }
}

Friend::Friend(string Name)
{
    Person_Name = Name;
}

int Friend::Contain(string Name)
{
    int I;
    for (I = 0; I < Person_Friends.size(); I++)
        if (Person_Friends[I]->Person_Name == Name) return I;
    return -1;
}

bool Friend::Make_Friendship(Friend *Other)
{
    if (Contain(Other->Person_Name) == -1)
    {
        Friend *Me = this;
        Other->Person_Friends.push_back(Me);
        Person_Friends.push_back(Other);
        return true;
    }
    else
        return false;
}

class Database
{
public:
    //All friends
    vector <Friend> All_People;
    Database() {}
    int Add_People(string);
    int Contain(string);
};

int Database::Contain(string Name)
{
    int I;
    for (I = 0; I < All_People.size(); I++)
    {
        if (All_People[I].Person_Name == Name) return I;
    }
    return -1;
}

int Database::Add_People(string Name)
{
    if (Contain(Name) == -1)
    {
        All_People.push_back(Friend(Name));
        return All_People.size() - 1;
    }
    else
        return -1;
}

class Search_System
{
public:
    Database *Main_Base;
    Search_System() {}
    Search_System(Database *);
    void Show_Connections(string, string);
};

Search_System::Search_System(Database *Base)
{
    Main_Base = Base;
}

void Search_Subfriends(Friend *Friend_Obj, vector<string> *Checked, vector<string> *Track_Strings,string Start_Search, string Search_For)
{
    int I;
    int II;

    Friend *Current_Friend = NULL;

    bool Pass = false;

    for (I = 0; I < Friend_Obj->Person_Friends.size(); I++)
    {
        Current_Friend = Friend_Obj->Person_Friends[I];
        string temp = Friend_Obj->Tag + ">" + Current_Friend->Person_Name;

        for (II = 0; II < Checked->size(); II++)
        {
            if (Current_Friend->Person_Name == (*Checked)[II])
            {
                Pass=true;
                break;
            }
        }


        if (Pass == false)
        {
            Current_Friend->Tag = temp;
            if (Current_Friend->Person_Name == Search_For)
            {
                //We found him
                Track_Strings->push_back(Current_Friend->Tag);
                Track_Strings->push_back("End");
            }
            else
            {
                Checked->push_back(Current_Friend->Person_Name);
                Search_Subfriends(Current_Friend, Checked, Track_Strings,Start_Search, Search_For);
                Checked->erase(Checked->end());
            }
        }
        else
        {
            Pass = false;
        }
    }
    return;
}

void Search_System::Show_Connections(string Name1, string Name2)
{
    if (Main_Base->Contain(Name1) == -1 && Main_Base->Contain(Name2) == -1)
    {
        cout <<Name1<<" and "<<Name2<< " not found." << endl;
    }
    else if(Main_Base->Contain(Name1) == -1)
    {
        cout <<Name1<< " not found." << endl;
    }
    else if(Main_Base->Contain(Name2)==-1)
    {
        cout <<Name2<< " not found." << endl;
    }
    else
    {
        Friend *Friend_Obj = &Main_Base->All_People[Main_Base->Contain(Name1)];
        vector <string> Checked_Names;
        vector <string> Track_Strings;

        int I;
        //Add itself
        Checked_Names.push_back(Name1);

        //Search sub items
        Friend_Obj->Tag = Friend_Obj->Person_Name;
        Search_Subfriends(Friend_Obj, &Checked_Names, &Track_Strings, Name1, Name2);
        cout << endl;
        if (Track_Strings.size() > 0)
        {
            int I;
            int ID = 0;
            cout << endl << "Connections between " << Name1 << " and " << Name2 << " :" << endl << endl;
            for (I = 0; I < Track_Strings.size(); I++)
            {
                if (Track_Strings[I] != "End")
                {
                    ID++;
                    cout << "Connection " << ID << ": " << Track_Strings[I] << endl;
                }
                else
                    cout << endl;
            }
        }
        else
            cout<<"No Connection Found Between "<<Name1<<" and "<<Name2<<"."<<endl;
    }
}

void All_Database_Functions(Database &Base)
{
    int Continue = 1;
    while (Continue)
    {
        cout << endl;
        cout << "What do you want to do ?" << endl;
        cout << "1. Add people" << endl;
        cout << "2. Unfriend Someone" << endl;
        cout << "3. Full list" << endl;
        cout << "4. Make friendship" << endl;
        cout << "5. Someone's friendlist" << endl;
        cout << "6. Back" << endl;
        int Choice;
        int I;
        int ID;
        string Name;
        cin >> Choice;
        switch (Choice)
        {
        case 1:

            while(1)
            {
                cout << "Enter person name : ";
                cin >> Name;
                if (Base.Add_People(Name) == -1)
                {
                    cout << "Person is already in the database. Try again." << endl;
                }
                else
                    break;
            }
            cout << "Person " << Name << " is added" << endl;
            break;
        case 2:
            cout << "-------------------------------" << endl;
            for (I = 0; I < Base.All_People.size(); I++)
                cout << I+1 << ". " << Base.All_People[I].Person_Name << endl;
            cout << "-------------------------------" << endl;
            cout << "Enter ID to select (0 to back) : " << endl;

            cin >> ID;
            if (ID == 0)
            {
                break;
            }
            else if (ID > 0 && ID <= Base.All_People.size())
            {
Try_Again_1:
                cout << endl;
                cout << Base.All_People[ID-1].Person_Name << " is selected." << endl;
                cout << "Person has : " << Base.All_People[ID-1].Person_Friends.size() << " friends" << endl;
                cout << "-------------------------------" << endl;
                bool Friend_With;
                for (I = 0; I < Base.All_People.size(); I++)
                {
                    if (I != ID-1)
                    {
                        Friend_With = Base.All_People[ID-1].Is_Friend_With(&Base.All_People[I]);
                        if (Friend_With == false)
                            cout << I+1 << ". " <<  Base.All_People[I].Person_Name << endl;
                        else
                            cout << I+1 << ". " <<  Base.All_People[I].Person_Name << " < friend." << endl;
                    }
                    else
                    {
                        cout << I+1 << ". " <<  Base.All_People[I].Person_Name << " < selected." << endl;
                    }
                }
                cout << "-------------------------------" << endl;
                cout << "Enter ID to remove from friendlist (0 to back) : " << endl;
                int ID_2;
                cin >> ID_2;
                if (ID_2 == 0)
                {

                }
                else if (ID_2 > 0 && ID_2 <= Base.All_People.size())
                {
                    int Sel_ID = ID_2 - 1;
                    if (Sel_ID == ID - 1)
                    {
                        //Selected person
                        cout << "Selected person cannot be removed from his/her friendlist!. Try again." << endl;
                        goto Try_Again_1;
                    }
                    Friend_With = Base.All_People[ID-1].Is_Friend_With(&Base.All_People[Sel_ID]);
                    if (Friend_With == false)
                    {
                        //Already friend
                        cout << "Selected person is not friend with " << Base.All_People[Sel_ID].Person_Name << ". Try again." << endl;
                        goto Try_Again_1;
                    }
                    Base.All_People[Sel_ID].Unfriend(&Base.All_People[ID-1]);
                    Base.All_People[ID-1].Unfriend(&Base.All_People[Sel_ID]);
                    cout << Base.All_People[Sel_ID].Person_Name << " is removed from " << Base.All_People[ID-1].Person_Name <<"'s friendlist."<< endl;
                    break;
                }
                else
                {
                    cout << "Invalid ID. Try again." << endl;
                    goto Try_Again_1;
                }
            }
            else
            {
                cout << "Invalid ID" << endl;
            }
            break;
        case 3:
            cout << "-------------------------------" << endl;
            for (I = 0; I < Base.All_People.size(); I++)
                cout << I+1 << ". " << Base.All_People[I].Person_Name << endl;
            cout << "-------------------------------" << endl;
            break;
        case 4:
            cout << "-------------------------------" << endl;
            for (I = 0; I < Base.All_People.size(); I++)
                cout << I+1 << ". " << Base.All_People[I].Person_Name << endl;
            cout << "-------------------------------" << endl;
            cout << "Enter ID to select (enter 0 to back) : " << endl;

            cin >> ID;
            if (ID == 0)
            {

            }
            else if (ID > 0 && ID <= Base.All_People.size())
            {
Try_Again_2:
                cout << endl;
                cout << Base.All_People[ID-1].Person_Name << " is selected." << endl;
                cout << "Person has : " << Base.All_People[ID-1].Person_Friends.size() << " friends" << endl;
                cout << "-------------------------------" << endl;
                bool Friend_With;
                for (I = 0; I < Base.All_People.size(); I++)
                {
                    if (I != ID-1)
                    {
                        Friend_With = Base.All_People[ID-1].Is_Friend_With(&Base.All_People[I]);
                        if (Friend_With == false)
                            cout << I+1 << ". " <<  Base.All_People[I].Person_Name << endl;
                        else
                            cout << I+1 << ". " <<  Base.All_People[I].Person_Name << " < friend." << endl;
                    }
                    else
                    {
                        cout << I+1 << ". " <<  Base.All_People[I].Person_Name << " < selected." << endl;
                    }
                }
                cout << "-------------------------------" << endl;
                cout << "Enter ID to make friendship with that person (0 to back) : " << endl;
                int ID_2;
                cin >> ID_2;
                if (ID_2 == 0)
                {

                }
                else if (ID_2 > 0 && ID_2 <= Base.All_People.size())
                {
                    int Sel_ID = ID_2 - 1;
                    if (Sel_ID == ID - 1)
                    {
                        //Selected person
                        cout << "Selected person cannot be friend with him/herself!. Try again." << endl;
                        goto Try_Again_2;
                    }
                    Friend_With = Base.All_People[ID-1].Is_Friend_With(&Base.All_People[Sel_ID]);
                    if (Friend_With == true)
                    {
                        //Already friend
                        cout << "Selected person is already friend with " << Base.All_People[Sel_ID].Person_Name << ". Try again." << endl;
                        goto Try_Again_2;
                    }
                    Base.All_People[ID-1].Make_Friendship(&Base.All_People[Sel_ID]);
                    cout << Base.All_People[ID-1].Person_Name << " is now friend with " << Base.All_People[Sel_ID].Person_Name << endl;
                    break;
                }
                else
                {
                    cout << "Invalid ID. Try again." << endl;
                    goto Try_Again_2;
                }
            }
            else
            {
                cout << "Invalid ID" << endl;
            }
            break;
        case 5:
            cout << "-------------------------------" << endl;

            for (I = 0; I < Base.All_People.size(); I++)
                cout << I+1 << ". " << Base.All_People[I].Person_Name << endl;
            cout << "-------------------------------" << endl;
            cout << "Enter ID to see friendlist (0 to back) : " << endl;

            cin >> ID;
            if (ID == 0)
            {
                break;
            }
            else if (ID > 0 && ID <= Base.All_People.size())
            {
                for (I = 0; I <  Base.All_People[ID-1].Person_Friends.size(); I++)
                    cout << I+1 << ". " << Base.All_People[ID-1].Person_Friends[I]->Person_Name<< endl;
                if(Base.All_People[ID-1].Person_Friends.size()==0)
                    cout<< Base.All_People[ID-1].Person_Name<<" has no friend."<<endl;
                cout << "-------------------------------" << endl;
            }
            else
            {
                cout << "Invalid ID" << endl;
            }
            break;
        case 6:
            Continue = 0;
            break;
        default:
            cout << "Invalid choice. Try again." << endl;
        }
    }
}

void All_Search_Functions(Search_System &Search)
{
    cout << "Enter friend 1 name : " << endl;
    string name1;
    cin >> name1;
    cout << "Enter friend 2 name : " << endl;
    string name2;
    cin >> name2;
    Search.Show_Connections(name1, name2);
}

int main()
{
    cout << "Welcome to Friendship Network" << endl;

    //Default Database
    Database Base;
    Search_System Search(&Base);
    int Amit = Base.Add_People("Amit");
    int Fahad = Base.Add_People("Fahad");
    int Tanmoy = Base.Add_People("Tanmoy");
    int Rima = Base.Add_People("Rima");
    int Muna = Base.Add_People("Muna");
    int Abir = Base.Add_People("Abir");
    int Soma = Base.Add_People("Soma");

    Base.All_People[Amit].Make_Friendship(&Base.All_People[Fahad]);
    Base.All_People[Fahad].Make_Friendship(&Base.All_People[Tanmoy]);
    Base.All_People[Tanmoy].Make_Friendship(&Base.All_People[Fahad]);
    Base.All_People[Rima].Make_Friendship(&Base.All_People[Amit]);
    Base.All_People[Muna].Make_Friendship(&Base.All_People[Tanmoy]);
    Base.All_People[Muna].Make_Friendship(&Base.All_People[Rima]);
    Base.All_People[Tanmoy].Make_Friendship(&Base.All_People[Muna]);
    Base.All_People[Abir].Make_Friendship(&Base.All_People[Rima]);
    Base.All_People[Abir].Make_Friendship(&Base.All_People[Amit]);

//    Base.All_People[Amit].Unfriend(&Base.All_People[Fahad]);
//    Base.All_People[Fahad].Unfriend(&Base.All_People[Amit]);

    int Continue = 1;
    while (Continue)
    {
        cout << endl << endl;
        cout << "1. Check or Edit Friendlist" << endl;
        cout << "2. Check Friendship between Two People" << endl;
        cout << "3. Exit" << endl;

        int Choice;
        cin >> Choice;

        if(Choice==1)
        {
            All_Database_Functions(Base);
        }
        else if(Choice==2)
        {
            All_Search_Functions(Search);
        }
        else if(Choice==3)
        {
            break;
        }
        else
        {
            cout << "Invalid choice. Try again." << endl;
        }
    }

    return 0;
}
