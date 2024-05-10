#include <ifaddrs.h>

#include <cstring>
#include <cstdio>
#include <netdb.h>

#include "ZhenxiLog.h"
#include "adapter.h"
#include "version.h"
#include "getNetLinkerMac.h"
#include "stringUtils.h"
#include "appUtils.h"

struct sockaddr_ll {
    unsigned short sll_family;
    __be16 sll_protocol;
    int sll_ifindex;
    unsigned short sll_hatype;
    unsigned char sll_pkttype;
    unsigned char sll_halen;
    unsigned char sll_addr[8];
};
/**
 * 如果name传入null则不进行比较,只进行输出。
 */
bool getMacForNetLink(char *mac, char *name, int type) {
#if __ANDROID_API__ >= 24
    if (get_sdk_level() > ANDROID_N) {
        bool isFind = false;
        struct ifaddrs *ifap, *ifaptr;
        memset(mac, 0, strlen(mac));
        int ret = getifaddrs(&ifap);
        if (ret == 0) {
            for (ifaptr = ifap; ifaptr != nullptr; ifaptr = (ifaptr)->ifa_next) {
                if(ifaptr->ifa_addr == nullptr){
                    continue;
                }
                //获取数据包的类型
                sa_family_t family = ifaptr->ifa_addr->sa_family;
                if (type == ZHENXI_RUNTIME_NETLINK_MAC) {
                    if (family == AF_PACKET||family ==AF_INET) {
                        auto *sAddr = (struct sockaddr_ll *) (ifaptr->ifa_addr);
                        if (name == nullptr) {
                            int len = 0;
                            char macp[INET6_ADDRSTRLEN]={};
                            for (int i = 0; i < 6; i++) {
                                len += sprintf(macp + len, "%02X%s", sAddr->sll_addr[i],
                                               (i < 5 ? ":" : ""));
                            }
                            ALOGI ("V4->  [%s] -> [%s]  ",ifaptr->ifa_name, macp)
                            continue;
                        }
                        if (strcmp(ifaptr->ifa_name, name) == 0) {
                            int len = 0;
                            char macp[INET6_ADDRSTRLEN]={};
                            //ALOGI (" %s -> %s  ", ifaptr->ifa_name, macp)
                            for (int i = 0; i < 6; i++) {
                                len += sprintf(macp + len, "%02X%s", sAddr->sll_addr[i],
                                               (i < 5 ? ":" : ""));
                            }
                            if(StringUtils::equals(macp,DEFINE_MAC_1)||StringUtils::equals(macp,DEFINE_MAC_2)){
                                ALOGI ("get orig mac is def mac  %s -> %s  ", ifaptr->ifa_name, macp)
                                continue;
                            }
                            strcpy(mac, macp);
//                            ALOGI (">>>>>>>>>>>>  netlink get mac  sucess  [%s] -> [%s]  ",
//                                   ifaptr->ifa_name, macp)
                            isFind = true;
                            break;
                        }
                    }
                }
                else if (type == ZHENXI_RUNTIME_NETLINK_IP_V6) {
                    //AF_INET v4
                    //AF_INET6 v6
                    if (family == AF_INET6) {
                        const string &info = getpData(ifaptr->ifa_addr, 50);
                        //get v4 & v6 info
                        char host[NI_MAXHOST]={};
                        ret = getnameinfo(ifaptr->ifa_addr,
                                          (family == AF_INET) ? sizeof(struct sockaddr_in) :sizeof(struct sockaddr_in6),
                                          host, NI_MAXHOST,
                                          nullptr, 0, NI_NUMERICHOST);
                        if (ret != 0) {
                            LOGE("AF_INET6  getnameinfo() failed   %s  ", gai_strerror(ret))
                            return false;
                        }
                        if (name == nullptr) {
                            LOGE("V6-> [%s] [%s] [%s] ", (ifaptr)->ifa_name, host,info.c_str())
                            continue;
                        }
                        if (strcmp(ifaptr->ifa_name, name) == 0) {
                            strcpy(mac, host);
                            LOGE("AF_INET6 [%s] [%s]  ", (ifaptr)->ifa_name, host)
                            isFind = true;
                            break;
                        }
                    }
                }
                else if (type == ZHENXI_RUNTIME_NETLINK_NEIGH) {
                    char buffer[100];
                    FILE *fp = popen("ip neigh show", "r");
                    if (fp != nullptr) {
                        while (fgets(buffer, sizeof(buffer), fp) != nullptr) {
                            LOGE("ip neigh  -> %s ",buffer)
                        }
                        pclose(fp);
                    }
                }
                else if (type == ZHENXI_RUNTIME_ALL) {
                    if (family == AF_PACKET||family ==AF_INET) {
                        auto *sAddr = (struct sockaddr_ll *) (ifaptr->ifa_addr);
                        int len = 0;
                        char macp[INET6_ADDRSTRLEN];
                        for (int i = 0; i < 6; i++) {
                            len += sprintf(macp + len, "%02X%s", sAddr->sll_addr[i],
                                           (i < 5 ? ":" : ""));
                        }
                        LOGE("V4 %s %s  ", (ifaptr)->ifa_name, macp)
                    }
                    else if (family == AF_INET6) {
                        const string &info = getpData(ifaptr->ifa_addr, 50);
                        char host[NI_MAXHOST];
                        ret = getnameinfo(ifaptr->ifa_addr,
                                          (family == AF_INET) ? sizeof(struct sockaddr_in) :
                                          sizeof(struct sockaddr_in6),
                                          host, NI_MAXHOST,
                                          nullptr, 0, NI_NUMERICHOST);
                        if (ret != 0) {
                            LOGE("AF_INET6  getnameinfo() failed   %s  ", gai_strerror(ret))
                        }
                        LOGE("V6   [%s] [%s] [%s] ", (ifaptr)->ifa_name, host,info.c_str())
                    }
                }
            }
        }
        if(type == ZHENXI_RUNTIME_ALL){
            char buffer[100]={0};
            FILE *fp = popen("ip neigh show", "r");
            if (fp) {
                while (fgets(buffer, sizeof(buffer), fp) != nullptr) {
                    LOGE("ip neigh  -> %s ",buffer)
                }
                pclose(fp);
            }
        }
        if (name != nullptr) {
            ALOGI (">>>>>>>>>>>>  netlink not found %s , getifaddrs ret =  %d ",
                   name,
                   ret)
        }
        if(ifap!= nullptr){
            freeifaddrs(ifap);
        }
        return isFind;
    }
    return false;
#else
    return false;
#endif
}

void printNetlinkerInfo(int type) {
    char macp[INET6_ADDRSTRLEN];
    getMacForNetLink(macp, nullptr, type);
}