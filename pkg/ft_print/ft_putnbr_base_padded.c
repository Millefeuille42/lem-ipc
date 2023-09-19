//
// Created by millefeuille on 6/26/23.
//

#include "ft_print.h"

static void ft_set(unsigned long nb, long l, unsigned long bl, const char *b)
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

static void ft_pad(size_t len, size_t pad_len, char pad_c) {
	for (size_t i = pad_len - len; i > 0; i--)
		write(1, &pad_c, 1);
}

void ft_putnbr_base_padded(unsigned long nb, char *base, size_t base_size, char pad_c, ssize_t pad_n) {
	unsigned long nb2;
	size_t length;

	nb2 = nb;
	length = 1;
	while (nb >= base_size) {
		nb = nb / base_size;
		length++;
	}

	if (pad_n < 0) ft_pad(length, pad_n * -1, pad_c);
	ft_set(nb2, (int)length, base_size, base);
	if (pad_n > 0) ft_pad(length, pad_n, pad_c);
}
