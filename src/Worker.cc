#include "Worker.h"
#include "Handler.h"

Worker::Worker() {
    this->state = PARSE;
}

Worker::~Worker() {}

/**
 * @details
 * if 解析请求：
 *      获取文件路径，设置报头信息，发送报头信息
 * else 发送数据：
 *      读取文件放到buff中
 * STATA: PARSE OPENFILE SEND
 */
void Worker::process() {
    switch (state) {
    case PARSE:
        /* 解析路径 */
        break;
    case OPENFILE:
        /* 打开文件 */
        /* 设置报头 */
        /* 发送报头 */
        /* 设置send_over为false */
        break;
    case SEND:
        /* 读取数据，调用Handler的write_data函数，直接放到bufferevent中，如果文件发送完，设置send_over为true */
        break;
    default:
        break;
    }
}

void Worker::set_handler(Handler* handler) {
    this->handler = handler;
}

void Worker::write_to_buff(char* data, size_t size) {
    memcpy(buff + buff_size, data, size);
    buff_size += size;
}