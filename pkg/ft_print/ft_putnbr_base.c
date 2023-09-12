//
// Created by millefeuille on 4/28/23.
//

#include "ft_print.h"

static void ft_set(unsigned long nb, int l, unsigned long bl, const char *b)
{
	int	i;
	char n[l];

	i = l - 1;
	while (nb >= bl) {
		n[i] = b[(nb % bl)];
		nb = nb / bl;
		i--;
	}
	n[0] = b[nb];
	i = 0;
	while (i != l) {
		write(1, &n[i], 1);
		i++;
	}
}

void ft_putnbr_base(unsigned long nb, char *base, size_t base_size)
{
	unsigned long nb2;
	size_t length;

	nb2 = nb;
	length = 1;
	while (nb >= base_size) {
		nb = nb / base_size;
		length++;
	}
	ft_set(nb2, (int)length, base_size, base);
}
