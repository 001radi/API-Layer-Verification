//
// Created by d_dab on 15/05/2020.
//

#include <assert.h>
#include "helpers.h"
#include "API.h"
#include <seahorn/seahorn.h>

int comment_id = 1;
int post_id = 1;
int message_id = 1;

extern int nd();


/**
 * struct that describes date.
 * gonna use this struct in personal info of the user, for example his birthday.
 * */
struct date_t {
    int day;
    int month;
    int year;
};


/**
 * struct that describes a post.
 * it had a list of the users that liked this post, a list of comments on this post, number of likes on this post,
 * the content of the post.
 * */
struct post_t {
    int id;
    int ownerID;
    List likes;
    List comments;
    int likesNum;
    char *content;
};

/**
 * sturct that describes a message between two users.
 * it had ID for every message, ID of the sender user, ID of the receiver, the content of the message.
 * */
struct message_t {
    int msgId;
    int from;
    int to;
    char *content;
};

/**
 * a struct that discribes broadcast.
 * it had an ID for each broadcast, as well as name, and a list of users.
 * */
struct broadcast_t {
    int Id;
    char *name;
    List users;
};

/**
 * struct that describes a comment.
 * each comment had ID, the ID of the post that it was written on, the ID of the comment owner (user), the content of
 * the comment.
 * */
struct comment_t {
    int id;
    int postId;
    int ownerId;
    char *content;
};

Date createBirthdate(int day, int month, int year) {
    Date birthdayDate = xmalloc(sizeof(*birthdayDate));
    sassert(birthdayDate != NULL);
    birthdayDate->day = day;
    birthdayDate->month = month;
    birthdayDate->year = year;
    return birthdayDate;
}

void freeEmpty(Element val) { assume(val != NULL); }

Element copyEmpty(Element val) {
    assume(val != NULL);
    return NULL;
}

int compareEmpty(Element val1, Element val2) {
    assume(val1 != NULL);
    assume(val2 != NULL);
    return 0;
}

void freeInt(Element val) {
    assume(val != NULL);
    int *temp = (int *) val;
    free(temp);
}

Element copyInt(Element val) {
    assume(val != NULL);
    int *temp = (int *) val;
    int *res = xmalloc(sizeof(int));
    sassert(res != NULL);
    *res = *temp;
    return res;
}

int compareInt(Element val1, Element val2) {
    assume(val1 != NULL);
    assume(val2 != NULL);
    int *temp1 = (int *) val1;
    int *temp2 = (int *) val2;
    return (*temp1 == *temp2) ? 0 : 1;
}

