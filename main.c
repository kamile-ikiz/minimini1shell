/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kikiz <kikiz@student.42istanbul.com.tr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 16:06:21 by kikiz             #+#    #+#             */
/*   Updated: 2025/07/15 13:29:12 by kikiz            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <termios.h>
#include <unistd.h>

int main() {
    struct termios oldt, newt;
    char ch;

    // Mevcut terminal ayarlarını al
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;

    // Yeni ayarları yap: canonical mode kapalı (satır satır değil, karakter karakter oku)
    newt.c_lflag &= ~(ICANON | ECHO);  // ECHO kapalı → yazılan gösterilmez
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    printf("Bir tuşa bas (Enter gerekmiyor): ");
    ch = getchar();

    printf("\nBasılan karakter: %c\n", ch);

    // Ayarları geri al
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

    return 0;
}
