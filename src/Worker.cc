#include "Worker.h"
#include "Handler.h"

Worker::Worker() {}

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
    if (/* 解析路径 */ false) {
        /* 1. 解析路径 */
    }
    if (/* 路径正确 */ false) {
        /* 2. 打开文件 */
        /* 3. 设置报头 */
        /* 4. 发送报头 */
    }
    if (/* 发送数据 */ false) {
        /* 5. 读取数据，直接放到bufferevent中，如果文件发送完，就什么也不干 */
    }

}

void Worker::set_handler(Handler* handler) {
    this->handler = handler;
}

void Worker::write_to_buff(char* data, size_t size) {
    memcpy(buff + buff_size, data, size);
}