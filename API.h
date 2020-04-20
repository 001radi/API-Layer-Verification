
#ifndef PROJECTEMOT_API_H
#define PROJECTEMOT_API_H

typedef char* string;

/**
 * return values from the functions.
 * */
typedef enum APIResults{
    SUCCESS,
    USER_DOES_NOT_EXIST,
    USER_ALREADY_EXISTS,
    INVALID_INPUT,
    ALREADY_FRIENDS,
    NOT_A_FRIEND,
    REQUEST_DOES_NOT_EXISTS,
    POST_ALREADY_EXISTS,
    POST_DOES_NOT_EXISTS,
    ALREADY_LIKED,
    DID_NOT_LIKE_POST,
    WRONG_PASSWORD
}APIRes;


/**
 * the sturcts we have defined may use sort of list or queue, not sure yet which code to include here.
 * */
typedef struct list{

}List;

/**
 * sturct that inculdes a list of all the users in the program.
 * */
typedef struct allUsers{
    List users;
}AllUsers;

/**
 * struct that describes date.
 * gonna use this struct in personal info of the user, for example his birthday.
 * */
typedef struct date{
    int day;
    int month;
    int year;
}Date;

/**
 * struct that describes a user in the system.
 * ech user has id, email, password, first name, last name, birthday, list of his friends, list of friend requests,
 * list named chat that inculdes chats with other users, a list of his posts, a list of broadcast that can send messages
 * to.
 * */
typedef struct user{
  int id;
  string email;
  string password;
  string firstName;
  string lastName;
  Date birthday;
  List friends;
  List friendsRequests;
  List chats;
  List Posts;
  List broadcasts;
}User;


/**
 * struct that describes a post.
 * it had a list of the users that liked this post, a list of comments on this post, number of likes on this post,
 * the content of the post.
 * */
typedef struct post{
    int id;
    List likes;
    List comments;
    int likesNum;
    string content;
}Post;

/**
 * sturct that describes a message between two users.
 * it had ID for every message, ID of the sender user, ID of the receiver, the content of the message.
 * */
typedef struct message{
    int msgId;
    int from;
    int to;
    string content;
}Message;

/**
 * struct that describes a chat.
 * it had ID for each chat, ID of the sender user, ID of the receiver, and the chat is a list of messages.
 * */
typedef struct chat{
    int chatId;
    int from;
    int to;
    List messages;
}Chat;

/**
 * a struct that discribes broadcast.
 * it had an ID for each broadcast, as well as name, and a list of users.
 * */
typedef struct broadcast{
    int Id;
    string name;
    List users;
}Broadcast;

/**
 * struct that describes a comment.
 * each comment had ID, the ID of the post that it was written on, the ID of the comment owner (user), the content of
 * the comment.
 * */
typedef struct comment{
  int id;
  int postId;
  int ownerId;
  string content;
}Comment;

/**
 * function that gets information about a new user and adds it to the system.
 * incase of success it returns SUCCESS.
 * if the input does not the match the requsted format it returns INVALID_INPUT.
 * if the user already in the system it returns USER_ALREADY_EXISTS.
 * */
APIRes createUser(int id, string firstName, string lastName, string email, string password, int day, int month, int year);

/**
 * the function gets an ID for an user and deletes the user from the system.
 * */
APIRes deleteUser(User user);

/**
 * function that gets an email and password in order to log in the user into the system.
 * if the email doesn't exsits in the system it returns USER_DOES_NOT_EXIST.
 * if the found in the system put the password doesn't match, it returns WRONG_PASSWORD.
 * otherwise, SUCCESS.
 * */
APIRes login(string email, string Password);

/**
 * function that log out an user from the system.
 * */
APIRes logout(User user);

/**
 * it gets an user, his old password and a new password. the function changes the password for the user.
 * incase that the old password doesn't match his password it returns WRONG_PASSWORD. otherwise, SUCCESS.
 * */
APIRes changePassword(User user, string oldPass, string newPass);

/**
 * function that helps the users that forgot their password.
 * it receives an ID and send the password to his email in the system.
 * incase that there is no such ID in the system it returns USER_DOES_NOT_EXIST.
 * otherwise, it send the password to the user and return SUCCESS.
 * */
APIRes getPasswordById(int id);

