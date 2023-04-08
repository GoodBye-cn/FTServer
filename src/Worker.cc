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
 */
void Worker::process() {

}

void Worker::set_handler(Handler* handler) {
    this->handler = handler;
}