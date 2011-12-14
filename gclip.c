/* http://idm.s9.xrea.com/ratio/2007/05/03/000614.html */

#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include <gtk/gtk.h>
struct buffer
{
    char *text;
    size_t len;
    size_t used;
};
static void
init_buffer(struct buffer* buffer)
{
    buffer->text = NULL;
    buffer->len = buffer->used = 0;
}
static void
free_buffer(struct buffer* buffer)
{
    free(buffer->text);
    init_buffer(buffer);
}
static int
extend_buffer(struct buffer* buffer, size_t required_len)
{
    char* new_buf;
    if (buffer->len - buffer->used >= required_len) return 1;
    new_buf = realloc(buffer->text, buffer->used + required_len);
    if (new_buf)
    {
      buffer->text = new_buf;
      buffer->len += required_len;
    }
}
static int
do_copy(const char* text, size_t len)
{
    GtkClipboard* clipboard;
    clipboard = gtk_clipboard_get(GDK_SELECTION_CLIPBOARD);
    if (!clipboard) return 0;
    gtk_clipboard_set_text(clipboard, text, len);
    gtk_clipboard_store(clipboard);
    return 1;
}
static int
do_read(FILE* fp, struct buffer* buffer)
{
    char tmp_buf[256];
    for (;;)
    {
        size_t read_len = fread(tmp_buf, sizeof(char), sizeof(tmp_buf), fp);
        if (read_len == 0)
        {
            break;
        }
        else if (read_len < 0)
        { 
            perror("STDIN");
            return 0;
        }
        else
        {
            if (!extend_buffer(buffer, read_len))
            {
                perror("");
                return 0;
            }
            memcpy(buffer->text + buffer->used,
                    tmp_buf, read_len);
            buffer->used += read_len;
        }
    }
    return 1;
}


int
main(int argc, char* argv[])
{
    struct buffer buffer;
    gtk_init(&argc, &argv);
    init_buffer(&buffer);
    if (!do_read(stdin, &buffer))
    {
        free_buffer(&buffer);
        return EXIT_FAILURE;
    }
    if (!do_copy(buffer.text, buffer.len))
    {
        free_buffer(&buffer);
        return EXIT_FAILURE;
    }
    free_buffer(&buffer);
    return EXIT_SUCCESS;
}

