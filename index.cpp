#include <iostream>
#include <fstream>
#include <string>
#include <cstdio>
// #include <dirent.h>
// #include <vector>
// #include <stdlib.h>

using namespace std;

struct post
{
    int id;
    string sharer;
    string content;
    string comments[500];
    int likes = 0;
};

string userEmail;
string userFriends[500];
string userName;
string userBio;

post allPosts[500];
post myPosts[500];
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
    string Input = "";
    getline(cin, Input);
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
void loginUser(string email, string password)
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
            userName = readLine("./Accounts/"+email+".txt",3);
            userBio = readLine("./Accounts/"+email+".txt",4);
                string Friend;
            int friendIndex = 0;
            while (getline(userFriendsReader, Friend))
            {
                userFriends[friendIndex] = Friend;
                friendIndex++;
            }
        }
        else
        {
            print("Invalid Password");
        }
    }
    else
    {
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
}

int main()
{
    loginUser("anotherEmail", "Pass");
    init(); // put after login

    
    return 0;
}