Post createPostAux (char* content, int ownerID){
    assume(content != NULL);
    assume(ownerID > 0);
    Post post = xmalloc(sizeof(*post));
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


void createPost(char *content, User user, int postId) {
    assume(content != NULL);
    assume(user != NULL);
    assume(postId > 0);
    for (int i = 0; i < user->postsSize; ++i) {
        if (user->user_posts[i]->id == postId) {
            user->user_posts[i]->has = 1;
            Post post = createPostAux(content,getUserID(user));
            assume(post!=NULL);
            user->user_posts[i]->post = post;
            user->postsNum++;
            break;
        }
    }

    if (user->postsNum == user->postsSize) {
        int postsArrayInitSize = user->postsSize;
        user->postsSize *= 2;
        user->user_posts = xrealloc(user->user_posts, user->postsSize * (sizeof(userPost)));
        sassert(user->user_posts > 0);
        for (int i = postsArrayInitSize; i < user->postsSize - 1; ++i) {
            int offset;
            unsigned int size;
            offset = 0;
            size = sizeof(userPost) * (user->postsSize);
            offset = sizeof(userPost) * i;
            sassert(offset < size);
            sassert(offset >= 0);
            user->user_posts[i] = xmalloc(sizeof(userPost));
            sassert(user->user_posts[i] != NULL);
            int x = nd();
            assume(x > 0);
            user->user_posts[i]->id = x;
            user->user_posts[i]->has = 0;
        }
    }
}

void destroyPost(Post post) {
    assume(post != NULL);
    list_destroy(post->likes);
    list_destroy(post->comments);
    free(post->content);
    free(post);
}

Comment createComment(char *content, int postId, int ownerId) {
    assume(content != NULL);
    Comment comment = xmalloc(sizeof(*comment));
    if (!comment) {
        return NULL;
    }
    comment->content = charcpy(content);
    if (!(comment->content)) {
        free(comment);
        return NULL;
    }
    comment->id = comment_id;
    comment_id++;
    comment->postId = postId;
    comment->ownerId = ownerId;
    return comment;
}

void destroyComment(Comment comment) {
    assume(comment != NULL);
    free(comment->content);
    free(comment);
}

void freeComment(Element val) {
    assume(val != NULL);
    Comment comment = (Comment) val;
    destroyComment(comment);
}

Element copyComment(Element val) {
    assume(val != NULL);
    Comment comment = (Comment) val;
    return createComment(comment->content, comment->postId, comment->ownerId);
}

int compareComment(Element val1, Element val2) {
    assume(val1 != NULL);
    assume(val2 != NULL);
    Comment comment1 = (Comment) val1;
    Comment comment2 = (Comment) val2;
    return (comment1->id - comment2->id);
}

Message createMessage(int sender, int receiver, char *content) {
    assume(content != NULL);
    Message msg = xmalloc(sizeof(*msg));
    if (!msg) {
        return NULL;
    }
    msg->msgId = message_id;
    message_id++;
    msg->from = sender;
    msg->to = receiver;
    msg->content = charcpy(content);
    if (!(msg->content)) {
        free(msg);
        return NULL;
    }
    return msg;
}

void destroyMessage(Message msg) {
    assume(msg != NULL);
    free(msg->content);
    free(msg);
}

void freeMessage(Element val) {
    assume(val != NULL);
    Message message = (Message) val;
    destroyMessage(message);
}

Element copyMessage(Element val) {
    assume(val != NULL);
    Message message = (Message) val;
    return createMessage(message->from, message->to, message->content);
}

int compareMessage(Element val1, Element val2) {
    assume(val1 != NULL);
    assume(val2 != NULL);
    Message message1 = (Message) val1;
    Message message2 = (Message) val2;
    return (message1->msgId - message2->msgId);
}


/* radikot **********************/
User
createUser(int user_id, char *firstName, char *lastName, char *email, char *password, int day, int month, int year) {
    assume(firstName != NULL);
    assume(lastName != NULL);
    assume(email != NULL);
    assume(password != NULL);
    User user = xmalloc(sizeof(*user));
    sassert(user > 0);
    user->friend_req_count = 0;
    user->friends_req_size = 1;
    user->friends_ghosts = xmalloc(sizeof(friendsReq)*1);
    sassert(user->friends_ghosts != NULL);
    for (int i = 0; i < user->friends_req_size ; ++i) {
        //verf
        int offset;
        unsigned int size;
        // initialization of the ghost variables
        offset = 0;
        size = sizeof(friendsReq) * (user->friends_req_size);
        // update offset
        offset = sizeof(friendsReq) * i;
        // check the array write is in-bounds
        sassert(offset < size);
        sassert(offset >= 0);

        user->friends_ghosts[i] = xmalloc(sizeof(friendsReq));
        sassert(user->friends_ghosts[i] != NULL);
        int x = nd();
        assume(x>0);
        user->friends_ghosts[i]->val = x;
        user->friends_ghosts[i]->requested = 0;
        user->friends_ghosts[i]->accepted = 0;
    }
    user->user_posts = xmalloc(sizeof(userPost)*1);
    sassert(user->user_posts);
    user->postsSize = 1;
    user->postsNum = 0;
    for (int j = 0; j < user->postsSize ; ++j) {
        int offset = 0;
        unsigned int size;
        size = sizeof(userPost) *(user->postsSize);
        offset = sizeof(userPost) * j;
        sassert(offset < size);
        sassert(offset >= 0);

        user->user_posts[j] = xmalloc(sizeof(userPost));
        sassert(user->user_posts[j] != NULL);
        int x = nd();
        assume(x>0);
        user->user_posts[j]->id = x;
        user->user_posts[j]->has = 0;
    }


    Date birthdayDate = createBirthdate(day, month, year);
    sassert(birthdayDate > 0);

    char *f_Name = charcpy(firstName);
    sassert(f_Name > 0);

    char *l_Name = charcpy(lastName);
    sassert(l_Name > 0);

    char *email1 = charcpy(email);
    sassert(email1 > 0);

    char *pass = charcpy(password);
    sassert(pass > 0);

    List friends = list_new(freeInt, copyInt, compareInt);
    sassert(friends > 0);

    List friendsRequests = list_new(freeInt, copyInt, compareInt);
    sassert(friendsRequests > 0);
    List chats = list_new(freeMessage, copyMessage, compareMessage);
    sassert(chats > 0);

    user->id = user_id;
    user->firstName = f_Name;
    user->lastName = l_Name;
    user->email = email1;
    user->password = pass;
    user->birthday = birthdayDate;
    user->chats = chats;
    return user;
}


void destroyUser(User user) {
    assume(user != NULL);
    free(user->email);
    free(user->password);
    free(user->firstName);
    free(user->lastName);
    free(user->birthday);
    list_destroy(user->posts);
    list_destroy(user->chats);
    free(user);
}

int getUserID(User user) {
    assume(user != NULL);
    return user->id;
}

void freeUser(Element val) {
    assume(val != NULL);
    User user = (User) val;
    destroyUser(user);
}

Element copyUser(Element val) {
    assume(val != NULL);
    User user = (User) val;
    User res = createUser(user->id, user->firstName, user->lastName, user->email, user->password,
                          user->birthday->day, user->birthday->month, user->birthday->year);
    sassert(res != NULL);
    return res;
}

int compareUser(Element val1, Element val2) {
    assume(val1 != NULL);
    assume(val2 != NULL);
    User user1 = (User) val1;
    User user2 = (User) val2;
    return (getUserID(user1) - getUserID(user2));
}

char *getUserEmail(User user) {
    assume(user != NULL);
    return user->email;
}

char *getUserPassword(User user) {
    assert(user != NULL);
    return user->password;
}

void updatePassword(User user, char *password) {
    assume(user != NULL);
    assume(password != NULL);
    free(user->password);
    user->password = charcpy(password);
}

int FriendExist(User user, int friendId) {
    assume(user != NULL);
    assume(friendId > 0);
    for (int i = 0; i < user->friends_req_size; ++i) {
        //radikot: changing the if, added has==1
        if ((user->friends_ghosts[i]->val == friendId) && (user->friends_ghosts[i]->accepted == 1)) {
            return 1;
        }
    }
    return 0;
}

int FriendRequestExist(User user, int friendId) {
    assume(user != NULL);
    assume(friendId > 0);
    for (int i = 0; i < N; ++i) {
        //radikot: changing the if, added has==1
        if ((user->friends_ghosts[i]->val == friendId) && (user->friends_ghosts[i]->requested == 1)) {
            return 1;
        }
    }
    return 0;
}

ListResult userAddMessage(User user, Message msg) {
    assert(user != NULL);
    assert(msg != NULL);
    return list_add(user->chats, msg);
}

ListResult userAddPost(User user, Post post) {
    assert(user != NULL);
    assert(post != NULL);
    return list_add(user->posts, post);
}

ListResult userDeletePost(User user, int postId) {
    assume(user != NULL);
    assume(postId > 0);

    for (int j = 0; j < user->postsSize; ++j) {
        int offset;
        unsigned int size;
        offset = 0;
        size = sizeof(userPost) * (user->postsSize);
        offset = sizeof(userPost) * j;
        sassert(offset < size);
        sassert(offset >= 0);
        if (user->user_posts[j]->id == postId) {
            userPost userPost1 = user->user_posts[j];
            destroyPost(userPost1->post);
            free(userPost1);
            user->postsNum--;
            for (int i = j; i < user->postsSize; ++i) {
                user->user_posts[i] = user ->user_posts[i+1];
                offset = sizeof(userPost) * i;
                int bgn = sizeof(userPost) * j;
                int end = sizeof(userPost) * user->postsSize;
                sassert(offset >= bgn);
                sassert(offset < end);
            }
            if ((user->postsNum) * 4 == user->postsSize) {
                user->postsSize /= 2;
                user->user_posts = xrealloc(user->user_posts, sizeof(userPost) * user->postsSize);
                sassert(user->user_posts > 0);
            }
            break;
        }
    }
}

int postGetId(Post post) {
    assert (post != NULL);
    return post->id;
}

int postGetOwner(Post post) {
    return post->ownerID;
}

Post getPostById(User user, int postId) {
    assert(user != NULL);
    assert(postId > 0);
    FOR_EACH(post, list_get_head(user->posts)) {
        if (postId == postGetId(post->data)) {
            return post->data;
        }
    }
    return NULL;
}

ListResult userAddCommentOnPost(User owner, int postId, int commenterId, char *content) {
    assert(owner != NULL);
    assert(postId > 0);
    assert(commenterId > 0);
    assert(content != NULL);

    Post post = getPostById(owner, postId);
    if (!post) return LIST_NOT_EXIST;
    Comment comment = createComment(content, postId, commenterId);
    if (!comment) return LIST_MEMORY_ERROR;
    ListResult res = list_add(post->comments, comment);
    destroyComment(comment);
    return res;
}

List getLikers(User owner, int postId) {
    assert(owner != NULL);
    assert(postId > 0);
    Post post = getPostById(owner, postId);
    if (!post) return NULL;
    return post->likes;
}

int likersNum(User owner, int postId) {
    assert(owner != NULL);
    assert(postId > 0);
    Post post = getPostById(owner, postId);
    if (!post) return -1;
    return post->likesNum;
}

List getComments(User owner, int postId) {
    assert(owner != NULL);
    assert(postId > 0);
    Post post = getPostById(owner, postId);
    if (!post) return NULL;
    return post->comments;
}

int commentsNum(User owner, int postId) {
    assert(owner != NULL);
    assert(postId > 0);
    Post post = getPostById(owner, postId);
    if (!post) return -1;
    return list_size(post->comments);
}

ListResult userLikePost(User owner, int postId, int likerId) {
    assert(owner != NULL);
    assert(postId > 0);
    assert(likerId > 0);
    Post post = getPostById(owner, postId);
    if (!post) return LIST_NOT_EXIST;

    List likers = getLikers(owner, postId);
    if (list_size(likers) > 0) {
        FOR_EACH(item, list_get_head(likers)) {
            int *temp = (int *) (item->data);
            if (*temp == likerId) {
                return LIST_SUCCESS;
            }
        }
    }

    ListResult res = list_add(post->likes, &likerId);
    if (res == LIST_SUCCESS) {
        post->likesNum++;
    }
    return res;
}


ListResult userUnlikePost(User owner, int postId, int likerId) {
    assert(owner != NULL);
    assert(postId > 0);
    assert(likerId > 0);
    Post post = getPostById(owner, postId);
    if (!post) return LIST_NOT_EXIST;
    ListResult res = list_delete(post->likes, &likerId);
    if (res == LIST_NOT_EXIST) {
        return LIST_ITEM2_DOES_NOT_EXIST;
    }
    if (res == LIST_SUCCESS) {
        post->likesNum--;
    }
    return res;
}

List getUserMessages(User user) {
    assert(user != NULL);
    return user->chats;
}

int getPostId(Post post) {
    return post->id;
}

void *xmalloc(size_t size) {
    assume(size > 0);
    void *ptr;
    ptr = malloc(size);
    assume(ptr != NULL);
    return ptr;
}

void *xrealloc(void *ptr, size_t size) {
    assume(size > 0);
    assume(ptr > 0);
    void *ptr1 = realloc(ptr, size);
    assume(ptr1 > 0);
    return ptr1;
}
/* radikot **********************/