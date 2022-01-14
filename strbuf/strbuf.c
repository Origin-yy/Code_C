#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "strbuf.h"

//Part 2A
void strbuf_init(struct strbuf *sb, size_t alloc);
//初始化 sb 结构体，容量为 alloc。
void strbuf_attach(struct strbuf *sb, void *str, size_t len, size_t alloc);
//将字符串填充到 sb 中，长度为 len, 容量为 alloc。
void strbuf_release(struct strbuf *sb);
//释放 sb 结构体的内存。
void strbuf_swap(struct strbuf *a, struct strbuf *b);
//交换两个 strbuf。
char *strbuf_detach(struct strbuf *sb, size_t *sz);
//将 sb 中的原始内存取出，并获得其长度。
int strbuf_cmp(const struct strbuf *first, const struct strbuf *second);
//比较两个 strbuf 的内存是否相同
void strbuf_reset(struct strbuf *sb);
//清空 sb。

//Part 2B

void strbuf_grow(struct strbuf *sb, size_t extra);
//将 sb 的长度扩大 extra。
void strbuf_add(struct strbuf *sb, const void *data, size_t len);
//向 sb 追加长度为 len 的数据 data。
void strbuf_addch(struct strbuf *sb, int c);
//向 sb 追加一个字符 c。
void strbuf_addstr(struct strbuf *sb, const char *s);
//向 sb 追加一个字符串 s。
void strbuf_addbuf(struct strbuf *sb, const struct strbuf *sb2);
//向一个 sb 追加另一个 strbuf的数据。
void strbuf_setlen(struct strbuf *sb, size_t len);
//设置 sb 的长度 len。
size_t strbuf_avail(const struct strbuf *sb);
//计算 sb 目前仍可以向后追加的字符串长度。
void strbuf_insert(struct strbuf *sb, size_t pos, const void *data, size_t len);
//向 sb 内存坐标为 pos 位置插入长度为 len 的数据 data。

//Part 2C

void strbuf_ltrim(struct strbuf *sb);
//去除 sb 缓冲区左端的所有 空格，tab, '\t'。
void strbuf_rtrim(struct strbuf *sb);
//去除 sb 缓冲区右端的所有 空格，tab, '\t'。
void strbuf_remove(struct strbuf *sb, size_t pos, size_t len);
//删除 sb 缓冲区从 pos 坐标长度为 len 的内容。

//Part 2D

ssize_t strbuf_read(struct strbuf *sb, int fd, size_t hint);
//sb 增长 hint ? hint : 8192 大小， 然后将文件描述符为 fd 的所有文件内容追加到 sb 中。
int strbuf_getline(struct strbuf *sb, FILE *fp);
//将 将文件句柄为 fp 的一行内容读取到 sb 。

//Paret 2A

void strbuf_init(struct strbuf *sb, size_t alloc)
{
    sb->len=0;
    sb->alloc=alloc;
    sb->buf = (char *)malloc(alloc*sizeof(char));
	if(sb->buf == NULL)
    {
		printf("malloc memory unsuccessful.");
		exit(1);
	}
}

void strbuf_attach(struct strbuf*sb,void*str,size_t len,size_t alloc)
{
    strbuf_init(sb,alloc);
    while(sb->alloc < len)
    {
        sb->alloc *=2;
        sb->buf = (char *)realloc(sb->buf,2*sb->alloc);
        if(sb->buf == NULL)
        {
            printf("realloc memory unsuccessful.");
            exit(1);
        }
    }
    sb->len = len;
    sb->buf = (char *)str;
}

void strbuf_release(struct strbuf *sb)
{
    if(sb == NULL)
        return;
        free(sb->buf);
    //free(sb);
}

void strbuf_swap(struct strbuf *a, struct strbuf *b)
{
    struct strbuf temp;

    temp.alloc = a->alloc;
    temp.len = a->len;
    temp.buf = a->buf;

    a->alloc = b->alloc;
    a->len = b->len;
    a->buf = b->buf;

    b->alloc = temp.alloc;
    b->len = temp.len;
    b->buf = temp.buf;
}

