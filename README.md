# آزمایش ٧ - آشنایی با ریسه‌ها

- گروه شماره ۲۵
  - معین آعلی - ۴۰۱۱۰۵۵۶۱
  - ثمین اکبری - ۴۰۱۱۰۵۵۹۴

## آشنایی اولیه با ریسه‌ها

ابتدا کد داده شده را اجرا می‌کنیم و خروجی آن را مشاهده می‌کنیم:

![](/figs/image.png)

حال کد زیر را پیاده‌سازی می‌کنیم تا به جای تابع 
`sleep`
از `join`
استفاده کنیم تا منتظر پایان یافتن ترد فرزند باشد. همچنین `id`
تردها را چاپ می‌کنیم:

```C
#include <sys/types.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <stdio.h>
#include <pthread.h>

void* func(void* args){
    pid_t id = pthread_self();
    printf("child id:%d\n", id);
    return NULL;
}

int main() {
    pthread_t thread;
    pid_t id = pthread_self();
    printf("main id: %d\n", id);
    pthread_create(&thread, NULL, func, NULL);
    pthread_join(thread, NULL);
    return 0;
}
```

خروجی:

![](/figs/image-1.png)

همانطور که انتظار داشتیم `pthread_self` برای هر ترد آیدی متفاوت را چاپ کرده است.


در ادامه یک متغیر `global`
تعریف می‌کنیم و به آن در هر دو ترد دسترسی پیدا می‌کنیم و آن را تغییر می‌دهیم:


```C
#include <sys/types.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <stdio.h>
#include <pthread.h>

int oslab;

void* func(void* args){
    printf("before in kid:%d\n", oslab);
    oslab=2;
    printf("after in kid:%d\n", oslab);
    return NULL;
}   

int main() {
    oslab=1;
    pthread_t thread;
    printf("before in main:%d\n", oslab);
    pthread_create(&thread, NULL, func, NULL);
    pthread_join(thread, NULL);
    printf("after in main:%d\n", oslab);
    return 0;
}
```

نتیجه اجرا:

![](/figs/image-2.png)

پس نتیجه می‌گیریم که هر دوی تردها به یک متغیر در یک مموری یکسان دسترسی دارند.

حال در این بخش می‌خواهیم با اتریبیوت‌های ورودی ترد‌ها کار کنیم. پس کد زیر را پیاده‌سازی می‌کنیم:

```C
#include <sys/types.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <stdio.h>
#include <pthread.h>

void* func(void* args) {
    int num = (int)(size_t) args;
    int javab = 0;
    for (int i = 2; i < num + 1; i++) {
        javab = javab + i;
    }
    printf("sum: %d\n", javab);
    return NULL;
}

int main() {
    int n;
    scanf("%d", &n);
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_t thread;
    pthread_create(&thread, &attr, func, (void *)(size_t) n);
    pthread_join(thread, NULL);
    return 0;
}
```

نتیجه‌ی اجرا:

![](/figs/image-3.png)

ورودی `n=10`
را می‌دهیم و حاصل جمع ۲ تا ۱۰ را داخل ترد فرزند محاسبه می‌کنیم و خروجی را چاپ می‌کنیم.


## ریسه‌های چندتایی

با استفاده از تابع `pthread_create` تعدادی ریسه به تعداد دلخواه ایجاد می‌کنیم و پیام `Hello World` را در آن چاپ می‌کنیم. سپس ریسه‌ها را با استفاده از تابع `pthread_exit` خاتمه می‌دهیم.

```C
#include <sys/types.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <stdio.h>
#include <pthread.h>

void* func(void* args) {
    printf("Hello World!\n");
    pthread_exit(0);
}

int main() {
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    int n;
    scanf("%d", &n);
    pthread_t threads[n];
    
    for (size_t i = 0; i < n; i++) {
        pthread_create(&threads[i], &attr, func, NULL);
    }

    for (size_t i = 0; i < n; i++) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}
```

نتیجه‌ی اجرا:

![](/figs/image-4.png)

## تفاوت بین پردازه‌ها و ریسه‌ها


