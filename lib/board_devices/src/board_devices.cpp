 #include <board_devices.hpp>

#include <unordered_map>
#include <chrono>
#include <thread>
#include <cmath>
#include <atomic>
#include <chrono>
#include <cstdio>
#include <optional>

#include <inttypes.h>

#include "Udp.hpp"
#include "lvgl.h"
#include "config.h"

#include <configmap.hpp>
struct field_t
{
    enum {TAG_PWM, TAG_TEMP, TAG_TIME};
    uint8_t tag;
    int64_t value;
};

struct board_devices_context_t
{
    bfc::UdpSocket sock;
    bfc::Ip4Port server;
    std::optional<int64_t> last_temp;
    std::optional<int64_t> last_time;
};

std::unique_ptr<board_devices_context_t> ctx;

static int64_t time_us()
{
    return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
}

void sendWithRepeat(field_t& f, field_t& fr, board_devices_context_t& ctx)
{
    bfc::BufferView buff((uint8_t*) &f, sizeof(f));
    bfc::BufferView buffr((uint8_t*) &fr, sizeof(fr));

    for (int i=0; i<3; i++)
    {
        ctx.sock.sendto(buff, ctx.server);
        bfc::Ip4Port from;
        auto rv = ctx.sock.recvfrom(buffr, from);
        if (rv <= 0 || f.tag != fr.tag)
        {
            fr = f;
            continue;
        }

        return;
    }
}

void initialize(board_devices_context_t& c)
{
    c.sock.bind(bfc::toIp4Port("0.0.0.0:11112"));

    configmap cm;
    cm.load(FS_PREFIX "/config.cfg");
    auto host  = cm.at_or("SLV_Host","127.0.0.1").raw();
    auto port  = cm.at_or("SLV_Port","11111").raw();
    auto host_port = host + ":" + (port);
    // auto host_port = "192.168.254.181:11111";
    c.server = bfc::toIp4Port(host_port);

    printf("%"PRId64"|board_devices| Slave device host[]=%s\n", time_us(), host.c_str());
    printf("%"PRId64"|board_devices| Slave device port[]=%s\n", time_us(), port.c_str());

    struct timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = 1000;
    if (setsockopt(c.sock.handle(), SOL_SOCKET, SO_RCVTIMEO,&tv,sizeof(tv)) < 0)
    {
        printf("%"PRId64"|board_devices| Can't set socket timeout!", time_us());
    }
}

board_devices::board_devices()
{
    ctx = std::make_unique<board_devices_context_t>();
    initialize(*ctx);
}

void board_devices::set_peltier_duty_100(float d)
{
    field_t fr;
    field_t f;
    f.tag = field_t::TAG_PWM;
    f.value = d*100;
    sendWithRepeat(f, fr, *ctx);
}

float board_devices::get_sensor_temp()
{
    field_t fr;
    field_t f;
    f.tag = field_t::TAG_TEMP;
    f.value = ctx->last_temp.value_or(-27315);
    sendWithRepeat(f, fr, *ctx);
    ctx->last_temp = fr.value;
    return float(fr.value)/100;
}

int64_t board_devices::time_us()
{
    return ::time_us();
    field_t fr;
    field_t f;
    f.tag = field_t::TAG_TIME;
    f.value = ctx->last_time.value_or(0);
    sendWithRepeat(f, fr, *ctx);
    ctx->last_time = ::time_us();
    return fr.value;
}

void board_devices::loop()
{
}