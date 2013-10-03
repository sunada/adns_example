#include "adns.h"
#include <sys/errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int test_dns(char *host)
{
    adns_state ads;
    adns_initflags flags;
    flags = adns_if_nosigpipe | adns_if_noerrprint;
    adns_init(&ads, flags, NULL);

    adns_query que = NULL;
    adns_submit(ads, host, (adns_rrtype) adns_r_a, (adns_queryflags) 0, NULL, &que);

    int try_cnt = -1;
    int adns_cname = 0;

    while(try_cnt < 32){
        try_cnt += 1;

        adns_answer *ans;
        int res = adns_check(ads, &que, &ans, NULL);
        if(res == 0){
            if(ans->status == adns_s_prohibitedcname && ans->cname != NULL){
                //printf("ans->cname: %s\n", ans->cname);
                char cname[128];
                strncpy(cname, ans->cname, 127);
                cname[strlen(ans->cname)] = '\0';
                adns_query que = NULL;
                adns_submit(ads, cname, (adns_rrtype) adns_r_addr, (adns_queryflags) 0, NULL, &que);
                adns_cname = 1;
            }else{
                if(adns_cname)
                    printf("ip: %s\n", ans->status == adns_s_ok ? inet_ntoa(ans->rrs.addr->addr.inet.sin_addr) : "no");
                else
                    printf("ip: %s\n", ans->status == adns_s_ok ? inet_ntoa(*(ans->rrs.inaddr)) : "no");
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
    char host[128];
    while(1){
        scanf("%s",host);
        if(strlen(host) == 3 && strcmp(host, "eof")) break;
        test_dns(host);
    }
    return 0;
}
