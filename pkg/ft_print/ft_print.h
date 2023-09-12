//
// Created by millefeuille on 4/21/23.
//

#ifndef FT_PRINT_H
# define FT_PRINT_H

# include <unistd.h>

void ft_putstr(const char *str);
void ft_fputstr(const char *str, int fd);

void ft_putchar(char c);
void ft_fputchar(char c, int fd);

void ft_putnbr(int n);
void ft_fputnbr(int n, int fd);

void ft_putnbr_base(unsigned long nb, char *base, size_t base_size);
void ft_putnbr_base_padded(unsigned long nb, char *base, size_t base_size, char pad_c, ssize_t pad_n);

#endif //FT_PRINT_H
