#include "minunit.h"
#include "lcthw/ringbuffer.h"
#include <assert.h>

#define BUFFER_SIZE 10

static RingBuffer *rbuff = NULL;
char test[] = {'a','b','v'};


char *test_create()
{
    rbuff = RingBuffer_create(BUFFER_SIZE);
    mu_assert(rbuff != NULL, "Failed to create ringbuff.");
    mu_assert(RingBuffer_empty(rbuff) , "Initiate error! The ringbuff should be empty.");

    //printf("Available space: %d\n",RingBuffer_available_space(rbuff));
        mu_assert(RingBuffer_available_space(rbuff) == BUFFER_SIZE, 
        "Incorrect available space after creation.");
    
    return NULL;
}

char *test_destroy()
{
    mu_assert(rbuff != NULL,"No ringbuff to destory!");
    RingBuffer_destroy(rbuff);

    return NULL;
}

char *test_write_read()
{

    bstring my_bstr = NULL;
    bstring bstr = bfromcstr("test1");
    RingBuffer_puts(rbuff,bstr);

    // Verify the available space after writing
    mu_assert(RingBuffer_available_space(rbuff) == BUFFER_SIZE - blength(bstr),
        "Incorrect available space after writing.");

    my_bstr = RingBuffer_get_all(rbuff);
    char *cstr = bstr2cstr(my_bstr, '\0');  // '\0' 表示以 null 结尾

    // 使用 printf 打印
    printf("%s\n", cstr);

     // Check the read content
    mu_assert(strcmp(cstr, "test1") == 0, "Data read from ringbuffer does not match.");

    // Verify the buffer is empty after reading
    mu_assert(RingBuffer_empty(rbuff), "Ringbuffer should be empty after reading all data.");

    bdestroy(bstr);
    bdestroy(my_bstr);

    
}

char *test_full_buffer()
{
    mu_assert(RingBuffer_empty(rbuff),"the rbuff is not be ready for full test!");
    bstring bstr = bfromcstr("0123456789");
    //bstring *bstr2 = bfromcstr("v");

    RingBuffer_puts(rbuff ,bstr);
    // printf("%d ,%d ,%d ,RingBuffer_available_data= %d",\
    // rbuff->start ,rbuff->end ,rbuff->length,RingBuffer_available_data(rbuff));
    mu_assert(RingBuffer_full(rbuff),"the buffer should be full." );

    //mu_assert((RingBuffer_puts(rbuff ,bstr) == -1 ,"It should be an error!");
    bstring extra_data = bfromcstr("extra");
    int extra_write = RingBuffer_puts(rbuff, extra_data);
    mu_assert(extra_write == -1, "Should not be able to write when buffer is full.");

    // Clean up
    bdestroy(bstr);
    bdestroy(extra_data);

    return NULL;

}

char *test_partial_read_write()
{
    // Read a portion of the data
    bstring read_data = RingBuffer_gets(rbuff, 3);
    mu_assert(blength(read_data) == 3, "Failed to read correct amount of data.");
    char *cstr = bstr2cstr(read_data, '\0');
    mu_assert(strcmp(cstr, "012") == 0, "Partial read data does not match.");

    // Buffer should still have data left
    mu_assert(!RingBuffer_empty(rbuff), "Buffer should not be empty after partial read.");
    mu_assert(RingBuffer_available_data(rbuff) == 2, "Incorrect available data after partial read.");

    // Clean up
    bdestroy(read_data);
    bcstrfree(cstr);

    return NULL;
}


char *all_tests() {
    mu_suite_start();

    mu_run_test(test_create);
    mu_run_test(test_write_read);
    mu_run_test(test_full_buffer);
    mu_run_test(test_partial_read_write);
    mu_run_test(test_destroy);
    

    return NULL;
}

RUN_TESTS(all_tests);