//
// Created by d_dab on 15/05/2020.
//

#include <assert.h>
#include "API.h"
#include "helpers.h"
#include <seahorn/seahorn.h>

extern int nd();


User getUserByEmail(API api, char *email);

/**
 * sturct that inculdes a list of all the users in the program.
 * */

Store store_create() {
    Store store = xmalloc(sizeof(Store));
    sassert(store != NULL);
    store->ghost_p = nd_ptr();
    store->ghost_q = nd_ptr();
    store->ghost_flag = 0;
    return store;
}

bool store_put(Store store, char *p, short sz) {
    store->sz = sz;

    store->ghost_flag = (store->ghost_p == p);

    if (store->ghost_flag) {
        store->ghost_p = p;
        return true;
    }

    return false;
}

inline void mem_eq(char *mem1, char *mem2, int sz) {
    for (int i = 0; i < sz; i++) assume(mem1[i] == mem2[i]);
}

#define MAX_SZ 6

void *store_get(Store store) {
    char *q = malloc(MAX_SZ);
    assume(store->sz < MAX_SZ);
    if (store->ghost_flag && q == store->ghost_q) {
        mem_eq(store->ghost_p, q, store->sz);
    }
    return q;
}


API initSystem() {
    API api = xmalloc(sizeof(*api));
    sassert(api != NULL);
    api->usersNum = 0;
    api->usersArraySize = 5;
    api->usersGhosts = xmalloc(sizeof(UserElement) * 5);
    sassert(api->usersGhosts != NULL);
    for (int j = 0; j < api->usersArraySize; ++j) {
        api->usersGhosts[j] = xmalloc(sizeof(UserElement));
        sassert(api->usersGhosts[j] != NULL);
        int x = nd();
        assume(x > 0);
        api->usersGhosts[j]->id = x;
        api->usersGhosts[j]->has = 0;

    }
    api->EmailsNum = 0;
    api->EmailsArraySize = 5;
    api->storedEmailes = xmalloc(sizeof(Store) * 5);
    sassert(api->storedEmailes > 0);
    for (int k = 0; k < api->EmailsArraySize; ++k) {
        api->storedEmailes[k] = store_create();
    }
    return api;
}

void destroyApi(API api) {
    assume(api != NULL);
    list_destroy(api->users);
    list_destroy(api->used_emails);
    free(api);
}


User createUserInAPI(API api, int user_id, char *firstName, char *lastName,
                     char *email, char *password, int day, int month, int year) {
    assume(api != NULL);
    assume(firstName != NULL);
    assume(lastName != NULL);
    assume(email != NULL);
    assume(password != NULL);
    bool flag = false;

    for (int i = 0; i < api->usersArraySize; ++i) {
        if (api->usersGhosts[i]->id == user_id) {
            User user = createUser(user_id, firstName, lastName, email, password, day, month, year);
            sassert(user != NULL);
            api->usersGhosts[i]->has = 1;
            api->usersGhosts[i]->user = user;
            int offset;
            unsigned int size;
            // initialization of the ghost variables
            offset = 0;
            size = sizeof(UserElement) * (api->usersArraySize);
            // update offset
            offset = sizeof(UserElement) * i;
            // check the array write is in-bounds
            sassert(offset < size);
            sassert(offset >= 0);
            api->usersNum++;
            if (api->usersNum == api->usersArraySize) {
                int usersArrayInitSize = api->usersArraySize;
                api->usersArraySize *= 2;
                api->usersGhosts = xrealloc(api->usersGhosts, api->usersArraySize * (sizeof(UserElement)));
                sassert(api->usersGhosts > 0);
                for (int j = usersArrayInitSize; j < api->usersArraySize - 1; ++j) {
                    api->usersGhosts[j] = xmalloc(sizeof(UserElement));
                    sassert(api->usersGhosts[j] != NULL);
                    int x = nd();
                    assume(x > 0);
                    api->usersGhosts[j]->id = x;
                    api->usersGhosts[j]->has = 0;
                }
            }
            for (int k = 0; k < api->EmailsArraySize; ++k) {
                bool put_flag = store_put(api->storedEmailes[k], email, M);
                if (put_flag) {
                    api->EmailsNum++;
                    char *emailGet = store_get(api->storedEmailes[k]);
                    sassert(*emailGet == *email);
                    if (api->EmailsNum == api->EmailsArraySize) {
                        int EmailsArrayInitSize = api->EmailsArraySize;
                        api->EmailsArraySize *= 2;
                        api->storedEmailes = xrealloc(api->storedEmailes, api->EmailsArraySize * (sizeof(Store)));
                        sassert(api->storedEmailes > 0);
                        for (int j = EmailsArrayInitSize; j < api->EmailsArraySize; ++j) {
                            api->storedEmailes[j] = store_create();
                        }
                    }

                }
            }
            return user;
        }
    }
    return NULL;
}

