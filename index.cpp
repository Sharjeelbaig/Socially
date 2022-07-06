#include <iostream>
#include <fstream>
#include <string>
#include <cstdio> //c language ki cheezain, file ko delete krnay k liye remove() method
// #include <dirent.h>
// #include <vector>
#include <stdlib.h> //for using system("clear")

using namespace std;

// post struct takay bar bar id,sharer waghera na likhnay prhay
struct post
{
    int id;
    string sharer;
    string content;
    string comments[500];
    int likes = 0;
};

// saray registered users
string allUsers[500];

// login hui wi email
string userEmail;

// saray login hua vay us k saray friends
string userFriends[500];
// login hua vay user ka naam
string userName;
// bio login user
string userBio;

// jitni bhi post system may bni hui hein, sari is array may save hongi
post allPosts[500];
// user jo login hua us ki share ki hui posts
post myPosts[500];
// user jo login hua us k friend ki posts
post friendsPosts[500];

/*cout ki jaga print ka function for making code good looking and readable*/
void print(auto value)
{
    cout << value;
}

/*Input ka function bnaya hei, takay bar bar getline na krna prhay*/
string input(string prompt)
{

    cout << prompt;
    string Input;
    cin >> Input;
    return Input;
}

/*User Registration function*/
void registerUser(string email, string password, string name, string bio)
{
    ofstream userWriter("./Accounts/" + email + ".txt");
    userWriter << email << "\n"
               << password << "\n"
               << name << "\n"
               << bio << "\n";
    userWriter.close();
    ofstream userFriendsWriter("./Accounts/" + email + ".friends.txt");
}

/*Aik Certain line read ho kisi file ki*/
string readLine(string directory, int lineIndex)
{
    string line;
    ifstream lineReader(directory);
    for (int i = 1; i <= lineIndex; i++)
    {
        getline(lineReader, line);
    }
    return line;
}

/*User Login Function*/
bool loginUser(string email, string password) // bool use kiya hei takay true false milay k email paas sahi hei?
{
    ifstream userFriendsReader("./Accounts/" + email + ".friends.txt");
    ifstream userReader("./Accounts/" + email + ".txt");
    string Email;
    string Password;
    getline(userReader, Email);

    if (Email == email)
    {
        getline(userReader, Password);
        if (Password == password)
        {
            // login
            userEmail = Email; // jesahi user login ho to userEmail may email store ho jae
            userName = readLine("./Accounts/" + email + ".txt", 3);
            userBio = readLine("./Accounts/" + email + ".txt", 4);
            string Friend;
            int friendIndex = 0;
            while (getline(userFriendsReader, Friend))
            {
                userFriends[friendIndex] = Friend;
                friendIndex++;
            }
            return true;
        }
        else
        {
            return false;
            print("Invalid Password");
        }
    }
    else
    {
        return false;
        print("Invalid Email");
    }
}

// /*Aik Certain line read ho kisi file ki*/
// string readLine(string directory, int lineIndex)
// {
//     string line;
//     ifstream lineReader(directory);
//     for (int i = 1; i <= lineIndex; i++)
//     {
//         getline(lineReader, line);
//     }
//     return line;
// }

// Friends add krna
void addFriend(string email)
{
    ofstream friendAppender("./Accounts/" + userEmail + ".friends.txt", ios::app);
    int currentFriendsCounter = 0; // ye is liye hei takay pehlay check karay kitnay friends hein friend list may
    ifstream friendsReader("./Accounts/" + userEmail + ".friends.txt");
    string Friend;
    while (getline(friendsReader, Friend))
    {
        currentFriendsCounter++;
    }
    // Ye neechay if else say check kr rahay hein k 500 say ziyada toh nahi friends
    if (currentFriendsCounter < 500)
    {
        friendAppender << endl
                       << email;                    // agar hundred say neechay hua tho appand krdo
        userFriends[currentFriendsCounter] = email; // Update user friends global variable
    }
    else
    {
        print("You have reached 500 limit of friends\n");
    }
}

// Friends delete krna
void deleteFriend(string email)
{

    string allFriends = "";
    ifstream friendsReader("./Accounts/" + userEmail + ".friends.txt");
    string Friend;
    while (getline(friendsReader, Friend))
    {
        if (Friend != email)
        {
            allFriends += Friend + "\n";
        }
    }

    ofstream friendsWriter("./Accounts/" + userEmail + ".friends.txt");
    friendsWriter << allFriends;
}

// // file ko searxh kr liye
// string searchFile(char directory[], string fileToSearch)
// {
//     struct dirent *d;
//     DIR *dr;
//     dr = opendir(directory);
//     if (dr != NULL)
//     {
//         cout << "List of Files & Folders:-\n";
//         for (d = readdir(dr); d != NULL; d = readdir(dr))
//         {
//             string file = d->d_name;
//             if (file == fileToSearch)
//             {

