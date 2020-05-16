//
// Created by d_dab on 15/05/2020.
//

#include <assert.h>
#include "API.h"

int user_id = 1;

/**
 * sturct that inculdes a list of all the users in the program.
 * */
struct API_t {
    List users;
    List used_emails;
};

API initSystem() {
    API api = malloc(sizeof(api));
    api->users = list_new(freeUser, copyUser, compareUser);
    if (!(api->users)) {
        free(api);
        return NULL;
    }
    api->used_emails = list_new(freeChar, copyChar, compareChar);
    if (!(api->used_emails)) {
        list_destroy(api->users);
        free(api);
        return NULL;
    }
    printf("CREATE TABLE users \n"
           "(\n"
           "userId integer NOT NULL,\n"
           "email text NOT NULL,\n"
           "password text NOT NULL,\n"
           "firstName text NOT NULL,\n"
           "lastName text NOT NULL,\n"
           "day integer NOT NULL,\n"
           "month integer NOT NULL,\n"
           "year integer NOT NULL,\n"
           "    PRIMARY KEY (userId),\n"
           "    CHECK (userId > 0)\n"
           ")");
    printf("CREATE TABLE friends \n"
           "(\n"
           "    userId1 integer NOT NULL,\n"
           "    userId2 integer NOT NULL,\n"
           "    PRIMARY KEY (userId1 , userId2),\n"
           "    FOREIGN KEY (userId1) REFERENCES users(userId) ON DELETE CASCADE ,\n"
           "    FOREIGN KEY (userId2) REFERENCES users(userId) ON DELETE CASCADE"
           ")");
    printf("CREATE TABLE friendsRequests \n"
           "(\n"
           "    senderId integer NOT NULL,\n"
           "    receiverId integer NOT NULL,\n"
           "    PRIMARY KEY (senderId , receiverId),\n"
           "    FOREIGN KEY (senderId) REFERENCES users(userId) ON DELETE CASCADE ,\n"
           "    FOREIGN KEY (receiverId) REFERENCES users(userId) ON DELETE CASCADE"
           ")");
    printf("CREATE TABLE messages \n"
           "(\n"
           "    messageId integer NOT NULL,\n"
           "    senderId integer NOT NULL,\n"
           "    receiverId integer NOT NULL,\n"
           "    content text NOT NULL,\n"
           "    PRIMARY KEY (messageId),\n"
           "    FOREIGN KEY (senderId) REFERENCES users(userId) ON DELETE CASCADE ,\n"
           "    FOREIGN KEY (receiverId) REFERENCES users(userId) ON DELETE CASCADE"
           ")");
    printf("CREATE TABLE posts \n"
           "(\n"
           "    postId integer NOT NULL,\n"
           "    publisherId integer NOT NULL,\n"
           "    receiverId integer NOT NULL,\n"
           "    content text NOT NULL,\n"
           "    PRIMARY KEY (postId),\n"
           "    FOREIGN KEY (publisherId) REFERENCES users(userId) ON DELETE CASCADE"
           ")");
    printf("CREATE TABLE likes \n"
           "(\n"
           "    likerId integer NOT NULL,\n"
           "    postId integer NOT NULL,\n"
           "    PRIMARY KEY (likerId, postId),\n"
           "    FOREIGN KEY (postId) REFERENCES posts(postId) ON DELETE CASCADE"
           ")");
    printf("CREATE TABLE comments \n"
           "(\n"
           "    commentId integer NOT NULL,\n"
           "    postId integer NOT NULL,\n"
           "    commenterId integer NOT NULL,\n"
           "    content text NOT NULL,\n"
           "    PRIMARY KEY (commentId),\n"
           "    FOREIGN KEY (postId) REFERENCES posts(postId) ON DELETE CASCADE"
           "    FOREIGN KEY (commenterId) REFERENCES users(userId) ON DELETE CASCADE"
           ")");

}


APIRes createUserInAPI(API api, char *firstName, char *lastName, char *email, char *password, int day, int month, int year) {
    assert(api != NULL);
    assert(firstName != NULL);
    assert(lastName != NULL);
    assert(email != NULL);
    assert(password != NULL);

    char *email1 = list_get_element(api->used_emails, email);
    if (email1 != NULL) {
        return API_EMAIL_ALREADY_USED;
    }
    User user = createUser(firstName, lastName, email, password, day, month, year);
    if (!user) {
        return API_MEMORY_ERROR;
    }
    ListResult res = list_add(api->users, user);
    destroyUser(user);
    if (res == LIST_MEMORY_ERROR) {
        return API_MEMORY_ERROR;
    }
    assert(res != LIST_ALREADY_EXIST);
    res = list_add(api->used_emails, email);
    if (res == LIST_MEMORY_ERROR) {
        return API_MEMORY_ERROR;
    }
    assert(res != LIST_ALREADY_EXIST);
    return API_SUCCESS;
//       printf("INSERT INTO users VALUES(%d,%s,%s,%s,%s,%d,%d,%d)",id,f_name,l_name,email1,pass,day,month,year);

}

