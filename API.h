#ifndef PROJECTEMOT_API_H
#define PROJECTEMOT_API_H

#include "string.h"
#include "list.h"
#include "helpers.h"

typedef struct API_t* API;

/**
 * return values from the functions.
 * */
typedef enum APIResults{
    API_SUCCESS,
    API_USER_DOES_NOT_EXIST,
    API_USER_ALREADY_EXISTS,
    API_EMAIL_ALREADY_USED,
    API_ALREADY_FRIENDS,
    API_NOT_A_FRIEND,
    API_REQUEST_ALREADY_EXISTS,
    API_REQUEST_DOES_NOT_EXIST,
    API_POST_ALREADY_EXISTS,
    API_POST_DOES_NOT_EXIST,
    API_ALREADY_LIKED,
    API_DID_NOT_LIKE_POST,
    API_WRONG_PASSWORD,
    API_COMMENT_DOES_NOT_EXIST,
    API_MEMORY_ERROR
}APIRes;

typedef enum HandleRequest{
    ACCEPT,
    DECLINE
}Handle;

/**
 * initialize the system.
 * */
API initSystem();

/**
 * function that gets information about a new user and adds it to the system.
 * incase of success it returns SUCCESS.
 * if the user already in the system it returns USER_ALREADY_EXISTS.
 * */
APIRes createUserInAPI(API api, char* firstName, char* lastName, char* email, char* password, int day, int month, int year);

/**
 * the function gets an ID for an user and deletes the user from the system.
 * */
APIRes deleteUserFromAPI(API api, int userID);

/**
 * function that gets an email and password in order to log in the user into the system.
 * if the email doesn't exsits in the system it returns USER_DOES_NOT_EXIST.
 * if the found in the system put the password doesn't match, it returns WRONG_PASSWORD.
 * otherwise, SUCCESS.
 * */
APIRes login(API api, char* email, char* Password);

/**
 * function that log out an user from the system.
 * */
APIRes logout(API api, int userID);

/**
 * it gets an user, his old password and a new password. the function changes the password for the user.
 * incase that the old password doesn't match his password it returns WRONG_PASSWORD. otherwise, SUCCESS.
 * */
APIRes changePassword(API api, int userID, char* oldPass, char* newPass);

/**
 * function that helps the users that forgot their password.
 * it receives an ID and send the password to his email in the system.
 * incase that there is no such ID in the system it returns USER_DOES_NOT_EXIST.
 * otherwise, it send the password to the user and return SUCCESS.
 * */
APIRes getPasswordById(API api, int userID);

/**
 * function the gets an unser and ID for other friend, it sends friend request to the user with the ID friendId.
 * if there's no such user in the system it returns USER_DOES_NOT_EXIST.
 * if there are already friends it returns ALREADY_FRIENDS.
 * otherwise, sends friend requset and returns SUCCESS.
 * */
APIRes sendFriendRequest(API api, int userID, int friendId);

/**
 * fuction to respond to friend requests. it gets the ID of the user that sent the friend request and the respond to the
 * friend request.
 * if the user with the ID "friendId" doesn't exists in the system it returns USER_DOES_NOT_EXIST.
 * if the request doesn't exists it returns REQUEST_DOES_NOT_EXISTS.
 * otherwise, it returns SUCCESS.
 * */
APIRes handleFriendRequest(API api, int userID, int friendId, Handle respond);

/**
 * delete a freind.
 * it gets the ID of the friend that we want to delete from user's friends.
 * if the user with the ID "friendId" doesn't exists in the system it returns USER_DOES_NOT_EXIST.
 * if they are not friends it returns NOT_A_FRIEND.
 * otherwise, SUCCESS and delete.
 * */
APIRes deleteFriend(API api, int userID, int friendId);

/*
 * send a message from user to user.
 * it gets the sender user, the content of the message, the ID of the receiver.
 * if the user with the ID "receiver" doesn't exists in the system it returns API_USER_DOES_NOT_EXIST.
 * incase that they are not friends it returns API_NOT_A_FRIEND, and does not send the message.
 * otherwise, sends the message and returns success.
 * **/
APIRes sendMessage(API api, int userID, int receiverId, char *content);

/**
 * publishing a post for user. it gets ID for the post and the contenct of the post.
 * if there's already post with this ID it returns POST_ALREADY_EXISTS.
 * otherwise, it shares the post and return SUCCESS.
 * */
APIRes publishPost(API api, int userId, char* content);

/**
 * deletes a post for the user.
 * if the post does not exsit, it returns POST_DOES_NOT_EXISTS.
 * otherwise, removes the post the returns SUCCESS.
 * */
APIRes deletePost(API api, int userID, int postId);

/**
 * add comment on a post.
 * if the "ownerId" doesn't exsits it returns USER_DOES_NOT_EXIST.
 * if the "ownerId" exists in the sytem but doesn't have the post "postId", it returns POST_DOES_NOT_EXISTS.
 * if they are not friends it returns NOT_A_FRIEND.
 * otherwise, adds the comment and returns SUCCESS.
 * */
APIRes addCommentOnPost(API api, int commenterId, char* content, int postId, int ownerId);

/**
 * a function to like a post.
 * if the "ownerId" doesn't exsits it returns USER_DOES_NOT_EXIST.
 * if the "ownerId" exists in the sytem but doesn't have the post "postId", it returns POST_DOES_NOT_EXISTS.
 * if they are not friends it returns NOT_A_FRIEND.
 * if he already liked the post it returns ALREADY_LIKED.
 * otherwise, likes the post and returns SUCCESS.
 * */
APIRes likePost(API api, int userID, int postId, int ownerId);

/**
 * a function to unlike (remove the like) a post.
 * if the "ownerId" doesn't exsits it returns USER_DOES_NOT_EXIST.
 * if the "ownerId" exists in the sytem but doesn't have the post "postId", it returns POST_DOES_NOT_EXISTS.
 * if they are not friends it returns NOT_A_FRIEND.
 * if he did not like the post before it returns DID_NOT_LIKE_POST.
 * otherwise, likes the post and returns SUCCESS.
 * */
APIRes unLikePost(API api, int userID, int postId, int ownerId);

//
//
///**not sure if this part gonna make it to the final cut*/
//APIRes createBroadcast(int userID , int broadcastId, char* broadcastName, List users);
//
//APIRes removeBroadcast (int userID, int broadcastId);
//
//APIRes addUserToBroadcast(int userID, int broadcastId, int userId);
//
//APIRes removeUserFromBroadcast(int userID, int broadcastId, int userId);

#endif //PROJECTEMOT_API_H