//                 return file;
//             }
//         }
//         closedir(dr);
//     }
//     else
//         cout << "\nError Occurred!";
//     cout << endl;
// }

// Post Ka aik struct ko initialize kray ga yeh
post getPosts(int id)
{
    post Post;
    string postDirectory = "./Posts/" + to_string(id) + ".txt";
    string postCommentsDirectory = "./Posts/" + to_string(id) + ".comments.txt";
    string postLikesDirectory = "./Posts/" + to_string(id) + ".likes.txt";
    Post.id = id;
    Post.sharer = readLine(postDirectory, 2);
    Post.content = readLine(postDirectory, 3);
    for (int i = 0; i < 500; i++)
    {
        Post.comments[i] = readLine(postCommentsDirectory, i + 1); // i+1 is liye kiya hei takay 0 index pr 1 comment aye
    }

    ifstream likesReader(postLikesDirectory);
    int likesCount = 0;
    string likedPerson;
    while (getline(likesReader, likedPerson))
    {
        likesCount++;
    }
    Post.likes = likesCount;

    return Post;
}

// void getFriends()
// {
//     // get allFriend posts
//     string Friend;
//     ifstream friendsReader("./Accounts/" + userEmail + ".friends.txt");
//     for (int i = 0; i < 500; i++)
//     {
//         getline(friendsReader, Friend);
//         userFriends[i] = Friend;
//     }
// }

// post like krnay k liye aik function hei like post ka
void likePost(int postId)
{
    ofstream likerAppender("./Posts/" + to_string(postId) + ".likes.txt", ios::app);
    likerAppender << "\n" + userEmail;
}
// Post delete krnay k liye function
void deletePost(int id)
{
    string postDirectory = "./Posts/" + to_string(id) + ".txt";
    char *postDirectoryChar = &postDirectory[0];
    string postCommentsDirectory = "./Posts/" + to_string(id) + ".comments.txt";
    char *postCommentsDirectoryChar = &postCommentsDirectory[0];
    string postLikesDirectory = "./Posts/" + to_string(id) + ".likes.txt";
    char *postLikesDirectoryChar = &postLikesDirectory[0];

    remove(postDirectoryChar);
    remove(postCommentsDirectoryChar);
    remove(postLikesDirectoryChar);

    ifstream totalPostsReader("./Posts/TOTALPOSTS.txt");
    int totalPosts = 0;
    string Post;
    while (getline(totalPostsReader, Post))
    {
        totalPosts = stoi(Post);
    }
    totalPosts -= 1;

    ofstream totalPostsWriter("./Posts/TOTALPOSTS.txt");
    totalPostsWriter << totalPosts;
}

// Comment krnay k liye function
void commentPosts(int postId, string comment)
{
    string seperatorLine = "";
    for (int i = 0; i < comment.length(); i++)
    {
        seperatorLine += "_";
    }
    ofstream commentAppender("./Posts/" + to_string(postId) + ".comments.txt");
    commentAppender << "\n"
                    << seperatorLine << "\n"
                    << "Shared by:"
                    << "\n"
                    << userEmail << "\n"
                    << comment
                    << "\n"
                    << seperatorLine << "\n";
}

// post add
void addPost(string text)
{
    ifstream totalPostsReader("./Posts/TOTALPOSTS.txt");
    int totalPosts = 0;
    string Post;
    while (getline(totalPostsReader, Post))
    {
        totalPosts = stoi(Post);
    }
    totalPosts++;
    ofstream postWriter("./Posts/" + to_string(totalPosts) + ".txt");
    postWriter << totalPosts << "\n"
               << userEmail << "\n"
               << text;

    ofstream postCommentsWriter("./Posts/" + to_string(totalPosts) + ".comments.txt");
    postCommentsWriter << "";
    ofstream postLikesWriter("./Posts/" + to_string(totalPosts) + ".likes.txt");
    postLikesWriter << "";

    ofstream totalPostsWriter("./Posts/TOTALPOSTS.txt");
    totalPostsWriter << totalPosts;
}
// ViewComments Of post
void commentViewer(int id)
{
    ifstream commentsReader("./Posts/" + to_string(id) + ".comments.txt");
    string comment;
    while (getline(commentsReader, comment))
    {
        cout << comment << endl;
    }
}
// share post
void sharePost(int index)
{
    string text = readLine("./Posts/" + to_string(index) + ".txt", 3);
    addPost(text);
}