APIRes deleteUser(API api, int userID) {
    User temp = createUser("temp", "temp", "temp", "temp", 1, 1, 2002);
    if (!temp) {
        return API_MEMORY_ERROR;
    }
    User user = list_get_element(api->users, temp);
    if (!user) {
        destroyUser(temp);
        return API_USER_DOES_NOT_EXIST;
    }
    ListResult res = list_delete(api->used_emails, getUserEmail(user));
    assert(res == LIST_SUCCESS);
    res = list_delete(api->users, user);
    assert(res == LIST_SUCCESS);
    destroyUser(temp);
    return API_SUCCESS;
    //printf("DELETE FROM users WHERE userId=%d",userID);
}

User getUserByEmail(API api, char *email) {
    assert(api != NULL);
    assert(email != NULL);
    FOR_EACH(user_it, api->users) {
        if (!(charcmp(getUserEmail(user_it->data), email))) {
            return user_it->data;
        }
    }
    return NULL;
}

User getUserById(API api, int userId) {
    assert(api != NULL);
    assert(userId > 0);
    FOR_EACH(user_it, api->users) {
        if (getUserID(user_it->data) == userId) {
            return user_it->data;
        }
    }
    return NULL;
}

APIRes login(API api, char *email, char *password) {
    assert(email != NULL);
    assert(password != NULL);
    User user = getUserByEmail(api, email);
    if (!user) {
        return API_USER_DOES_NOT_EXIST;
    }
    if (!(charcmp(password, getUserPassword(user)))) {
        return API_SUCCESS;
    }
    return API_WRONG_PASSWORD;
}

APIRes logout(API api, int userID) {
    return API_SUCCESS;
}

APIRes changePassword(API api, int userID, char *oldPass, char *newPass) {
    assert(userID > 0);
    assert(oldPass != NULL);
    assert(newPass != NULL);
    User user = getUserById(api, userID);
    if (!user) {
        return API_USER_DOES_NOT_EXIST;
    }
    if (charcmp(getUserPassword(user), oldPass)) {
        return API_WRONG_PASSWORD;
    }
    updatePassword(user, newPass);
    return API_SUCCESS;
}

APIRes getPasswordById(API api, int userID) {
    assert(api != NULL);
    assert(userID > 0);
    User user = getUserById(api, userID);
    if (!user) {
        return API_USER_DOES_NOT_EXIST;
    }
    char *password = getUserPassword(user);
    // TODO : send the password by email to the user.
    return API_SUCCESS;
}

APIRes sendFriendRequest(API api, int userID, int friendId) {
    assert(api != NULL);
    assert(userID > 0);
    assert(friendId > 0);
    User user = getUserById(api, userID);
    User friend = getUserById(api, friendId);
    if (FriendExist(user, friendId)) {
        return API_ALREADY_FRIENDS;
    } else if (FriendRequestExist(user, friendId) || FriendRequestExist(friend, userID)) {
        return API_REQUEST_ALREADY_EXISTS;
    }
    ListResult res = list_add(getUserFriendRequests(user), &friendId);
    if (res == LIST_MEMORY_ERROR) {
        return API_MEMORY_ERROR;
    }
    return API_SUCCESS;
}

APIRes handleFriendRequest(API api, int userID, int friendId, Handle respond) {
    assert(api != NULL);
    assert(userID > 0);
    assert(friendId > 0);
    User user = getUserById(api, userID);
    User friend = getUserById(api, friendId);
    if (FriendExist(user, friendId)) {
        return API_ALREADY_FRIENDS;
    }
    if (FriendRequestExist(friend, userID)) {
        if (respond == ACCEPT) {
            ListResult res = list_add(getUserFriends(user), &friendId);
            assert(res != LIST_ALREADY_EXIST);
            if (res == LIST_MEMORY_ERROR) {
                return API_MEMORY_ERROR;
            }
            res = list_add(getUserFriends(friend), &userID);
            assert(res != LIST_ALREADY_EXIST);
            if (res == LIST_MEMORY_ERROR) {
                return API_MEMORY_ERROR;
            }
            return API_SUCCESS;
        } else {
            list_delete(getUserFriendRequests(user), &friendId);
            return API_SUCCESS;
        }
    } else {
        return API_REQUEST_DOES_NOT_EXIST;
    }
}

