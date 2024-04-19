
#include <ifaddrs.h>
#include <linux/netlink.h>
#include <linux/rtnetlink.h>

#define DEFINE_MAC_2 "00:00:00:00:00:00"
#define DEFINE_MAC_1 "02:00:00:00:00:00"

/**
 * mac相关类型
 * 注意:
 * 在安卓高版本已经无法通过netlink获取到mac
 */
#define ZHENXI_RUNTIME_NETLINK_MAC 1
/**
 * ipv6
 */
#define ZHENXI_RUNTIME_NETLINK_IP_V6 2
/**
 * 相邻ip信息
 * 执行ip neigh show
 */
#define ZHENXI_RUNTIME_NETLINK_NEIGH 3

/**
 * 输出全部的信息
 */
#define ZHENXI_RUNTIME_ALL 9





/**
 * 获取原始值,需要在hook之前进行获取。
 *
 * 返回通过netlink获取mac和v4&v6相关
 *
 * ZHENXI_RUNTIME_NETLINK_MAC 网卡相关 。
 * ZHENXI_RUNTIME_NETLINK_IP_V6 v4v6相关
 *
 * @return 返回是否成功
 */
bool getMacForNetLink(char macp[INET6_ADDRSTRLEN],char* name,int type);

void printNetlinkerInfo(int type);

