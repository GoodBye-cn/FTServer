#include "Worker.h"
#include "Handler.h"

Worker::Worker() {
    this->status = PARSE;
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
    Line_Status line_state = LINE_OK;
    while (true) {
        switch (status) {
        case PARSE:
            /* 解析路径 */
            line_state = parse_request();
            if (line_state == LINE_OK) {
                status = OPENFILE;
            }
            return;
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
}

void Worker::set_handler(Handler* handler) {
    this->handler = handler;
}

void Worker::write_to_buff(char* data, size_t size) {
    memcpy(buff + buff_size, data, size);
    buff_size += size;
}

Worker::Line_Status Worker::parse_request() {
    if (buff_size > sizeof(int)) {
        memcpy(&request, buff, sizeof(int));
        /* 请求读取完毕，可以获取路径 */
        if (request.length + sizeof(int) == buff_size) {
            path = request.path;
            return LINE_OK;
        }
        return LINE_OPEN;
    }
    return LINE_OPEN;
}