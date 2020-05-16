//
// Created by d_dab on 15/05/2020.
//

#include <assert.h>
#include "helpers.h"

int comment_id = 1;
int post_id = 1;
int message_id = 1;
int user_id;


/**
 * struct that describes date.
 * gonna use this struct in personal info of the user, for example his birthday.
 * */
struct date_t{
    int day;
    int month;
    int year;
};


/**
 * struct that describes a post.
 * it had a list of the users that liked this post, a list of comments on this post, number of likes on this post,
 * the content of the post.
 * */
struct post_t{
    int id;
    int ownerID;
    List likes;
    List comments;
    int likesNum;
    char* content;
};

/**
 * sturct that describes a message between two users.
 * it had ID for every message, ID of the sender user, ID of the receiver, the content of the message.
 * */
struct message_t{
    int msgId;
    int from;
    int to;
    char* content;
};

/**
 * a struct that discribes broadcast.
 * it had an ID for each broadcast, as well as name, and a list of users.
 * */
struct broadcast_t{
    int Id;
    char* name;
    List users;
};

/**
 * struct that describes a comment.
 * each comment had ID, the ID of the post that it was written on, the ID of the comment owner (user), the content of
 * the comment.
 * */
struct comment_t{
    int id;
    int postId;
    int ownerId;
    char* content;
};

Date createBirthdate(int day, int month, int year){
    Date birthdayDate = malloc(sizeof(Date));
    if(!birthdayDate){
        return NULL;
    }
    birthdayDate->day=day;
    birthdayDate->month=month;
    birthdayDate->year=year;
    return birthdayDate;
}

void freeEmpty (Element val){ assert(val != NULL); }
Element copyEmpty(Element val){ assert(val != NULL); return NULL; }
int compareEmpty(Element val1,Element val2){assert(val1 != NULL); assert(val2 != NULL); return 0; }

void freeInt (Element val){
    assert(!val);
    int* temp=(int*) val;
    free(temp);
}

Element copyInt(Element val){
    assert(!val);
    int* temp=(int*) val;
    int* res = malloc(sizeof(int));
    if(!res){
        return NULL;
    }
    *res = *temp;
    return res;
}

int compareInt(Element val1,Element val2){
    assert(!val1);
    assert(!val2);
    int* temp1=(int*) val1;
    int* temp2=(int*) val2;
    return (*temp1 == *temp2) ? 0 : 1;
}

Post createPost(char* content, int ownerID){
    Post post = malloc(sizeof(*post));
    if(!post){
        return NULL;
    }
    post->ownerID = ownerID;
    post->id = post_id;
    post_id++;
    post->content = charcpy(content);
    if(!(post->content )){
        free(post);
        return NULL;
    }
    post->likesNum = 0;
    post->likes = list_new(freeInt,copyInt,compareInt);
    if(!(post->likes)){
        free(post);
        free(post->content);
        return NULL;
    }
    post->comments = list_new(freeComment,copyComment,compareComment);
    if(!(post->comments)){
        free(post);
        free(post->content);
        list_destroy(post->likes);
        return NULL;
    }
    return post;
}

void destroyPost(Post post){
    assert(post != NULL);
    list_destroy(post->likes);
    list_destroy(post->comments);
    free(post->content);
    free(post);
}

void freePost(Element val){
    assert(val != NULL);
    Post post = (Post) val;
    destroyPost(post);
}

Element copyPost(Element val) {
    assert(val != NULL);
    Post post = (Post) val;
    return createPost(post->content, post->ownerID);
}

int comparePost(Element val1,Element val2){
    assert(val1 != NULL);
    assert(val2 != NULL);
    Post post1 = (Post) val1;
    Post post2 = (Post) val2;
    return (post1->id - post2->id);
}

Comment createComment(char* content,int postId ,int ownerId){
    assert(content != NULL);
    Comment  comment = malloc(sizeof(*comment));
    if(!comment){
        return NULL;
    }
    comment->content = charcpy(content);
    if(!(comment->content)){
        free(comment);
        return NULL;
    }
    comment->id = comment_id;
    comment_id++;
    comment->postId = postId;
    comment->ownerId = ownerId;
    return comment;
}

void destroyComment(Comment comment){
    assert(comment != NULL);
    free(comment->content);
    free(comment);
}

void freeComment(Element val){
    assert(val != NULL);
    Comment comment = (Comment) val;
    destroyComment(comment);
}

