#include "adns.h"
#include <sys/errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int test_dns(FILE *fp)
{
    adns_state ads;
    adns_initflags flags;
    flags = adns_if_nosigpipe | adns_if_noerrprint;
    adns_init(&ads, flags, NULL);

    adns_query que = NULL;
    
    char buf[128];
    FILE *p;
    p = fgets(buf, 128, fp);
    while(p){
        adns_submit(ads, host, (adns_rrtype) adns_r_aaaa, (adns_queryflags) 0, NULL, &que);
        p = fgets(buf, 128, fp);
    }

    int try_cnt = -1;
    int adns_cname = 0;


    while(try_cnt < 32){
        try_cnt += 1;
        printf("try_cnt: %d\n", try_cnt);

        adns_answer *ans;
        int res = adns_check(ads, &que, &ans, NULL);
        printf("res: %d\n", res);
        if(res == 0){
            if(ans->status == adns_s_prohibitedcname && ans->cname != NULL){
                printf("ans_s_prohibitedcname\n");
                //printf("ans->cname: %s\n", ans->cname);
                char cname[128];
                strncpy(cname, ans->cname, 127);
                cname[strlen(ans->cname)] = '\0';
                adns_query que = NULL;
                adns_submit(ads, cname, (adns_rrtype) adns_r_aaaa, (adns_queryflags) 0, NULL, &que);
                adns_cname = 1;
            }else{
                printf("not prohibited\n");
                if(adns_cname){
                    printf("adns_cname is not NULL\n");
                    if(ans->status == adns_s_ok){
                        printf("status == adns_s_ok\n");
                        char buf[INET6_ADDRSTRLEN];
                        inet_ntop(AF_INET6, &ans->rrs.addr->addr.inet6.sin6_addr, buf, sizeof(buf));
                        printf("ip: %s\n", buf);
                    }
                    else{
                        printf("status != adns_s_ok\n");
                        printf("no\n");
                    }
                }
                else{
                    printf("adns_cname is NULL\n");
                    if(ans->status == adns_s_ok){
                        printf("status == adns_s_ok\n");
                        char buf[INET6_ADDRSTRLEN];
                        inet_ntop(AF_INET6, ans->rrs.in6addr, buf, sizeof(buf));
                        printf("ip: %s\n", buf);
                    }
                    else{
                        printf("status != adns_s_ok\n");
                        printf("no\n");
                    }
                }
                adns_finish(ads);
                break;
            }
        }
        else if(res == ESRCH || res == EAGAIN) {
            sleep(1);
        }else{
            printf("host(%s) is err!\n",host);
        }
    }
    return 0;
}

int main(int argc, char *argv[]){
    /* 
    char buf[128];
    char *p;
    FILE *fp = fopen(argv[1], "r");
    if(!fp){
        perror("fopen");
        exit(1);
    }
    
    test_dns(fp)

    //p = fgets(buf, 128, fp);
    //while(p){
    //    printf("%s", buf);
    //    adns_submit(ads, buf, (adns_rrtype) adns_r_aaaa, (adns_queryflags) 0, NULL, &que);
    //    p = fgets(buf, 128, fp);
    //}
    //test_dns(buf, ads);

    fclose(fp);
    */

    // /* 
    char buf[128];

    while(1){
        scanf("%s",buf);
        if(strlen(buf) == 3 && strcmp(buf, "eof")) break;
        test_dns(buf, ads);
    }
    // */

    return 0;
}
