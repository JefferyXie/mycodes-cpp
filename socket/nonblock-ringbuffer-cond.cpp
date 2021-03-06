#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

//
// http://stackoverflow.com/questions/14703707/circular-ring-buffer-with-blocking-read-and-non-blocking-write
// Requirements for the ringbuffer:
// 1) non-blocked write (=overwrite oldest data)
// 2) blocked read if empty
// Solution: use phtread mutex + condition variable
//

#if 0

#define RINGBUFFER_SIZE (5)
char ringbuffer[RINGBUFFER_SIZE];
unsigned reader_unread = 0;
unsigned writer_next = 0;
pthread_mutex_t ringbuffer_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t ringbuffer_written_cond = PTHREAD_COND_INITIALIZER;

void process_code(char ch) {
    printf("Processing code %c", ch);
    for(int counter=2; counter>0; --counter) {
        putchar('.');
        fflush(stdout);
        sleep(1);
    }
    printf("done.\n");
}

void *reader(void*) {
    pthread_mutex_lock(&ringbuffer_mutex);
    for(;;) {
        if (reader_unread == 0) {
            pthread_cond_wait(&ringbuffer_written_cond, &ringbuffer_mutex);
        }
        if (reader_unread > 0) {

            int pos = writer_next - reader_unread;
            if (pos < 0) pos += RINGBUFFER_SIZE;
            char ch = ringbuffer[pos];
            --reader_unread;

            if (ch == '\n') continue;
            if (ch == EOF) break;

            pthread_mutex_unlock(&ringbuffer_mutex);
            process_code(ch);
            pthread_mutex_lock(&ringbuffer_mutex);
        }
    }
    pthread_mutex_unlock(&ringbuffer_mutex);

    puts("READER THREAD GOT EOF");
    return NULL;
}

void *writer(void*) {
    char ch;
    do {
        ch = getchar();
        pthread_mutex_lock(&ringbuffer_mutex);

        ringbuffer[writer_next] = ch;

        ++writer_next;
        if (writer_next == RINGBUFFER_SIZE) writer_next = 0;

        int overflow = 0;
        if (reader_unread < RINGBUFFER_SIZE) ++reader_unread;
        else overflow = 1;

        pthread_cond_signal(&ringbuffer_written_cond);
        pthread_mutex_unlock(&ringbuffer_mutex);

        if (overflow) puts("WARNING: OVERFLOW!");
    } while (ch != EOF);

    puts("WRITER THREAD GOT EOF");
    return NULL;
}

int main(void)
{
    pthread_t reader_thread, writer_thread;

    puts("Starting threads. Type text and press enter, or type ctrl-d at empty line to quit.");
    pthread_create(&reader_thread, NULL, reader, NULL);
    pthread_create(&writer_thread, NULL, writer, NULL);

    pthread_join(writer_thread, NULL);
    pthread_join(reader_thread, NULL);

    return 0;
}

#endif

