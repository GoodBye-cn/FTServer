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
    int ret;
    while (true) {
        switch (status) {
        case PARSE:
            /* 解析路径 */
            line_state = parse_request();
            if (line_state == LINE_BAD || line_state == LINE_OPEN) {
                return;
            }
            status = OPENFILE;
            break;
        case OPENFILE:
            /* 打开文件 */
            /* 设置报头 */
            /* 发送报头 */
            /* 设置send_over为false */
            open_file();
            status = SEND;
            break;
        case SEND:
            /* 读取数据，调用Handler的write_data函数，直接放到bufferevent中，如果文件发送完，设置send_over为true */
            ret = send_file();
            if (ret == 0) {
                status = PARSE;
                request.length = 0;
            }
            return;
        default:
            break;
        }
    }
}

void Worker::set_handler(Handler* handler) {
    this->handler = handler;
}

/**
 * @details 将数据写到buff中，如果状态为OPENFILE 或者 SEND 时写入数据，会破坏数据
 * 解决：添加状态判断，如果不是PARSE，就不写入
 */
void Worker::write_to_buff(char* data, size_t size) {
    memcpy(buff + buff_size, data, size);
    buff_size += size;
}

Worker::Line_Status Worker::parse_request() {
    if (buff_size > sizeof(int)) {
        if (request.length == 0) {
            memcpy(&request, buff, sizeof(int));
        }
        /* 请求读取完毕，可以获取路径 */
        if (request.length + sizeof(int) == buff_size) {
            memcpy(&request, buff, request.length + sizeof(int));
            request.path[request.length] = 0;
            path = request.path;
            return LINE_OK;
        }
        return LINE_OPEN;
    }
    return LINE_OPEN;
}

void Worker::open_file() {
    fd = open(path, O_RDONLY);
    if (fd == -1) {
        /* 路径错误 */
        response.size = -1;
        read_over = true;
    }
    else {
        read_over = false;
        stat(path, &file_stat);
        response.size = file_stat.st_size;
    }
    response.end[0] = '\r';
    response.end[1] = '\n';
    response.end[2] = '\r';
    response.end[3] = '\n';
    buff_size = sizeof(response);
    memcpy(buff, &response, buff_size);
    buff_wait_send = true;
    handler->set_send_over(false);
}

int Worker::send_file() {
    int ret = 0;
    while (true) {
        if (!buff_wait_send) {
            buff_size = read(fd, buff, sizeof(buff));
        }
        /* 文件读取完毕 */
        if (buff_size == 0) {
            read_over = true;
            close(fd);
        }
        ret = handler->write_data(buff, buff_size);
        if (ret == -1) {
            buff_wait_send = true;
            return -1;
        }
        buff_wait_send = false;
        if (!buff_wait_send && read_over) {
            handler->set_send_over(true);
            break;
        }
    }
    return 0;
}