int ApiHasUser(API api, int user_id) {
    for (int i = 0; i < api->usersArraySize; i++) {
        if (user_id == api->usersGhosts[i]->id)
            return api->usersGhosts[i]->has;
    }
    int res = nd();
    assume(res > 0);
    return res;
}

APIRes deleteUserFromAPI(API api, int user_id) {
    assume(api != NULL);
    assume(user_id > 0);

    for (int j = 0; j < api->usersArraySize; ++j) {
        // ghost variables
        int offset;
        unsigned int size;
        // initialization of the ghost variables
        offset = 0;
        size = sizeof(UserElement) * (api->usersArraySize);
        // update offset
        offset = sizeof(UserElement) * j;
        // check the array write is in-bounds
        sassert(offset < size);
        sassert(offset >= 0);
        if (api->usersGhosts[j]->id == user_id) {
            UserElement userElem = api->usersGhosts[j];
            destroyUser(userElem->user);
            free(userElem);
            api->usersNum--;
            for (int i = j; i < api->usersArraySize; ++i) {
                api->usersGhosts[i] = api->usersGhosts[i + 1];
                offset = sizeof(UserElement) * i;
                int bgn = sizeof(UserElement) * j;
                int end = sizeof(UserElement) * api->usersArraySize;
                sassert(offset >= bgn);
                sassert(offset < end);
            }
            if ((api->usersNum) * 4 == api->usersArraySize) {
                api->usersArraySize /= 2;
                api->usersGhosts = xrealloc(api->usersGhosts, sizeof(UserElement) * api->usersArraySize);
                sassert(api->usersGhosts > 0);
            }
            break;
        }
    }
}

User getUserByEmail(API api, char *email) {
    assume(api != NULL);
    assume(email != NULL);
    FOR_EACH(user_it, list_get_head(api->users)) {
        if (!(charcmp(getUserEmail((User) user_it->data), email))) {
            return user_it->data;
        }
    }
    return NULL;
}

