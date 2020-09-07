#ifndef PROJECTEMOT_API_H
#define PROJECTEMOT_API_H

//#define N 2
// M is emails length.
#define M 5

#include "string.h"
#include "list.h"
#include "helpers.h"

typedef struct API_t* API;
typedef struct store_t* Store;
typedef struct userElement_t* UserElement;

typedef enum APIResults{
    API_SUCCESS,
    API_USER_DOES_NOT_EXIST,
    API_EMAIL_ALREADY_USED,
    API_ALREADY_FRIENDS,
    API_NOT_A_FRIEND,
    API_REQUEST_ALREADY_EXISTS,
    API_REQUEST_DOES_NOT_EXIST,
    API_POST_DOES_NOT_EXIST,
    API_DID_NOT_LIKE_POST,
    API_WRONG_PASSWORD,
    API_MEMORY_ERROR
}APIRes;

typedef enum HandleRequest{
    ACCEPT,
    DECLINE
}Handle;

struct store_t {
    short sz;
    char *ghost_p;
    char *ghost_q;
    int ghost_flag;
};
struct userElement_t{
    int id;
    bool has;
    User user;
};

struct API_t {
    int usersNum;
    int usersArraySize;
    List users;
    List used_emails;
    UserElement* usersGhosts;
    Store* storedEmailes;
    int EmailsNum;
    int EmailsArraySize;
};

API initSystem();

void destroyApi(API api);

User createUserInAPI(API api,int user_id, char* firstName, char* lastName, char* email, char* password, int day, int month, int year);

int ApiHasUser(API api, int user_id);

User getUserById(API api, int user_id);

APIRes deleteUserFromAPI(API api,int user_id);

APIRes login(API api, char* email, char* Password);

APIRes logout(API api, int userID);

APIRes changePassword(API api, int user_id, char *oldPass, char *newPass);

char* getPasswordByEmail(API api, char* email);

APIRes sendFriendRequest(API api, int friendId , User user);

APIRes handleFriendRequest(API api, User user, int friendId, Handle respond);

APIRes deleteFriend(API api, char* userEmail, char* friendEmail);

APIRes sendMessage(API api, char* userEmail, char* receiverEmail, char *content);

List getMessages(API api, char* email);

//int publishPost(API api, char* userEmail, char *content);

APIRes publishPost(API api, int userID, int postID, char *content);

//APIRes deletePost(API api, char* userEmail, int postId);

APIRes deletePost(API api, int userId, int postId);

APIRes addCommentOnPost(API api, char* commenterEmail, char *content, int postId, char* ownerEmail);

APIRes likePost(API api, char* likerEmail, int postId, char* ownerEmail);

APIRes unLikePost(API api, char* likerEmail, int postId, char* ownerEmail);

List getPostLikers(API api, char *email, int postId);

int getLikersNum( API api,char* ownerEmail ,int postId);

List getPostComments(API api, char *email, int postId);

int getCommentsNum(API api,char* ownerEmail ,int postId);

List friendsOfUser(API api, char* email);
#endif //PROJECTEMOT_API_H