در این قسمت، قصد آن را داریم تا تفاوت میان پردازه‌ها و ریسه‌ها را بهتر متوجه شویم.

دو ریسه‌ی فرزند با تابع `child` ایجاد و اجرا می‌کنیم. در پایان ریسه‌ها نیز مقدار متغیر `global_var` را چاپ می‌کنیم:

```C
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

int global_var = 0; 

void *child(void *arg) {
    int local_var;

    printf("Thread %ld, pid %d, addresses: &global: %p, &local: %p \n",
        pthread_self(), getpid(), &global_var, &local_var);

    global_var++;

    printf("Thread %ld, pid %d, incremented global var=%d\n",
        pthread_self(), getpid(), global_var);

    pthread_exit(0);
}

int main(void) {
    pthread_t t1, t2;

    global_var = 0;

    if (pthread_create(&t1, NULL, child, NULL) != 0) {
        perror("pthread_create t1");
        return 1;
    }
    
    if (pthread_create(&t2, NULL, child, NULL) != 0) {
        perror("pthread_create t2");
        return 1;
    }

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    printf("Main thread: final global_var=%d\n", global_var);
    return 0;
}
```

نتیجه‌ی اجرا:

![](/figs/image-5.png)

همانطور که مشخص است، مقدار متغیر `global`
تغییر کرده است و مقدار آن درست است.

در این بخش علاوه بر ساخت ترد، یک `fork` هم روی پردازه انجام می‌دهیم و نتایج را چاپ می‌کنیم:


```C
#include <sys/types.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/wait.h>
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

int global_param;

void *child(void *param) {
    int local_var;

    printf("Thread %ld, pid %d, addresses: &global: %p, &local: %p \n",
        pthread_self(), getpid(), &global_param, &local_var);

    global_param++;

    printf("Thread %ld, pid %d, incremented global param=%d\n",
        pthread_self(), getpid(), global_param);

    pthread_exit(0);
}

int main(){    
    global_param = 100;
    pthread_t threads[2];
    
    pthread_create(&threads[0], NULL, child, NULL);
    pthread_create(&threads[1], NULL, child, NULL);

    pthread_join(threads[0], NULL);
    pthread_join(threads[1], NULL);
    printf("the final global param : %d\n", global_param);
    
    global_param = 200;
    int local_param = 999;

    int k = fork();
    if (k == 0){
        printf("Child: local and global param before change: %d, %d\n", local_param, global_param);
        local_param = 111;
        global_param = 4321;
        printf("Child: local and global param after change: %d, %d\n", local_param, global_param);
    }
    else{
        printf("Parent: local and global param before change: %d, %d\n", local_param, global_param);
        wait(NULL);
        printf("Parent: local and global param after change: %d, %d\n", local_param, global_param);
    }
    return 0;
}
```


نتیجه:

![](/figs/image-6.png)

همانطور که در خروجی مشخص است، داخل `fork` فرزند با این که متغیر گلوبال و لوکال را تغییر داده‌ایم، اما بعد از آن همچنان در پردازه اصلی مقادیر آن‌ها ثابت هستند.

## پاس دادن متغیرها به ریسه‌ها

دو ریسه ایجاد می‌کنیم و به هر کدام یک `struct`
که شامل یک `id`
و یک `message`
است پاس می‌دهیم:

```C
#include <sys/types.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <stdio.h>
#include <pthread.h>

struct thdata {
    int thread_no;
    char message[100];
} stdata;

void* func(void* params) {
    struct thdata data = *((struct thdata*) params);
    printf("id:%d msg:%s\n", data.thread_no, data.message);
}

int main() {
    pthread_t threads[2];

    struct thdata data[2] = {
        {1, "thread 1"},
        {2, "thread 2"},
    };
    
    for (size_t i = 0; i < 2; i++) {
        pthread_create(&threads[i], NULL, func, data+i);
    }
    
    for (size_t i = 0; i < 2; i++) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}
```

نتیجه‌ی اجرا:

![](/figs/image-7.png)