Element copyComment(Element val){
    assert(val != NULL);
    Comment comment = (Comment) val;
    return createComment(comment->content, comment->postId, comment->ownerId);
}

int compareComment(Element val1,Element val2){
    assert(val1 != NULL);
    assert(val2 != NULL);
    Comment comment1 = (Comment) val1;
    Comment comment2 = (Comment) val2;
    return (comment1->id - comment2->id);
}

Message createMessage(int sender, int receiver, char* content){
    Message msg = malloc(sizeof(*msg));
    if(!msg){
        return NULL;
    }
    msg->msgId = message_id;
    message_id++;
    msg->from = sender;
    msg->to = receiver;
    msg->content = charcpy(content);
    if(!(msg->content)){
        free(msg);
        return NULL;
    }
    return msg;
}

void destroyMessage(Message msg){
    assert(msg!=NULL);
    free(msg->content);
    free(msg);
}

void freeMessage(Element val){
    assert(val != NULL);
    Message message = (Message) val;
    destroyMessage(message);
}

Element copyMessage(Element val){
    assert(val != NULL);
    Message message = (Message) val;
    return createMessage(message->from,message->to,message->content);
}

int compareMessage(Element val1,Element val2){
    assert(val1 != NULL);
    assert(val2 != NULL);
    Message message1 = (Message) val1;
    Message message2 = (Message) val2;
    return (message1->msgId - message2->msgId);
}


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
    List friends;
    List friendsRequests;
    List chats;
    List posts;
    List broadcasts;
};

/* radikot **********************/
User createUser( char* firstName, char* lastName, char* email, char* password, int day, int month, int year){
    User user = malloc(sizeof(user));
    if(!user) {
        return NULL;
    }

    Date birthdayDate = createBirthdate(day,month,year);
    if(!birthdayDate){
        free(user);
        return NULL;
    }

    char* f_Name=charcpy(firstName);
    if(!f_Name) {
        free(birthdayDate);
        free(user);
        return NULL;
    }
    char* l_Name=charcpy(lastName);
    if(!l_Name) {
        free(birthdayDate);
        free(f_Name);
        free(user);
        return NULL;
    }
    char* email1=charcpy(email);
    if(!email1) {
        free(birthdayDate);
        free(f_Name);
        free(l_Name);
        free(user);
        return NULL;
    }
    char* pass=charcpy(password);
    if(!pass) {
        free(birthdayDate);
        free(f_Name);
        free(l_Name);
        free(email1);
        free(user);
        return NULL;
    }

    List friends=list_new(freeInt,copyInt,compareInt);
    if(!friends){
        free(birthdayDate);
        free(f_Name);
        free(l_Name);
        free(email1);
        free(pass);
        free(user);
        return NULL;
    }

    List friendsRequests=list_new(freeInt,copyInt,compareInt);
    if(!friendsRequests){
        free(birthdayDate);
        free(f_Name);
        free(l_Name);
        free(email1);
        free(pass);
        list_destroy(friends);
        free(user);
        return NULL;
    }

    List broadcasts=list_new(freeEmpty,copyEmpty,compareEmpty);
    if(!broadcasts){
        free(birthdayDate);
        free(f_Name);
        free(l_Name);
        free(email1);
        free(pass);
        list_destroy(friends);
        list_destroy(friendsRequests);
        free(user);
        return NULL;
    }

    List posts=list_new(freePost,copyPost,comparePost);
    if(!posts){
        free(birthdayDate);
        free(f_Name);
        free(l_Name);
        free(email1);
        free(pass);
        list_destroy(friends);
        list_destroy(friendsRequests);
        list_destroy(broadcasts);
        free(user);
        return NULL;
    }

    List chats=list_new(freeMessage,copyMessage,compareMessage);
    if(!chats){
        free(birthdayDate);
        free(f_Name);
        free(l_Name);
        free(email1);
        free(pass);
        list_destroy(friends);
        list_destroy(friendsRequests);
        list_destroy(broadcasts);
        list_destroy(posts);
        free(user);
        return NULL;
    }

    user -> id = user_id;
    user_id++;
    user->firstName = f_Name;
    user->lastName = l_Name;
    user->email = email1;
    user->password = pass;
    user->birthday = birthdayDate;
    user->broadcasts = broadcasts;
    user->chats = chats;
    user->friends = friends;
    user->friendsRequests = friendsRequests;
    user->posts = posts;
    return user;
}


