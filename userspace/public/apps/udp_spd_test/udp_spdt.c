/*
<:copyright-BRCM:2019:DUAL/GPL:standard

   Copyright (c) 2019 Broadcom 
   All Rights Reserved

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License, version 2, as published by
the Free Software Foundation (the "GPL").

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.


A copy of the GPL is available at http://www.broadcom.com/licenses/GPLv2.php, or by
writing to the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
Boston, MA 02111-1307, USA.

:> 
*/

#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <getopt.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/time.h>
#include <time.h>
#include <errno.h>
#include <fcntl.h>
#include <inttypes.h>

#include <spdt_api.h>

static struct option long_options[] =
{
    {"help", no_argument, 0, 'h' },
    {"recv", no_argument, 0,  'r' },
    {"send", no_argument, 0,  's' },
    {"ip", required_argument, 0, 'a' },
    {"port", required_argument, 0, 'p' },
    {"local_ip", optional_argument, 0, 'A' },
    {"local_port", optional_argument, 0, 'P' },
    {"time", optional_argument, 0, 't' },
    {"kbps", optional_argument, 0, 'k' },
    {"mbs", optional_argument, 0, 'm' },
    {"buflen", optional_argument, 0, 'l' },
    {"totalsize", optional_argument, 0, 'S' },
    {"interval", optional_argument, 0, 'i' },
    {"quiet_start", no_argument, 0, 'q' },
    {0, 0, 0, 0 }
};

int is_recv = -1, time_ms = 0, kbps = 0, buf_len = 0, interval = 0, interval_test_started = 0, quiet_start = 0;
int total_size = 0, msb = 0;
char ip_addr[NI_MAXHOST] = "";
uint16_t port;
char local_ip_addr[NI_MAXHOST] = "";
uint16_t local_port = 0;
spdt_conn_params_t conn_params = {};
spdt_rx_params_t rx_params = {};
spdt_tx_params_t tx_params = {};

uint8_t stream_idx = 0xff;
int is_running = 0, is_connected = 0;

#ifndef MAX
#define MAX(a, b) ((a) < (b) ? (b) : (a))
#endif
#ifndef MIN
#define MIN(a, b) ((a) >= (b) ? (b) : (a))
#endif

static void usage(char *prog)
{
    fprintf(stderr, "\nUDP Speed Test sample application Usage:\n\n");

    fprintf(stderr, "\t%s --recv --ip=<remote IP address> --port=<remote UDP port> "
        "[--local_ip=<local IP address> --local_port=<local UDP port>] [ --time=<msec> ] "
        "[ --interval=<sec> ] [--quiet_start ]\n", prog);
    fprintf(stderr, "\t\t time:      Optional. Test can be terminated by Ctrl+C at any time.\n");
    fprintf(stderr, "\t\t interval:  Optional. If set, results are polled and printed every interval period.\n\n");
    fprintf(stderr, "\t%s --send --ip=<remote IP address> --port=<remote UDP port> "
        "[--local_ip=<local IP address> --local_port=<local UDP port>] --kbps=<rate> "
        "--buflen=<data buffer length> [ --time=<msec> | --totalsize=<total number of bytes to send> ] "
        "[ --msb=<max burst size> ] [ --interval=<sec> ]\n\n", prog);
    fprintf(stderr, "\t\t kbps:        Rate (kilobits per second).\n");
    fprintf(stderr, "\t\t buflen:      UDP Data payload length. Length + UDP headroom cannot exceed MTU.\n");
    fprintf(stderr, "\t\t time:        Optional. Test can be terminated by Ctrl+C at any time.\n");
    fprintf(stderr, "\t\t totalsize:   Optional. If set, number of packets will be calculated according to (totalsize / buflen).\n");
    fprintf(stderr, "\t\t              Test will be terminated when all packets will be sent or by Ctrl+C.\n");
    fprintf(stderr, "\t\t msb:         Maximun burst size in packets. Optional. If not set, default of 2000 is used.\n");
    fprintf(stderr, "\t\t interval:    Optional. If set, results are polled and printed every interval period.\n");
    fprintf(stderr, "\t\t quiet_start: Optional. If set, periodic results are started to be printed only when sender starts to transmit.\n\n");
}

static int set_addr(char *ip_addr, uint16_t port, struct sockaddr_storage *addr)
{
    struct addrinfo *res = NULL, hints = {.ai_family = AF_UNSPEC, .ai_flags = AI_NUMERICHOST};
    int rc;

    rc = getaddrinfo(ip_addr, NULL, &hints, &res);
    if (rc)
    {
        fprintf(stderr, "%s: Failed to parse IP address from %s, error %d\n", __FUNCTION__, ip_addr, rc);
        return -1;
    }

    /* Set IP address */
    memset(addr, 0, sizeof(*addr));
    addr->ss_family = res->ai_family;
    memcpy(addr, res->ai_addr, res->ai_addrlen);
    freeaddrinfo(res);

    /* Set port */
    if (addr->ss_family == AF_INET)
        ((struct sockaddr_in *)addr)->sin_port = htons(port);
    else
        ((struct sockaddr_in6 *)addr)->sin6_port = htons(port);

    return 0;
}