User getUserById(API api, int userId) {
    assume(api != NULL);
    assume(userId > 0);
    for (int i = 0; i < api->usersNum; ++i) {
        if (api->usersGhosts[i]->id == userId)
            return api->usersGhosts[i]->user;
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

APIRes changePassword(API api, int user_id, char *oldPass, char *newPass) {
    assume(oldPass != NULL);
    assume(newPass != NULL);
    assume(user_id > 0);
    User user = getUserById(api, user_id);
    if (!user) {
        return API_USER_DOES_NOT_EXIST;
    }
    if (charcmp(user->password, oldPass)) {
        return API_WRONG_PASSWORD;
    }
    updatePassword(user, newPass);
    return API_SUCCESS;
}

char *getPasswordByEmail(API api, char *email) {
    assert(api != NULL);
    assert(email != NULL);
    User user = getUserByEmail(api, email);
    if (!user) {
        return NULL;
    }
    char *password = getUserPassword(user);
    // TODO : send the password by email to the user.
    return password;
}

APIRes sendFriendRequest(API api, int friendId, User user) {
    assume(api != NULL);
    assume(friendId > 0);
    bool flag = false;
    assume(user != NULL);
    for (int i = 0; i < user->friends_req_size; ++i) {
        if (user->friends_ghosts[i]->val == friendId && user->friends_ghosts[i]->requested == 0) {
            user->friends_ghosts[i]->requested = 1;
            int res = FriendRequestExist(user, friendId);
            sassert(res);
            user->friend_req_count++;
            break;
        }
    }
    if (user->friend_req_count == user->friends_req_size) {
        int friendsArrayInitSize = user->friends_req_size;
        user->friends_req_size *= 2;
        user->friends_ghosts = xrealloc(user->friends_ghosts, user->friends_req_size * (sizeof(friendsReq)));
        sassert(user->friends_ghosts > 0);
        for (int i = friendsArrayInitSize; i < user->friends_req_size - 1; ++i) {
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
            assume(x > 0);
            user->friends_ghosts[i]->val = x;
            user->friends_ghosts[i]->requested = 0;
            user->friends_ghosts[i]->accepted = 0;
        }
    }
    return API_SUCCESS;
}


APIRes handleFriendRequest(API api, User user, int friendId, Handle respond) {
    assume(api != NULL);
    assume(user != NULL);
    assume(friendId > 0);
    bool flag = false;
    for (int j = 0; j < user->friends_req_size; ++j) {
        if (user->friends_ghosts[j]->val == friendId && user->friends_ghosts[j]->requested == 1) {
            //verf
            int offset;
            unsigned int size;
            // initialization of the ghost variables
            offset = 0;
            size = sizeof(friendsReq) * (user->friends_req_size);
            // update offset
            offset = sizeof(friendsReq) * j;
            // check the array write is in-bounds
            sassert(offset < size);
            sassert(offset >= 0);

            if (respond == ACCEPT) {
                user->friends_ghosts[j]->accepted = 1;
            } else {
                user->friends_ghosts[j]->accepted = 0;
                user->friends_ghosts[j]->requested = 0;
            }
            if (respond == ACCEPT) {
                int res = FriendExist(user, friendId);
                sassert(res);
//            }
            }
        }
        return API_SUCCESS;
    }
}

    APIRes deleteFriend(API api, char *userEmail, char *friendEmail) {
        assert(api != NULL);
        assert(userEmail != NULL);
        assert(friendEmail != NULL);
        User user = getUserByEmail(api, userEmail);
        User friend = getUserByEmail(api, friendEmail);
        if (!user || !friend) {
            return API_USER_DOES_NOT_EXIST;
        }
        int userID = getUserID(user);
        int friendId = getUserID(friend);
        if (!FriendExist(user, friendId)) {
            assert(!FriendExist(friend, userID));
            return API_NOT_A_FRIEND;
        } else {
            assert(FriendExist(friend, userID));
            list_delete(getUserFriends(user), &friendId);
            list_delete(getUserFriends(friend), &userID);
            return API_SUCCESS;
        }
    }

    APIRes sendMessage(API api, char *userEmail, char *receiverEmail, char *content) {
        assert(api != NULL);
        assert(content != NULL);
        assert(userEmail != NULL);
        assert(receiverEmail != NULL);
        User user = getUserByEmail(api, userEmail);
        if (!user) {
            return API_USER_DOES_NOT_EXIST;
        }
        User receiver = getUserByEmail(api, receiverEmail);
        if (!receiver) {
            return API_USER_DOES_NOT_EXIST;
        }
        int userID = getUserID(user);
        int receiverId = getUserID(receiver);
        Message message = createMessage(userID, receiverId, content);
        if (!message) {
            return API_MEMORY_ERROR;
        }
        ListResult res = userAddMessage(user, message);
        if (res == LIST_MEMORY_ERROR) {
            return API_MEMORY_ERROR;
        }
        res = userAddMessage(receiver, message);
        if (res == LIST_MEMORY_ERROR) {
            return API_MEMORY_ERROR;
        }
        destroyMessage(message);
        return API_SUCCESS;
    }

    List getMessages(API api, char *email) {
        assert(api != NULL);
        assert(email != NULL);
        User user = getUserByEmail(api, email);
        if (!user) {
            return NULL;
        }
        return getUserMessages(user);
    }


    APIRes publishPost(API api, int userID, int postID, char *content) {
        assume(api != NULL);
        assume(content != NULL);
        assume(userID > 0);
        User user = getUserById(api, userID);
        assume(user != NULL);
        createPost(content, user, postID);
        return API_SUCCESS;

    }

    APIRes deletePost(API api, int userId, int postId) {
        assume(api != NULL);
        assume(userId > 0);
        assume(postId > 0);
        User user = getUserById(api, userId);
        assume(user != NULL);
        userDeletePost(user, postId);
        return API_SUCCESS;
    }

    APIRes addCommentOnPost(API api, char *commenterEmail, char *content, int postId, char *ownerEmail) {
        assert(api != NULL);
        assert(commenterEmail != NULL);
        assert(content != NULL);
        assert(postId > 0);
        assert(ownerEmail != NULL);
        User owner = getUserByEmail(api, ownerEmail);
        if (!owner) return API_USER_DOES_NOT_EXIST;
        if (!getPostById(owner, postId)) {
            return API_POST_DOES_NOT_EXIST;
        }
        User commenter = getUserByEmail(api, commenterEmail);
        if (!commenter) return API_USER_DOES_NOT_EXIST;
        int commenterId = getUserID(commenter);
        ListResult res = userAddCommentOnPost(owner, postId, commenterId, content);
        if (res == LIST_MEMORY_ERROR) {
            return API_MEMORY_ERROR;
        }
        assert(res != LIST_ALREADY_EXIST);
        return API_SUCCESS;
    }

    APIRes likePost(API api, char *likerEmail, int postId, char *ownerEmail) {
        assert(api != NULL);
        assert(likerEmail != NULL);
        assert(postId > 0);
        assert(ownerEmail != NULL);
        User owner = getUserByEmail(api, ownerEmail);
        if (!owner) return API_USER_DOES_NOT_EXIST;

        User liker = getUserByEmail(api, likerEmail);
        if (!liker) return API_USER_DOES_NOT_EXIST;
        int likerId = getUserID(liker);
        ListResult res = userLikePost(owner, postId, likerId);
        if (res == LIST_MEMORY_ERROR) {
            return API_MEMORY_ERROR;
        }
        if (res == LIST_NOT_EXIST) {
            return API_POST_DOES_NOT_EXIST;
        }
        return API_SUCCESS;
    }

    APIRes unLikePost(API api, char *likerEmail, int postId, char *ownerEmail) {
        assert(api != NULL);
        assert(likerEmail != NULL);
        assert(postId > 0);
        assert(ownerEmail != NULL);
        User owner = getUserByEmail(api, ownerEmail);
        if (!owner) return API_USER_DOES_NOT_EXIST;
        User liker = getUserByEmail(api, likerEmail);
        if (!liker) return API_USER_DOES_NOT_EXIST;
        int likerId = getUserID(liker);
        ListResult res = userUnlikePost(owner, postId, likerId);
        if (res == LIST_MEMORY_ERROR) {
            return API_MEMORY_ERROR;
        }
        if (res == LIST_NOT_EXIST) {
            return API_POST_DOES_NOT_EXIST;
        }
        if (res == LIST_ITEM2_DOES_NOT_EXIST) {
            return API_DID_NOT_LIKE_POST;
        }
        return API_SUCCESS;
    }

    int getLikersNum(API api, char *ownerEmail, int postId) {
        assert(api != NULL);
        assert(ownerEmail != NULL);
        assert(postId > 0);
        User owner = getUserByEmail(api, ownerEmail);
        if (!owner) {
            return -1;
        }
        return likersNum(owner, postId);
    }

    List getPostLikers(API api, char *email, int postId) {
        assert(api != NULL);
        assert(email != NULL);
        assert(postId > 0);
        User owner = getUserByEmail(api, email);
        if (!owner) {
            return NULL;
        }
        return getLikers(owner, postId);
    }

    List getPostComments(API api, char *email, int postId) {
        assert(api != NULL);
        assert(email != NULL);
        assert(postId > 0);
        User owner = getUserByEmail(api, email);
        if (!owner) {
            return NULL;
        }
        return getComments(owner, postId);
    }

    int getCommentsNum(API api, char *ownerEmail, int postId) {
        assert(api != NULL);
        assert(ownerEmail != NULL);
        assert(postId > 0);
        User owner = getUserByEmail(api, ownerEmail);
        if (!owner) {
            return -1;
        }
        return commentsNum(owner, postId);
    }

    List friendsOfUser(API api, char *email) {
        assert(api != NULL);
        assert(email != NULL);
        User user = getUserByEmail(api, email);
        if (!user) {
            return NULL;
        }
        return getUserFriends(user);
    }
