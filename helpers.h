//
// Created by d_dab on 15/05/2020.
//

#ifndef PROJECTEMOT_HELPERS_H
#define PROJECTEMOT_HELPERS_H

#include "list.h"
#include "string.h"

typedef struct date_t* Date;
typedef struct post_t* Post;
typedef struct message_t* Message;
typedef struct comment_t* Comment;
typedef struct user_t* User;

void freeEmpty (Element val);
Element copyEmpty(Element val);
int compareEmpty(Element val1,Element val2);

void freeInt (Element val);
Element copyInt(Element val);
int compareInt(Element val1,Element val2);

Post createPost(char* content, int ownerID);
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

User createUser(char* firstName, char* lastName, char* email, char* password, int day, int month, int year);
void destroyUser(User user);
void freeUser(Element val);
Element copyUser(Element val);
int compareUser(Element val1,Element val2);

int getUserID(User user);

char* getUserEmail(User user);

char* getUserPassword(User user);

void updatePassword(User user, char* password);

List getUserFriends(User user);

List getUserFriendRequests(User user);

bool FriendRequestExist(User user, int friendId);

bool FriendExist(User user, int friendId);

ListResult userAddMessage(User user, Message msg);

ListResult userAddPost(User user, Post post);

ListResult userDeletePost(User user, int postId);

Post getPostById(User user, int postId);

ListResult userAddCommentOnPost(User owner,int postId,int commenterId,char* content);

ListResult userLikePost(User owner, int postId, int likerId);

ListResult userUnlikePost(User owner,int postId,int likerId);

#endif //PROJECTEMOT_HELPERS_H