static inline int sleep_ms(uint32_t ms)
{
    struct timespec sleep_ts = {};
    
    sleep_ts.tv_sec = ms / 1000;
    if (sleep_ts.tv_sec > 0)
        ms = ms % 1000;
    sleep_ts.tv_nsec = ms * 1000000L;
    return nanosleep(&sleep_ts, NULL);
}

#define GB 1000000000
#define MB 1000000
#define KB 1000

static void _dump_stats(char *prefix, uint64_t packets, uint64_t bytes, uint64_t usec)
{
    uint64_t rate = 0, sec;

    printf("Stats: %s packets %lld, %s bytes %lld, usec %lld, (Goodput) Rate = ",
        prefix, packets, prefix, bytes, usec);

    sec = usec / 1000000;
    if (sec)
        rate = bytes * 8 / sec;

    if (rate / GB > 0)
    {
        uint64_t rate_gb = rate / GB;
        uint64_t rate_mb = (rate - rate_gb * GB) / MB / KB;

        printf("%d.%dGbps (%lldbps)\n", (int)rate_gb, (int)rate_mb, rate);
    }
    else if (rate / MB > 0)
    {
        uint64_t rate_mb = rate / MB;
        uint64_t rate_kb = (rate - rate_mb * MB) / KB;

        printf("%d.%dMbps (%lldbps)\n", (int)rate_mb, (int)rate_kb, rate);
    }
    else if (rate / KB > 0)
    {
        uint64_t rate_kb = rate / KB;
        uint64_t rate_b = (rate - rate_kb * KB);

        printf("%d.%dMbps (%lldbps)\n", (int)rate_kb, (int)rate_b, rate);
    }
    else
        printf("%lldbps\n", rate);
}

static void dump_stats(spdt_stat_t *stats)
{
    if (is_recv)
    {
        _dump_stats("RX", stats->proto_ext.udp_basic.rx.packets,
            stats->proto_ext.udp_basic.rx.bytes, stats->proto_ext.udp_basic.rx_usec);
    }
    else
    {
        _dump_stats("TX", stats->proto_ext.udp_basic.tx.packets,
            stats->proto_ext.udp_basic.tx.bytes, stats->proto_ext.udp_basic.tx_usec);
    }
}

static void stream_close(void)
{
    spdt_stat_t stats = {};
    int rc;

    if (stream_idx == 0xff)
        return;

    if (is_connected)
    {
        if (is_running)
        {
            if (is_recv)
            {
                spdt_recv_stop(stream_idx);
                rc = spdt_stats_get(stream_idx, &stats);
                if (!rc)
                {
                    if (stats.proto_ext.udp_basic.rx.packets || !interval_test_started)
                        dump_stats(&stats);
                }
            }
            else
            {
                spdt_send_stop(stream_idx);
                rc = spdt_stats_get(stream_idx, &stats);
                if (!rc)
                {
                    if (stats.proto_ext.udp_basic.tx.packets || !interval_test_started)
                        dump_stats(&stats);
                }
            }
            is_running = 0;
        }
        spdt_disconnect(stream_idx);
        is_connected = 0;
    }
    spdt_uninit(stream_idx);
}

static void _dump_single_addr(struct sockaddr_storage *addr, char *title)
{
    char ip_addr[NI_MAXHOST] = {};
    int rc;

    rc = getnameinfo((struct sockaddr *)addr, sizeof(struct sockaddr_storage), ip_addr,
        NI_MAXHOST, NULL, 0, NI_NUMERICHOST);
    if (rc)
    {
        fprintf(stderr, "%s: Failed to retrieve server IP address, rc = %d (%s)\n", __FUNCTION__,
            rc, gai_strerror(rc));
    }
    printf("%s: %s : %d\n", title, ip_addr, addr->ss_family == AF_INET ?
        ntohs(((struct sockaddr_in *)addr)->sin_port) : ntohs(((struct sockaddr_in6 *)addr)->sin6_port));
}

static void dump_conn_params(spdt_conn_params_t *test_conn_params)
{
    _dump_single_addr(&test_conn_params->server_addr, "Server");
    _dump_single_addr(&test_conn_params->local_addr, "Local");
}

static void sig_handler(int sig_num)
{
    printf("Stop UDP Speed Test sample application, signal %d\n", sig_num);

    stream_close();
    exit(0);
}

static int sig_init(void)
{
    struct sigaction sa;
    int rc;

    sa.sa_handler = sig_handler;
    sa.sa_flags = SA_RESETHAND;

    rc = sigaction(SIGTERM, &sa, NULL);
    if(rc)
        return rc;

    rc = sigaction(SIGINT, &sa, NULL);
    if(rc)
        return rc;

    return rc;
}