char *strbuf_detach(struct strbuf *sb, size_t *sz) 
{
    *sz = sb->alloc;
    return sb->buf;
}

int strbuf_cmp(const struct strbuf *first, const struct strbuf *second)
{
    return first != second && first->len != second->len;
}

void strbuf_reset(struct strbuf *sb)
{
    if (sb == NULL) return;
    strbuf_init(sb,sb->alloc);
}

//Part 2B

void strbuf_grow(struct strbuf *sb, size_t extra)
{
    if(extra == 0) return;
    sb->buf = (char*)realloc(sb->buf,(sb->alloc+extra)*sizeof(char));
    sb->alloc += extra;
}

void strbuf_add(struct strbuf *sb, const void *data, size_t len)
{
    if(sb->len + len > sb->alloc)
        strbuf_grow(sb,len);
    memmove(sb->buf + sb->len,data,len);
    sb->len += len;
    sb->buf[sb->len] = '\0';
}

void strbuf_addch(struct strbuf *sb, int c)
{
    if (sb == NULL) return;
    if(sb->len + 2>sb->alloc)
        strbuf_grow(sb,2);
    memmove(sb->buf + sb->len,&c,2);
    sb->len++;
    sb->buf[sb->len] = '\0';
}

void strbuf_addstr(struct strbuf *sb, const char *s)
{
    int len = strlen(s);
    if(sb->len + len +1 > sb->alloc)
        strbuf_grow(sb,len + 1);
    memmove(sb->buf + sb->len,s,len);
    sb->len += len;
    sb->buf[sb->len] = '\0';
}

void strbuf_addbuf(struct strbuf *sb, const struct strbuf *sb2)
{
    strbuf_addstr(sb,sb2->buf);
}

void strbuf_setlen(struct strbuf *sb, size_t len)
{
    int stop = '\0';
    memmove(sb->buf + len,&stop,1);
    sb->len = len;
}

size_t strbuf_avail(const struct strbuf *sb)
{
    return sb->alloc-sb->len-1;
}

void strbuf_insert(struct strbuf *sb, size_t pos, const void *data, size_t len)
{
    sb->len += len;
    int last_len = sb->len - pos - len;
    char *temp = (char *)malloc(sizeof(char)*last_len);
    memmove(temp,sb->buf + pos,last_len);
    memmove(sb->buf + pos,data,len);
    memmove(sb->buf + pos + len,temp,last_len);
}

//Part 2C

void strbuf_ltrim(struct strbuf *sb)
{
    int i = 0; 
    while(sb->buf[i] == ' ' || sb->buf[i] == '\t')
        ++i;
    strncpy(sb->buf, sb->buf + i , strlen(sb->buf) - i);
    sb->buf[strlen(sb->buf) - i + 1] = '\0';
    sb->len -=i;
}

void strbuf_rtrim(struct strbuf *sb)
{
    int i;
        i = strlen(sb->buf) - 1;
 
        while(sb->buf[i] == ' ' || sb->buf[i] == '\t')
            --i;
        strncpy(sb->buf, sb->buf, i + 1);
        sb->buf[i + 1] = '\0';
        sb->len = i +1;
}

void strbuf_remove(struct strbuf *sb, size_t pos, size_t len)
{
    sb->len -= len;
    int last_len = sb->len - pos;
    char *temp = (char *)malloc(sizeof(char)*last_len);
    memmove(temp,sb->buf + pos + len,last_len);
    memmove(sb->buf + pos,temp,last_len);
    sb->buf[sb->len + 1] = '\0';
}

//Part 2D
ssize_t strbuf_read(struct strbuf *sb, int fd, size_t hint)
{
    return 1;
}
int strbuf_getline(struct strbuf *sb, FILE *fp)
{
    return 1;
}

//CHALLENGE

struct strbuf **strbuf_split_buf(const char *str, size_t len, int terminator, int max)
{
    return NULL;
}

bool strbuf_begin_judge(char* target_str, const char* str, int strlen)
{
    return 0;
}
char* strbuf_get_mid_buf(char* target_buf, int begin, int end, int len)
{
    return NULL;
}