//
// Created by jgiron on 12/4/21.
//

#include <cstring>
#include <iostream>
#include <cstdio>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <deque>
#include <errno.h>
#ifndef BUFFER_SIZE
#define BUFFER_SIZE 1
#endif
#ifndef BEFORE_SIZE
# define BEFORE_SIZE -1
#endif
bool stop = false;

void handler(int)
{
	stop = true;
}

int main(int argc, char **argv)
{
	char buf1[BUFFER_SIZE + 1];
	char buf2[BUFFER_SIZE + 1];
	int fd1, fd2;
	int ret1 = 1, ret2 = 1;
	std::deque<char> d1, d2;
	std::deque<char> before;
	int			pos = 0;

	signal(SIGTERM, &handler);
	buf1[BUFFER_SIZE] = 0;
	buf2[BUFFER_SIZE] = 0;
	if (argc == 3)
	{
		if ((fd1 = open(argv[1], O_RDONLY | O_NONBLOCK)) == -1 || (fd2 = open(argv[2], O_RDONLY | O_NONBLOCK)) == -1)
		{
			perror("open");
			return (1);
		}
		std::cout << "\033[32m";
		while ( !(ret1 <= 0 && ret2 <= 0 && stop))
		{
			ret1 = read(fd1, buf1, BUFFER_SIZE);
			if (ret1 == -1 && errno != EAGAIN)
			{
				perror("fifodiff");
				return 1;
			}
			ret2 = read(fd2, buf2, BUFFER_SIZE);
			if (ret2 == -1 && errno != EAGAIN)
			{
				perror("fifodiff");
				return 1;
			}
//			std::cout << ret1 << " " << ret2 << std::endl;
			for (int i = 0; i < ret1; i++)
					d1.push_back(buf1[i]);
			for (int i = 0; i < ret2; i++)
					d2.push_back(buf2[i]);
			while (!d2.empty() && !d1.empty()) {
				if (d1.front() == d2.front()) {
					if (d1.front() == 3) {
						std::cout << std::endl << "\033[0m🐒 no diff detected" << std::endl;
						return (0);
					}
#if BEFORE_SIZE > 0
						before.push_back(d1.front());
						if (before.size() > BEFORE_SIZE)
							before.pop_front();
#else
						std::cout << d1.front();
#endif
					d1.pop_front();
					d2.pop_front();
					pos++;
				}
				else
				{
					if ((d2.size() >= 100 && d1.size() >= 100)) {
						std::cout << std::endl << "\033[0m🐒 files differ!!! (pos: " << pos << ")" << std::endl;
#if BEFORE_SIZE > 0
							std::cout << "context:" << std::endl;
							for (std::deque<char>::size_type i = 0; i < BEFORE_SIZE && i < before.size(); i++)
								std::cout << before[i];
							std::cout << std::endl;
							std::cout << "===================================================== ";
#endif
						std::cout << "\033[0m" << argv[1] << ":" << std::endl;
						std::cout << "\033[31m";
						for (std::deque<char>::size_type i = 0; i < 200 && i < d1.size(); i++)
							std::cout << d1[i];
						std::cout << std::endl;
						std::cout << "\033[0m";
						std::cout << argv[2] << ":" << std::endl;
						std::cout << "\033[31m";
						for (std::deque<char>::size_type i = 0; i < 200 && i < d1.size(); i++)
							std::cout << d2[i];
						std::cout << std::endl;
						return (1);
					}
					break;
				}
			}
		}
		if (ret1 != ret2)
			return (1);
	}
}