APIRes deleteFriend(API api, int userID, int friendId){
    assert(api != NULL);
    assert(userID > 0);
    assert(friendId > 0);
    User user = getUserById(api, userID);
    User friend = getUserById(api, friendId);
    if (!FriendExist(user, friendId)) {
        assert(!FriendExist(friend, userID));
        return API_NOT_A_FRIEND;
    }
    else{
        assert(FriendExist(friend, userID));
        list_delete(getUserFriends(user),&friendId);
        list_delete(getUserFriends(friend),&userID);
        return API_SUCCESS;
    }
}

APIRes sendMessage(API api, int userID, int receiverId, char *content){
    assert(api != NULL);
    assert(content != NULL);
    assert(userID > 0);
    assert(receiverId > 0);
    User user = getUserById(api,userID);
    if(!user){
        return API_USER_DOES_NOT_EXIST;
    }
    User receiver = getUserById(api, receiverId);
    if(!receiver){
        return API_USER_DOES_NOT_EXIST;
    }
    Message message = createMessage(userID, receiverId,content);
    if(!message){
        return API_MEMORY_ERROR;
    }
    ListResult res = userAddMessage(user , message);
    if(res == LIST_MEMORY_ERROR){
        return API_MEMORY_ERROR;
    }
    res = userAddMessage(receiver, message);
    if(res == LIST_MEMORY_ERROR){
        return API_MEMORY_ERROR;
    }
    destroyMessage(message);
}

APIRes publishPost(API api, int userId, char *content){
    assert(api != NULL);
    assert(content != NULL);
    assert(userId > 0);
    User user  = getUserById(api,userId);
    if(!user){
        return API_USER_DOES_NOT_EXIST;
    }
    Post post = createPost(content, userId);
    if(!post){
        return API_MEMORY_ERROR;
    }
    ListResult res = userAddPost(user, post);
    if(res == LIST_MEMORY_ERROR){
        return API_MEMORY_ERROR;
    }
    destroyPost(post);
}

APIRes deletePost(API api, int userID, int postId){
    assert(api != NULL);
    assert(userID > 0);
    assert(postId > 0);
    User user = getUserById(api,userID);
    if(!user) return API_USER_DOES_NOT_EXIST;
    ListResult res = userDeletePost(user,postId);
    if(res == LIST_NOT_EXIST){
        return API_POST_DOES_NOT_EXIST;
    }
    return API_SUCCESS;
}

APIRes addCommentOnPost(API api, int commenterId, char *content, int postId, int ownerId){
    assert(api != NULL);
    assert(commenterId > 0);
    assert(content != NULL);
    assert(postId > 0);
    assert(ownerId > 0);
    User owner = getUserById(api,ownerId);
    if(!owner) return API_USER_DOES_NOT_EXIST;
    ListResult res = userAddCommentOnPost(owner,postId,commenterId,content);
    if(res == LIST_MEMORY_ERROR){
        return API_MEMORY_ERROR;
    }
    assert(res != LIST_ALREADY_EXIST);
    return API_SUCCESS;
}

APIRes likePost(API api, int likerId, int postId, int ownerId){
    assert(api != NULL);
    assert(likerId > 0);
    assert(postId > 0);
    assert(ownerId > 0);
    User owner = getUserById(api,ownerId);
    if(!owner) return  API_USER_DOES_NOT_EXIST;
    ListResult res = userLikePost(owner, postId, likerId);
    if(res == LIST_MEMORY_ERROR){
        return API_MEMORY_ERROR;
    }
    if(res == LIST_NOT_EXIST){
        return API_POST_DOES_NOT_EXIST;
    }
    return API_SUCCESS;
}

APIRes unLikePost(API api, int likerId, int postId, int ownerId){
    assert(api != NULL);
    assert(likerId > 0);
    assert(postId > 0);
    assert(ownerId > 0);
    User owner = getUserById(api,ownerId);
    if(!owner) return  API_USER_DOES_NOT_EXIST;
    ListResult res = userUnlikePost(owner, postId, likerId);
    if(res == LIST_MEMORY_ERROR){
        return API_MEMORY_ERROR;
    }
    if(res == LIST_NOT_EXIST){
        return API_POST_DOES_NOT_EXIST;
    }
    if(res == LIST_ITEM2_DOES_NOT_EXIST){
        return API_DID_NOT_LIKE_POST;
    }
    return API_SUCCESS;
}