// chgange name
void changeName(string name)
{
    string info = "";
    string combinedInfo = "";
    ifstream reader("./Accounts/" + userEmail + ".txt");
    while (getline(reader, info))
    {
        if (info != userName)
        {
            combinedInfo += info + "\n";
        }
        else
        {
            combinedInfo += name + "\n";
        }
    }
    ofstream writer("./Accounts/" + userEmail + ".txt");
    writer << combinedInfo;
}

// chgange bio
void changeBio(string bio)
{
    string info = "";
    string combinedInfo = "";
    ifstream reader("./Accounts/" + userEmail + ".txt");
    while (getline(reader, info))
    {
        if (info != userBio)
        {
            combinedInfo += "info\n";
        }
        else
        {
            combinedInfo += bio + "\n";
        }
    }
    ofstream writer("./Accounts/" + userEmail + ".txt");
    writer << combinedInfo;
}

// kuch settings programe chlnay say pehlay
void init()
{
    // getting all posts
    for (int i = 0; i < 500; i++)
    {
        allPosts[i] = getPosts(i + 1); // i+1 is liye kiya hei takay 0 pr 1 aye
    }
    // getting my posts
    int myPostArrayIndex = 0;
    for (int i = 0; i < 500; i++)
    {
        if (allPosts[i].sharer == userEmail)
        {
            myPosts[myPostArrayIndex] = allPosts[i];
            myPostArrayIndex++;
        }
    }
    // get allFriend posts
    string Friend;
    int friendsPostIndex = 0;
    ifstream friendsReader("./Accounts/" + userEmail + ".friends.txt");
    for (int i = 0; i < 500; i++)
    {
        getline(friendsReader, Friend);
        if (allPosts[i].sharer == Friend)
        {
            friendsPosts[friendsPostIndex] = allPosts[i];
            friendsPostIndex++;
        }
    }

    // saray users lenay k liye
    ifstream usersReader("./Accounts/REGISTEREDUSERS.txt");
    int indexOfUser = 0;
    while (getline(usersReader, allUsers[indexOfUser]))
    {
        indexOfUser++;
    }
    // Vapis login karay
    string password = readLine("./Accounts/" + userEmail + ".txt", 2);
    loginUser(userEmail, password);
}

// Screens
void likeScreen()
{
    print("=================Like=====================\n");
    print("\n");
    int id;
    print("Enter id of the post you want to like\n");
    cin >> id;
    likePost(id);
    init();
}
void commentScreen()
{
    print("=================Comment=====================");
    print("\n");
    int id;
    print("Enter id of the post you want to comment\n");
    cin >> id;
    string comment;
    getline(cin, comment);
    commentPosts(id, comment);
    init();
}

void viewAllCommentsOfPostsScreen()
{
    print("=================All Comments=====================");
    print("\n");
    print("Enter post Id to view all its comment\n");
    int id;
    cin >> id;
    commentViewer(id);
}
void shareScreen()
{
    print("=================Share=====================");
    print("\n");
    int id;
    print("Enter id of the post you want to share\n");
    cin >> id;
    sharePost(id);
    init();
}
void allPostsScreens()
{
    system("clear");
    print("=================NewsFeed/All Posts=====================");
    print("\n");
    print("Enter any option: \n");
    print("1 for like\n2 for comment\n for share\n");
    for (int i = 0; i < 500; i++)
    {
        if (friendsPosts[i].id != 0)
        {
            print("____________________________________________");
            print("post id: " + to_string(friendsPosts[i].id) + "\n");
            print("sharedby: " + friendsPosts[i].sharer + "\n");
            print("content:: " + friendsPosts[i].content + "\n");
            print("likes: " + to_string(friendsPosts[i].likes) + "\n");
        }
    }
    for (int i = 0; i < 500; i++)
    {
        if (myPosts[i].id != 0)
        {
            print("____________________________________________");
            print("post id: " + to_string(myPosts[i].id) + "\n");
            print("sharedby: " + myPosts[i].sharer + "\n");
            print("content:: " + myPosts[i].content + "\n");
            print("likes: " + to_string(myPosts[i].likes) + "\n");
        }
    }
    int option;
    cin >> option;
    switch (option)
    {
    case 1:
        likeScreen();
        break;
    case 2:
        commentScreen();
        break;
    case 3:
        shareScreen();
        break;
    default:
        print("invalid option");
        break;
    }
}

