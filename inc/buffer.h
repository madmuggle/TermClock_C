#ifndef __BUFFER_H__
#define __BUFFER_H__

void initialize_contentbuf();
void update_contentbuf(int hour, int minute, int second);
void flush_contentbuf();

void update_windowbuf();
void flush_windowbuf();

#endif

