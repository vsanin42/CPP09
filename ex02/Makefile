# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: vsanin <vsanin@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/03/19 14:07:47 by vsanin            #+#    #+#              #
#    Updated: 2025/03/19 14:07:47 by vsanin           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = PmergeMe

CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98
RM = rm -f

HEADERS = PmergeMe.hpp
SRC = main.cpp PmergeMe.cpp

OBJ = $(SRC:.cpp=.o)

all: $(NAME)

$(NAME): $(OBJ) $(HEADERS)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $(NAME)

%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

run1: $(NAME)
	@ARG="$(shell seq 0 1 | shuf -n 1 | tr '\n' ' ')"; \
	./$(NAME) $$ARG 

run2: $(NAME)
	@ARG="$(shell seq 0 10 | shuf -n 2 | tr '\n' ' ')"; \
	./$(NAME) $$ARG 

run3: $(NAME)
	@ARG="$(shell seq 0 10 | shuf -n 3 | tr '\n' ' ')"; \
	./$(NAME) $$ARG 

run5: $(NAME)
	@ARG="$(shell seq 0 20 | shuf -n 5 | tr '\n' ' ')"; \
	./$(NAME) $$ARG 

run10: $(NAME)
	@ARG="$(shell seq 0 200 | shuf -n 10 | tr '\n' ' ')"; \
	./$(NAME) $$ARG 

run21: $(NAME)
	@ARG="$(shell seq 0 20 | shuf -n 21 | tr '\n' ' ')"; \
	./$(NAME) $$ARG

run50: $(NAME)
	@ARG="$(shell seq 0 2000 | shuf -n 100 | tr '\n' ' ')"; \
	./$(NAME) $$ARG

run100: $(NAME)
	@ARG="$(shell seq 0 200000 | shuf -n 100 | tr '\n' ' ')"; \
	./$(NAME) $$ARG

run500: $(NAME)
	@ARG="$(shell seq 0 20000 | shuf -n 500 | tr '\n' ' ')"; \
	./$(NAME) $$ARG 

run1000: $(NAME)
	@ARG="$(shell seq 0 20000 | shuf -n 1000 | tr '\n' ' ')"; \
	./$(NAME) $$ARG

run3000: $(NAME)
	@ARG="$(shell seq 0 20000 | shuf -n 3000 | tr '\n' ' ')"; \
	./$(NAME) $$ARG

run6000: $(NAME)
	@ARG="$(shell seq 0 10000 | shuf -n 6000 | tr '\n' ' ')"; \
	./$(NAME) $$ARG

run10000: $(NAME)
	@ARG="$(shell seq 0 10000 | shuf -n 10000 | tr '\n' ' ')"; \
	./$(NAME) $$ARG

clean:
	$(RM) $(OBJ)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re run1 run2 run3 run5 run10 run21 run50 run100 run500 run1000 run3000 run6000 run10000
