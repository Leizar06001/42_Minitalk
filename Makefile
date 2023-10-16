CC = gcc

CCFLAGS = -Werror -Wextra -Wall

SRCS = ft_atoi.c
SRCSCLIENT = client.c
SRCSSRV = server.c

OBJS = $(SRCS:%.c=%.o)

NAMECLIENT = client

NAMESRV = server

DEPS = minitalk.h

RM = rm -f

all: $(NAMESRV) $(NAMECLIENT)

%.o:%.c
	$(CC) -c $< -o $@ $(CCFLAGS)

$(NAMESRV): $(SRCSSRV) $(OBJS) $(DEPS)
	$(CC) $(SRCSSRV) $(OBJS) -o $(NAMESRV) $(CCFLAGS)

$(NAMECLIENT): $(SRCSCLIENT) $(OBJS) $(DEPS)
	$(CC) $(SRCSCLIENT) $(OBJS) -o $(NAMECLIENT) $(CCFLAGS)

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAMESRV) $(NAMECLIENT)

re: fclean all
