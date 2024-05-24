#include <libavformat/avformat.h> // 包含FFmpeg的格式IO库，用于处理多媒体容器格式
#include <libavutil/avutil.h>     // 包含FFmpeg的基本工具库，提供常用的数据结构和函数

int main(int argc, char **argv)
{
    // 检查是否提供了正确的命令行参数（视频文件路径）
    if (argc != 2)
    {
        printf("Usage: %s input_file\n", argv[0]);
        return -1;
    }

    const char *filename = argv[1]; // 获取输入的视频文件路径

    AVFormatContext *fmt_ctx = NULL; // 初始化AVFormatContext指针，用于存储文件格式上下文信息

    // 打开输入文件
    if (avformat_open_input(&fmt_ctx, filename, NULL, NULL) != 0)
    {
        printf("Could not open input file '%s'\n", filename);
        return -1;
    }

    // 获取媒体流信息，包括时长、编码格式等
    if (avformat_find_stream_info(fmt_ctx, NULL) < 0)
    {
        printf("Failed to retrieve stream info from file '%s'\n", filename);
        return -1;
    }

    // 计算并打印视频时长（秒）
    printf("File information for '%s':\n", filename);
    // 计算并打印视频时长（秒）
    printf("Duration: %.02ld seconds\n", (int64_t)(fmt_ctx->duration / AV_TIME_BASE));
    // 打印视频流数量
    printf("Video streams: %d\n", fmt_ctx->nb_streams);

    // 查找视频流
    AVStream *video_stream = NULL;
    for (int i = 0; i < fmt_ctx->nb_streams; i++)
    {
        if (fmt_ctx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) // 检查是否为视频流
        {
            video_stream = fmt_ctx->streams[i]; // 存储视频流信息
            break;
        }
    }

    // 如果找到了视频流，则打印分辨率
    if (video_stream)
    {
        printf("Video resolution: %dx%d\n", video_stream->codecpar->width, video_stream->codecpar->height);
    }
    else
    {
        printf("No video stream found.\n");
    }

    // 关闭输入文件并释放资源
    avformat_close_input(&fmt_ctx);
    return 0;
}

// gcc -o main main.c -I/usr/include/x86_64-linux-gnu/ -lavformat -lavcodec -lavutil -lswscale -L/usr/lib/x86_64-linux-gnu/