void destroyUser(User user){
    assert(user != NULL);
    free(user->email);
    free(user->password);
    free(user->firstName);
    free(user->lastName);
    list_destroy(user->posts);
    list_destroy(user->friendsRequests);
    list_destroy(user->friends);
    list_destroy(user->chats);
    list_destroy(user->broadcasts);
    free(user);
}

int getUserID(User user){
    assert(user != NULL);
    return user->id;
}

void freeUser(Element val){
    assert(val != NULL);
    User user = (User) val;
    destroyUser(user);
}

Element copyUser(Element val) {
    assert(val != NULL);
    User user = (User) val;
    return createUser(user->firstName,user->lastName, user->email, user->password,
                      user->birthday->day, user->birthday->month,user->birthday->year);
}

int compareUser(Element val1,Element val2){
    assert(val1 != NULL);
    assert(val2 != NULL);
    User user1 = (User) val1;
    User user2 = (User) val2;
    return (getUserID(user1) - getUserID(user2));
}

char* getUserEmail(User user){
    assert(user != NULL);
    return user->email;
}

char* getUserPassword(User user){
    assert(user != NULL);
    return user->password;
}

void updatePassword(User user, char* password){
    assert(user != NULL);
    assert(password != NULL);
    free(user->password);
    user->password = charcpy(password);
}

List getUserFriends(User user){
    assert(user != NULL);
    return user->friends;
}
List getUserFriendRequests(User user){
    assert(user != NULL);
    return user->friendsRequests;
}

bool FriendExist(User user, int friendId){
    assert(user != NULL);
    assert(friendId > 0);
    FOR_EACH(item, user->friends){
        int* temp = (int*) (item->data);
        if( *temp == friendId ) {
            return true;
        }
    }
    return false;
}

bool FriendRequestExist(User user, int friendId){
    assert(user != NULL);
    assert(friendId > 0);
    FOR_EACH(item, user->friendsRequests){
        int* temp = (int*) (item->data);
        if( *temp == friendId ) {
            return true;
        }
    }
    return false;
}

ListResult userAddMessage(User user, Message msg){
    assert(user != NULL);
    assert(msg != NULL);
    return list_add(user->chats, msg);
}

ListResult userAddPost(User user, Post post){
    assert(user != NULL);
    assert(post != NULL);
    return list_add(user->posts, post);
}

ListResult userDeletePost(User user, int postId){
    assert(user != NULL);
    assert(postId > 0);
    Post post = getPostById(user,postId);
    if(!post) return LIST_NOT_EXIST;
    return list_delete(user->posts, post);
}

int postGetId(Post post){
    return post->id;
}

int postGetOwner(Post post){
    return post->ownerID;
}

Post getPostById(User user, int postId){
    assert(user != NULL);
    assert(postId > 0);
    FOR_EACH(post,user->posts){
        if(postId == postGetId(post->data)){
            return post->data;
        }
    }
}

ListResult userAddCommentOnPost(User owner,int postId,int commenterId,char* content){
    assert(owner != NULL);
    assert(postId > 0);
    assert(commenterId > 0);
    assert(content != NULL);

    Post post = getPostById(owner, postId);
    if(!post) return LIST_NOT_EXIST;
    Comment comment = createComment(content,postId,commenterId);
    if(!comment) return LIST_MEMORY_ERROR;
    ListResult res = list_add(post->comments,comment);
    destroyComment(comment);
    return res;
}

ListResult userLikePost(User owner, int postId, int likerId){
    assert(owner != NULL);
    assert(postId > 0);
    assert(likerId > 0);

    Post post = getPostById(owner, postId);
    if(!post) return LIST_NOT_EXIST;
    ListResult res = list_add(post->likes,&likerId);
    if(res == LIST_SUCCESS){
        post->likesNum++ ;
    }
    destroyPost(post);
    return res;
}

ListResult userUnlikePost(User owner,int postId,int likerId){
    assert(owner != NULL);
    assert(postId > 0);
    assert(likerId > 0);
    Post post = getPostById(owner, postId);
    if(!post) return LIST_NOT_EXIST;
    ListResult res = list_delete(post->likes,&likerId);
    if(res == LIST_NOT_EXIST){
        return LIST_ITEM2_DOES_NOT_EXIST;
    }
    if(res == LIST_SUCCESS){
        post->likesNum-- ;
    }
    destroyPost(post);
    return res;
}
/* radikot **********************/