void addPostScreen()
{
    print("+++++Add post+++++++\n");
    system("clear");
    string content;
    print("Write something");
    getline(cin, content);
    addPost(content);
    init();
}
void accountSetttingsScreen()
{
    print("=========================");
    print("Account Setting");
    print("=========================");
    print("\n");
    print("Select option:\n");
    print("1 for change name\n");
    print("2 any other for change bio:\n");
    int option;
    cin >> option;
    if (option == 1)
    {
        string name = input("Enter new name: ");
        changeName(name);
    }
    else
    {
        string bio;
        print("Enter new bio: ");
        getline(cin, bio);
        changeBio(bio);
    }
}
void myPostsScreen()
{
    print("=================My Posts=====================");
    print("Enter any option: \n");
    print("1 for deletePost\n");
    for (int i = 0; i < 500; i++)
    {
        if (myPosts[i].id != 0)
        {
            print("____________________________________________");
            print("post id: " + to_string(myPosts[i].id) + "\n");
            print("sharedby: " + myPosts[i].sharer + "\n");
            print("content:: " + myPosts[i].content + "\n");
            print("likes: " + to_string(myPosts[i].likes) + "\n");
        }
    }
    cout << "Enter 1 for deleting post\n";
    int option;
    cin >> option;
    if (option == 1)
    {
        int id;
        cout << "Enter post id to delete\n";
        cin >> id;

        deletePost(id);
        init();
    }
}
void friendsScreen()
{
    print("========all my friends======= \n");

    for (int i = 0; i < 500; i++)
    {
        if (userFriends[i] != "")
        {
            cout << userFriends[i] << endl;
        }
    }

    print("press d to delete friend\n");
    char option;
    cin >> option;

    if (option == 'd')
    {
        string email;
        cout << "Enter friend's email to delete\n";
        cin >> email;
        deleteFriend(email);
        init();
    }
}

void allUsersScreen()
{
    print("========all users======= \n");

    for (int i = 0; i < 500; i++)
    {
        if (allUsers[i] != "")
        {
            cout << allUsers[i] << endl;
        }
    }
    print("press a to add friend\n");
    char option;
    cin >> option;

    if (option == 'a')
    {
        string email;
        cout << "Enter friend's email to add\n";
        cin >> email;
        addFriend(email);
        init();
    }
}

void accountScreen()
{
    system("clear");
    print("=================Account=====================");
    print("\n");
    print("Name: " + userName + "\n");
    print("Email: " + userEmail + "\n");
    print("Bio: " + userBio + "\n");

    print("1 for account settings \n");
    print("2 for friends \n");
    print("3 for find friends \n");
    print("4 for viewing my posts \n");
    int option;
    cin >> option;

    switch (option)
    {
    case 1:
        accountSetttingsScreen();
        break;
    case 2:
        friendsScreen();
        break;
    case 3:
        allUsersScreen();
        break;
        case 4:
    myPostsScreen();
    break;
    }

    init();
}

void homeScreen()
{
    do
    {
        init();
        system("clear");

        print("=================Homescreen=====================");
        print("\n");
        int option;
        print("\n\t **Welcome to Home Page** ");
        print("\n\t 1.Go  to  *Acount ");
        print("\n\t 2.Go  to  *Post");
        print("\n\t 3.Go  to  *Add Posts");
        print("\n\t 4.Go  to  *View All Comments");
        cin >> option;
        switch (option)
        {
        case 1:
            accountScreen();
            break;
        case 2:
            allPostsScreens();
            break;
        case 3:
            addPostScreen();
            break;

        case 4:
            viewAllCommentsOfPostsScreen();
            break;
        }
    } while (true);
}
void loginScreen()
{
    print("=================Login=====================");
    print("\n");
    string email = input("Enter your email\n");
    string password = input("Enter your password\n");
    bool isCorrect = loginUser(email, password);

    if (isCorrect)
    {
        init();
        homeScreen();
    }
}

void registerScreen()
{
    print("=================Register=====================");
    print("\n");
    string email = input("Enter email (Do not include space)\n");
    string password = input("Enter password (Do not include space)\n");
    string confirmPassword = input("Confirm Password\n");
    if (confirmPassword == password)
    {
        cin.ignore();
        string name;
        print("Enter your name: ");
        getline(cin, name);
        string bio;
        print("Enter bio: ");
        cin.ignore();
        getline(cin, bio);

        registerUser(email, password, name, bio);
        system("clear");
        // init();
        loginScreen();
    }
    else
    {
        print("Password does not match\n");
        registerScreen();
    }
}

void indexScreen()
{
    print("=================Welcome to Socially Social Network Project=====================");
    print("\n");
    print("Do you want to login or signup?\n");
    int option;
    print("1 for login,\n2 for signup\n");
    cin >> option;
    switch (option)
    {
    case 1:
        system("clear");
        loginScreen();
        break;

    case 2:
        system("clear");
        registerScreen();
        break;
    default:
        print("Please select a valid option\n");
        indexScreen();
    }
}

int main()
{
    indexScreen();
    // loginUser("anotherEmail", "Pass");
    init(); // put after login
    // system("chafa /home/sharjeel/Firefox_wallpaper.png");

    //}
    return 0;
}