/**
 * function the gets an unser and ID for other friend, it sends friend request to the user with the ID friendId.
 * if there's no such user in the system it returns USER_DOES_NOT_EXIST.
 * if there are already friends it returns ALREADY_FRIENDS.
 * otherwise, sends friend requset and returns SUCCESS.
 * */
APIRes sendFriendRequest(User user, int friendId);

/**
 * fuction to respond to friend requests. it gets the ID of the user that sent the friend request and the respond to the
 * friend request.
 * if the user with the ID "friendId" doesn't exists in the system it returns USER_DOES_NOT_EXIST.
 * if the request doesn't exists it returns REQUEST_DOES_NOT_EXISTS.
 * otherwise, it returns SUCCESS.
 * */
APIRes handleFriendRequest(User user, int friendId, string respond);

/**
 * delete a freind.
 * it gets the ID of the friend that we want to delete from user's friends.
 * if the user with the ID "friendId" doesn't exists in the system it returns USER_DOES_NOT_EXIST.
 * if they are not friends it returns NOT_A_FRIEND.
 * otherwise, SUCCESS and delete.
 * */
APIRes deleteFriend(User user, int friendId);

/*
 * send a message from user to user.
 * it gets the sender user, the content of the message, the ID of the receiver.
 * if the user with the ID "receiver" doesn't exists in the system it returns USER_DOES_NOT_EXIST.
 * incase that they are not friends it returns NOT_A_FRIEND, and does not send the message.
 * otherwise, sends the message and returns success.
 * **/
APIRes sendMessage(User user, string content, int receiver);

/**
 * opens a chat for the user. the chat with the user which his ID is "receiver".
 * if the user with the ID "receiver" doesn't exists in the system it returns USER_DOES_NOT_EXIST.
 * if they are not friends it returns NOT_A_FRIEND.
 * otherwise, it opens the chat and returns SUCCESS.
 * */
APIRes openChat(User user, int receiver);

/**
 * publishing a post for user. it gets ID for the post and the contenct of the post.
 * if there's already post with this ID it returns POST_ALREADY_EXISTS.
 * otherwise, it shares the post and return SUCCESS.
 * */
APIRes publishPost(User user, int postId, string content);

/**
 * deletes a post for the user.
 * if the post does not exsit, it returns POST_DOES_NOT_EXISTS.
 * otherwise, removes the post the returns SUCCESS.
 * */
APIRes deletePost(User user, int postId);

/**
 * add comment on a post.
 * if the "ownerId" doesn't exsits it returns USER_DOES_NOT_EXIST.
 * if the "ownerId" exists in the sytem but doesn't have the post "postId", it returns POST_DOES_NOT_EXISTS.
 * if they are not friends it returns NOT_A_FRIEND.
 * otherwise, adds the comment and returns SUCCESS.
 * */
APIRes addCommentOnPost(User user, string content, int postId, int ownerId);

/**
 * a function to like a post.
 * if the "ownerId" doesn't exsits it returns USER_DOES_NOT_EXIST.
 * if the "ownerId" exists in the sytem but doesn't have the post "postId", it returns POST_DOES_NOT_EXISTS.
 * if they are not friends it returns NOT_A_FRIEND.
 * if he already liked the post it returns ALREADY_LIKED.
 * otherwise, likes the post and returns SUCCESS.
 * */
APIRes likePost(User user, int postId, int ownerId);

/**
 * a function to unlike (remove the like) a post.
 * if the "ownerId" doesn't exsits it returns USER_DOES_NOT_EXIST.
 * if the "ownerId" exists in the sytem but doesn't have the post "postId", it returns POST_DOES_NOT_EXISTS.
 * if they are not friends it returns NOT_A_FRIEND.
 * if he did not like the post before it returns DID_NOT_LIKE_POST.
 * otherwise, likes the post and returns SUCCESS.
 * */
APIRes unLikePost(User user, int postId, int ownerId);



/**not sure if this part gonna make it to the final cut*/
APIRes createBroadcast(User user , int broadcastId, string broadcastName, List users);

APIRes removeBroadcast (User user, int broadcastId);

APIRes addUserToBroadcast(User user, int broadcastId, int userId);

APIRes removeUserFromBroadcast(User user, int broadcastId, int userId);

#endif //PROJECTEMOT_API_H
