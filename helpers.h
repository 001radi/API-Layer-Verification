//
// Created by d_dab on 15/05/2020.
//

#ifndef PROJECTEMOT_HELPERS_H
#define PROJECTEMOT_HELPERS_H

#define N 2

#include "list.h"
#include "string.h"

typedef struct date_t* Date;
typedef struct post_t* Post;
typedef struct message_t* Message;
typedef struct comment_t* Comment;
typedef struct user_t* User;
typedef struct friendsReq_t* friendsReq;
typedef struct post_user_arr* userPost;

struct friendsReq_t { int val; bool requested; bool accepted; };
struct post_user_arr {int id; bool has; Post post;};

extern void* nd_ptr();


/**
 * struct that describes a user in the system.
 * ech user has id, email, password, first name, last name, birthday, list of his friends, list of friend requests,
 * list named chat that inculdes chats with other users, a list of his posts, a list of broadcast that can send messages
 * to.
 * */
struct user_t{
    int id;
    char* email;
    char* password;
    char* firstName;
    char* lastName;
    Date birthday;
    int friend_req_count;
    int friends_req_size;
    friendsReq* friends_ghosts;
    List chats;
    userPost* user_posts;
    int postsNum;
    int postsSize;
    List posts;
};

void freeEmpty (Element val);
Element copyEmpty(Element val);
int compareEmpty(Element val1,Element val2);

void freeInt (Element val);
Element copyInt(Element val);
int compareInt(Element val1,Element val2);

//Post createPost(char* content, int ownerID);
void createPost(char *content, User user, int postId);
void destroyPost(Post post);
void freePost(Element val);
Element copyPost(Element val);
int comparePost(Element val1,Element val2);

Comment createComment(char* content,int postId ,int ownerId);
void destroyComment(Comment comment);
void freeComment(Element val);
Element copyComment(Element val);
int compareComment(Element val1,Element val2);

Message createMessage(int sender, int receiver, char* content);
void destroyMessage(Message msg);
void freeMessage(Element val);
Element copyMessage(Element val);
int compareMessage(Element val1,Element val2);

Date createBirthdate(int day, int month, int year);

User createUser(int user_id , char* firstName, char* lastName, char* email, char* password, int day, int month, int year);
void destroyUser(User user);
void freeUser(Element val);
Element copyUser(Element val);
int compareUser(Element val1,Element val2);

int getUserID(User user);

char* getUserEmail(User user);

char* getUserPassword(User user);

List getUserMessages(User user);

void updatePassword(User user, char* password);

List getUserFriends(User user);

List getUserFriendRequests(User user);

int FriendRequestExist(User user, int friendId);

int FriendExist(User user, int friendId);

ListResult userAddMessage(User user, Message msg);

ListResult userAddPost(User user, Post post);

ListResult userDeletePost(User user, int postId);

Post getPostById(User user, int postId);

int getPostId(Post post);

List getLikers(User owner, int postId);

int likersNum(User owner, int postId);

ListResult userAddCommentOnPost(User owner,int postId,int commenterId,char* content);

ListResult userLikePost(User owner, int postId, int likerId);

ListResult userUnlikePost(User owner,int postId,int likerId);

List getComments(User owner, int postId);

int commentsNum(User owner, int postId);

void* xmalloc(size_t size);

void* xrealloc(void *ptr, size_t size);

#endif //PROJECTEMOT_HELPERS_H
