//
// Created by d_dab on 06/06/2020.
//

#include <stdio.h>
#include "API.h"
#include <assert.h>
#include <seahorn/seahorn.h>
#include<stdint.h>
#include<stddef.h>
#include"string.h"
#include "helpers.h"
#include "list.h"

extern int nd();


char FN1[50] = "diana";
char LN1[50] = "dabbah";
char email1[5] = "abcde";
char pass1[50] = "1234";
int day1 = 1, month1 = 1, year1 = 1998;
char FN2[50] = "radi";
char LN2[50] = "kot";
char email2[5] = "radi1";
char pass2[50] = "12345";
int day2 = 6, month2 = 9, year2 = 1996;
char FN3[50] = "nadia";
char LN3[50] = "nodnod";
char email3[5] = "nadi1";
char pass3[50] = "123456";
int day3 = 2, month3 = 12, year3 = 1961;
char FN4[50] = "nadir";
char LN4[50] = "nodnor";
char email4[5] = "nadir";
char pass4[50] = "123456";
int day4 = 2, month4 = 12, year4 = 1961;


void VerificationAll() {

    API api = initSystem();
    sassert(api != NULL);
    int id1 = nd();
    assume(id1 > 0);

    User user1 = createUserInAPI(api, id1, FN1, LN1, email1, pass1, day1, month1, year1);
    assume(user1 != NULL);

    int id2 = nd();
    assume(id1 != id2);
    assume(id2 > 0);

    User user2 = createUserInAPI(api, id2, FN2, LN2, email2, pass2, day2, month2, year2);
    assume(user2 != NULL);

    int id3 = nd();
    assume(id3 != id2);
    assume(id3 != id1);
    assume(id3 > 0);
    User user3 = createUserInAPI(api, id3, FN3, LN3, email3, pass3, day3, month3, year3);
    assume(user3 != NULL);

  //  deleteUserFromAPI(api, id3);

    sendFriendRequest(api, id1, user2);
    handleFriendRequest(api, user2, id1, ACCEPT);
//    sendFriendRequest(api, id2 ,user1);
//    handleFriendRequest(api, user1, id2, DECLINE);
    int postID1 = nd();
    assume(postID1>0);
    int postID2 = nd();
    assume(postID2>0);
    assume (postID1 != postID2);
    int postID3 = nd();
    assume(postID3>0);
    assume (postID3 != postID2);
    assume (postID3 != postID1);
    publishPost(api,id1,postID1,"hello");
    publishPost(api,id2,postID2,"bye");
    deletePost(api,id1,postID1);
    deletePost(api,id2,postID2);
    publishPost(api,id1,postID3,"b3b3");
}

void AddUserVerification(){
    API api = initSystem();
    sassert(api != NULL);
    int id1 = nd();
    assume(id1 > 0);

    User user1 = createUserInAPI(api, id1, FN1, LN1, email1, pass1, day1, month1, year1);
    assume(user1 != NULL);
//    int res1 = ApiHasUser(api, id1);
//    for (int i = 0; i < api->usersArraySize; ++i) {
//        if (id1 == api->usersGhosts[i]->id) {
//            sassert(res1);
//        }
//    }

    int id2 = nd();
    assume(id1 != id2);
    assume(id2 > 0);
//    bool res2 = ApiHasUser(api, id2);
//    for (int i = 0; i < api->usersArraySize; ++i) {
//        if (id2 == api->ghost[i].val) {
//            sassert(res2 == 0);
//        }
//    }


    User user2 = createUserInAPI(api, id2, FN2, LN2, email2, pass2, day2, month2, year2);
    assume(user2 != NULL);
    //bool res4 = ApiHasUser(api, id2);
//    for (int i = 0; i < api->usersArraySize; ++i) {
//        if (id2 == api->ghost[i].val) {
//            sassert(res4);
//        }
//    }
//
}

void AddAndDeleteUserVerfication(){
    API api = initSystem();
    sassert(api != NULL);
    int id1 = nd();
    assume(id1 > 0);

    User user1 = createUserInAPI(api, id1, FN1, LN1, email1, pass1, day1, month1, year1);
    assume(user1 != NULL);


    int id2 = nd();
    assume(id1 != id2);
    assume(id2 > 0);
    User user2 = createUserInAPI(api, id2, FN2, LN2, email2, pass2, day2, month2, year2);
    assume(user2 != NULL);
    deleteUserFromAPI(api, id1);

}

void sendAndHandleFriendRequest(){
    API api = initSystem();
    sassert(api != NULL);
    int id1 = nd();
    assume(id1 > 0);

    User user1 = createUserInAPI(api, id1, FN1, LN1, email1, pass1, day1, month1, year1);
    assume(user1 != NULL);

    int id2 = nd();
    assume(id1 != id2);
    assume(id2 > 0);

    User user2 = createUserInAPI(api, id2, FN2, LN2, email2, pass2, day2, month2, year2);
    assume(user2 != NULL);

    sendFriendRequest(api, id1, user2);
    handleFriendRequest(api, user2, id1, ACCEPT);

}

void AddPost(){
    API api = initSystem();
    sassert(api != NULL);
    int id1 = nd();
    assume(id1 > 0);

    User user1 = createUserInAPI(api, id1, FN1, LN1, email1, pass1, day1, month1, year1);
    assume(user1 != NULL);

    int id2 = nd();
    assume(id1 != id2);
    assume(id2 > 0);

    User user2 = createUserInAPI(api, id2, FN2, LN2, email2, pass2, day2, month2, year2);
    assume(user2 != NULL);
    int postID1 = nd();
    assume(postID1>0);
    int postID2 = nd();
    assume(postID2>0);
    assume (postID1 != postID2);
    int postID3 = nd();
    assume(postID3>0);
    assume (postID3 != postID2);
    assume (postID3 != postID1);
    publishPost(api,id1,postID1,"hello");
    publishPost(api,id2,postID2,"bye");
    deletePost(api,id1,postID1);
    deletePost(api,id2,postID2);
    publishPost(api,id1,postID3,"b3b3");
}

int main(int argc, char *argv[]) {


     AddUserVerification();
//    AddAndDeleteUserVerfication();
//    sendAndHandleFriendRequest();
//    AddPost();
  //  VerificationAll();
//    printf("Test Passed! GOOD JOB \n");

}