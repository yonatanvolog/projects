all: app wd

app:
	@echo "Compiling app only into ./a.out"

	gcc app.c watcher.c wd_utils.c \
	dllist/dllist.c \
	sortedlist/sortedlist.c \
	pq/pq.c \
	task/task.c \
	uid/uid.c \
	schedule/scheduler.c \
	-ansi -pedantic-errors -Wall -Wextra -g -pthread

wd:
	@echo "Compiling wd only into ./wd.out"

	gcc -o wd.out wd.c wd_utils.c \
	dllist/dllist.c \
	sortedlist/sortedlist.c \
	pq/pq.c \
	task/task.c \
	uid/uid.c \
	schedule/scheduler.c \
	-ansi -pedantic-errors -Wall -Wextra -g -pthread