void parse_args(int argc, char *argv[])
{
    int long_index = 0, opt;

    while ((opt = getopt_long_only(argc, argv, "", long_options, &long_index)) != -1)
    {
        switch (opt) {
        case 'r':
            is_recv = 1;
            break;
        case 's':
            is_recv = 0;
            break;
        case 'a':
            strncpy(ip_addr, optarg, NI_MAXHOST);
            break;
        case 'p':
            port = atoi(optarg); 
            break;
        case 'A':
            strncpy(local_ip_addr, optarg, NI_MAXHOST);
            break;
        case 'P':
            local_port = atoi(optarg); 
            break;
        case 't':
            time_ms = atoi(optarg);
            break;
        case 'k':
            kbps = atoi(optarg);
            break;
        case 'l':
            buf_len = atoi(optarg);
            break;
        case 'S':
            total_size = atoi(optarg);
            break;
        case 'm':
            msb = atoi(optarg);
            break;
        case 'i':
            interval = atoi(optarg);
            break;
        case 'q':
            quiet_start = 1;
            break;
        case 'h':
        default:
            usage(argv[0]); 
            exit(0);
        }
    }
    if (is_recv == -1)
    {
        fprintf(stderr, "Must specify send or recieve\n\n");
        goto exit;
    }
    if (!strlen(ip_addr))
    {
        fprintf(stderr, "Must set IP address for remote peer\n");
        goto exit;
    }
    if (!port)
    {
        fprintf(stderr, "Must set UDP port for remote peer\n");
        goto exit;
    }
    if (!is_recv)
    {
        if (!kbps)
        {
            fprintf(stderr, "Must specify transmit rate");
            goto exit;
        }
        tx_params.proto.udp.kbps = kbps;
        tx_params.proto.udp.data_buf_len = buf_len;
        tx_params.proto.udp.total_bytes_to_send = total_size; 
        tx_params.proto.udp.max_burst_size = msb; 
    }
    if (set_addr(ip_addr, port, &conn_params.server_addr))
        goto exit;
    if (local_port)
    {
        if (set_addr(local_ip_addr, local_port, &conn_params.local_addr))
            goto exit;
    }

    return;

exit:
    usage(argv[0]);
    exit(-1);
}

int main(int argc, char *argv[])
{
    int rc;
    spdt_conn_params_t test_conn_params;

    sig_init();

    parse_args(argc, argv);
    
    rc = spdt_init(SPDT_UDP_BASIC, &stream_idx);
    if (rc)
        goto exit;

    rc = spdt_connect(stream_idx, is_recv ? SPDT_DIR_RX : SPDT_DIR_TX, &conn_params);
    if (rc)
        goto exit;
    is_connected = 1;

    rc = spdt_get_conn_params(stream_idx, &test_conn_params);
    if (rc)
        goto exit;

    printf("Test Connection params are:\n");
    dump_conn_params(&test_conn_params);

    if (is_recv)
        rc = spdt_recv_start(stream_idx, &rx_params);
    else
        rc = spdt_send_start(stream_idx, &tx_params);
    if (rc)
        goto exit;
    is_running = 1;

    /* XXX: For transmit, need to implement statistics polling and see if still running */
    if (time_ms)
    {
        if (!interval)
        {
            /* If timer has been set, stop the test after it expires */
            sleep_ms(time_ms);
        }
        else
        {
            int period;
            spdt_stat_t stats;
            static int _interval = 0;

            while (time_ms && is_running)
            {
                period = MIN(1000, time_ms);
                sleep_ms(period);
                time_ms -= period;
                _interval++;
                if (_interval == interval)
                {
                    rc = spdt_stats_get(stream_idx, &stats);
                    if (!rc)
                        dump_stats(&stats);
                    _interval = 0;
                }
            }
        }
    }
    else
    {
        spdt_stat_t stats;
        static int _interval = 0;

        /* If time for waiting was not specified, terminate the test with Ctrl+C */
        while (is_running)
        {
            sleep_ms(1000);
            if (!interval)
                continue;
            _interval++;
            if (_interval == interval)
            {
                rc = spdt_stats_get(stream_idx, &stats);
                if (is_recv)
                {
                    if (!rc && stats.proto_ext.udp_basic.rx.packets)
                        interval_test_started = 1;
                    else if (interval_test_started)
                        break; /* No more statistics - we are done, stop the test */
                }
                else
                {
                    if (!rc && stats.proto_ext.udp_basic.tx.packets)
                        interval_test_started = 1;
                    else if (interval_test_started)
                        break; /* No more statistics - we are done, stop the test */
                }
                if (!rc && (interval_test_started || (is_recv && !quiet_start)))
                    dump_stats(&stats);
                _interval = 0;
            }
        }
    }

exit:
    if (stream_idx != 0xff)
        stream_close();
    return rc;
}

